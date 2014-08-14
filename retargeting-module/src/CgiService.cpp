#include <sys/stat.h>

#include <boost/algorithm/string.hpp>

#include "Log.h"
#include "CgiService.h"
#include "UrlParser.h"
#include "Core.h"
#include "Server.h"
#include "Cookie.h"

#include <fcgi_stdio.h>

#define THREAD_STACK_SIZE PTHREAD_STACK_MIN + 10 * 1024

std::string time_t_to_string(time_t t)
{
    std::stringstream sstr;
    sstr << t;
    return sstr.str();
}

CgiService::CgiService()
{
    bcore = new BaseCore();

    FCGX_Init();

    mode_t old_mode = umask(0);

    socketId = FCGX_OpenSocket(config->server_socket_path_.c_str(), config->server_children_);
    if(socketId < 0)
    {
        Log::err("Error open socket. exit");
        exit(1);
    }
    umask(old_mode);

    pthread_attr_t* attributes = (pthread_attr_t*) malloc(sizeof(pthread_attr_t));
    pthread_attr_init(attributes);
    pthread_attr_setstacksize(attributes, THREAD_STACK_SIZE);

    struct sigaction actions;

    memset(&actions, 0, sizeof(actions));
    actions.sa_flags = 0;
    actions.sa_handler = SignalHandler;

    sigaction(SIGHUP,&actions,NULL);
    sigaction(SIGPIPE,&actions,NULL);

    threads = new pthread_t[config->server_children_ + 1];

    for(int i = 0; i < config->server_children_; i++)
    {
        if(pthread_create(&threads[i], attributes, &this->Serve, this))
        {
            Log::err("creating thread failed");
        }
    }
    pthread_attr_destroy(attributes);
}

CgiService::~CgiService()
{
    for(int i = 0; i < config->server_children_; i++)
    {
        pthread_join(threads[i], 0);
    }

    delete []threads;
}

void CgiService::run()
{
    int memSize = 0, newMemSize;
    //main loop
    for(;;)
    {
        //read mq and process
        bcore->ProcessMQ();

        newMemSize = Log::memUsage();
        if(newMemSize != memSize)
        {
            std::clog<<"VmSize:"<<newMemSize<<" diff:"<<newMemSize - memSize<<std::endl;
            memSize = newMemSize;
        }

        sleep(1);
    }
}

void CgiService::Response(FCGX_Request *req,
                          const std::string &out,
                          const std::string &cookie,
                          const std::string &cookie_track)
{
    FCGX_FPrintF(req->out,"Content-type: text/html\r\n");
    FCGX_FPrintF(req->out,"Set-Cookie: %s\r\n", cookie.c_str());
    FCGX_FPrintF(req->out,"Set-Cookie: %s\r\n", cookie_track.c_str());
    FCGX_FPrintF(req->out,"Status: 200 OK\r\n");
    FCGX_FFlush(req->out);
    FCGX_FPrintF(req->out,"\r\n%s\n", out.c_str());
    FCGX_Finish_r(req);
}

void CgiService::Response(FCGX_Request *req, int status)
{
    FCGX_FPrintF(req->out,"Content-type: text/html\r\n");

    FCGX_FPrintF(req->out,"Status: ");
    switch (status)
    {
    case 200:
        FCGX_FPrintF(req->out,"200 OK");
        break;
    case 301:
        FCGX_FPrintF(req->out,"301 Moved Permanently");
        break;
    case 302:
        FCGX_FPrintF(req->out,"302 Found");
        break;
    case 307:
        FCGX_FPrintF(req->out,"307 Temporary Redirect");
        break;
    case 400:
        FCGX_FPrintF(req->out,"400 Bad Request");
        break;
    case 403:
        FCGX_FPrintF(req->out,"403 Forbidden");
        break;
    case 500:
        FCGX_FPrintF(req->out,"500 Internal Server Error");
        break;
    case 503:
        FCGX_FPrintF(req->out,"503 Service Unavailable");
        break;
    default:
        FCGX_FPrintF(req->out,"200 OK");
        break;
    }
    FCGX_FPrintF(req->out,"\r\n");
    FCGX_FFlush(req->out);
    FCGX_Finish_r(req);
}


void *CgiService::Serve(void *data)
{
    CgiService *csrv = (CgiService*)data;

    Core *core = new Core();
//    core->set_server_ip(Config::Instance()->server_ip_);
//    core->set_redirect_script(Config::Instance()->redirect_script_);

    FCGX_Request request;

    if(FCGX_InitRequest(&request, csrv->socketId, 0) != 0)
    {
        Log::err("Can not init request");
        return nullptr;
    }

    for(;;)
    {
        static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&accept_mutex);
        int rc = FCGX_Accept_r(&request);
        pthread_mutex_unlock(&accept_mutex);

        if(rc < 0)
        {
            Log::err("Can not accept new request");
            break;
        }

        csrv->ProcessRequest(&request, core);
    }

    return nullptr;
}


void CgiService::ProcessRequest(FCGX_Request *req, Core *core)
{
    char *tmp_str = nullptr;
    std::string cookie_value, cookie_value_tracking;
    std::string query, ip, host;

    if (!(tmp_str = FCGX_GetParam("QUERY_STRING", req->envp)))
    {
        std::clog<<"query string is not set"<<std::endl;
        return;
    }
    else
    {
        query = std::string(tmp_str);
        if(!query.size() || query == "/favicon.ico")
        {
            Response(req, "favicon.ico", "");
            return;
        }
    }

    tmp_str = nullptr;
    if( !(tmp_str = FCGX_GetParam("REMOTE_ADDR", req->envp)) )
    {
        std::clog<<"remote address is not set"<<std::endl;
        return;
    }
    else
    {
        ip = std::string(tmp_str);
    }


    tmp_str = nullptr;
    if( !(tmp_str = FCGX_GetParam("REMOTE_HOST", req->envp)) )
    {
    }
    else
    {
        host = std::string(tmp_str);
    }

    /*
        tmp_str = nullptr;
        if (!(tmp_str = FCGX_GetParam("SCRIPT_NAME", req->envp)))
        {
            Log::warn("script name is not set");
            return;
        }
        else
        {
            script_name = std::string(tmp_str);
        }
    */
    tmp_str = nullptr;
    if (!(tmp_str = FCGX_GetParam("HTTP_COOKIE", req->envp)))
    {
        std::clog<<"cookie is not set"<<std::endl;
        cookie_value = time_t_to_string(time(NULL));
        cookie_value_tracking = cookie_value;
    }
    else
    {
        std::string visitor_cookie = std::string(tmp_str);

        std::vector<std::string> strs;
        boost::split(strs, visitor_cookie, boost::is_any_of(";"));

        for (unsigned int i=0; i<strs.size(); i++)
        {
            if(strs[i].find(config->cookie_name_) != std::string::npos)
            {
                std::vector<std::string> name_value;
                boost::split(name_value, strs[i], boost::is_any_of("="));
                if (name_value.size() == 2)
                    cookie_value = name_value[1];
            }
            if(strs[i].find(config->cookie_tracking_name_) != std::string::npos)
            {
                std::vector<std::string> name_value_tracking;
                boost::split(name_value_tracking, strs[i], boost::is_any_of("="));
                if (name_value_tracking.size() == 2)
                    cookie_value_tracking = name_value_tracking[1];
            }
        }
    }

    if (cookie_value != cookie_value_tracking)
    {
        cookie_value_tracking = cookie_value;
    }

    UrlParser url(query);

    if (url.param("show") == "status")
    {
        Response(req, bcore->Status(), "", "");
        return;
    }

    Params prm = Params()
                 .ip(ip)
                 .host(host)
                 .cookie_id(cookie_value)
                 .cookie_tracking_id(cookie_value_tracking)
                 .country(url.param("country"))
                 .region(url.param("region"))
                 .account_id(url.param("ac"))
                 .location(url.param("location"))
                 .search(url.param("search"))
                 .context(url.param("context"))
                 .tracking_time(url.param("time"))
                 .retargeting_id(url.param("offer_id"));

    ClearSilver::Cookie c = ClearSilver::Cookie(config->cookie_name_,
                            cookie_value,
                            ClearSilver::Cookie::Credentials(
                                ClearSilver::Cookie::Authority(config->cookie_domain_),
                                ClearSilver::Cookie::Path(config->cookie_path_),
                                ClearSilver::Cookie::Expires(boost::posix_time::second_clock::local_time() + boost::gregorian::years(1))));

    ClearSilver::Cookie::Expires expires_tracking =
        ClearSilver::Cookie::Expires(
            boost::posix_time::second_clock::local_time() +
            boost::gregorian::days(prm.trackingTime()));

    ClearSilver::Cookie c1 = ClearSilver::Cookie(config->cookie_name_,
                             cookie_value,
                             ClearSilver::Cookie::Credentials(
                                 ClearSilver::Cookie::Authority(config->cookie_tracking_domain_),
                                 ClearSilver::Cookie::Path(config->cookie_tracking_path_),
                                 expires_tracking));

    Response(req, config->template_out_, c.to_string(), c1.to_string());

    try
    {
        core->Process(&prm);
    }
    catch (std::exception const &ex)
    {
        Response(req, 503);
        Log::err("%s exception %s: name: %s while processing: %s",__func__,
                 typeid(ex).name(), ex.what(), query.c_str());
    }
}

void CgiService::SignalHandler(int signum)
{
    switch(signum)
    {
    case SIGHUP:
        std::clog<<"CgiService: sig hup"<<std::endl;
        config->ReLoad();
        break;
    case SIGPIPE:
        std::clog<<"CgiService: sig pipe"<<std::endl;
        break;
    }
}
