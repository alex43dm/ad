#include <unistd.h>

#include "CgiService.h"
#include "Log.h"
#include "Server.h"

Config *config;
/*
perl -MURI::Escape -e 'print uri_escape("762FA8DB-66A1-4C96-89F5-5E0C77E74F3A")'

http://127.0.0.1/adshow.fcgi?offer_id=8432100&ac=762FA8DB-66A1-4C96-89F5-5E0C77E74F3A&time=14
http://127.0.0.1/adshow.fcgi?location=http%3A%2F%2Fwww.topshoptv.com.ua%2Fkyhnya%2Ftovary_za_tipom%2Fbullit-express.html%3Fcsu%3Dgeneral%26cex%3D1599765%26med%3Dom%26src%3Dua_gen_general_om_sa_yottos%26referrer%3Dyottos_om_1599765%26utm_source%3Dyottos_ext%26utm_medium%3DOff-Site_Merchandising_rem%26utm_campaign%3Dua_gen_general_om_sa_yottos_1599765%26utm_content%3DYottos_Remarketing_1599765&time=365&ac=762FA8DB-66A1-4C96-89F5-5E0C77E74F3A&rand=2353465&search=&context=%D0%9A%D1%83%D1%85%D0%BE%D0%BD%D0%BD%D0%B8%D0%B9%20%D0%BA%D0%BE%D0%BC%D0%B1%D0%B0%D0%B9%D0%BD
*/
int main(int argc, char *argv[])
{
    std::clog.rdbuf(new Log(LOG_LOCAL0));

//    std::locale::global(std::locale("ru_RU.UTF-8"));

    std::string config_file = "config.xml";
    std::string sock_path;
    int ret;

    bool fPrintPidFile = false;
    bool fTestConfig = false;

    while ( (ret = getopt(argc,argv,"tpc:s:")) != -1)
    {
        switch (ret)
        {
        case 'c':
            config_file = optarg;
            break;
        case 's':
            sock_path = optarg;
            break;
        case 'p':
            fPrintPidFile = true;
            break;
        case 't':
            fTestConfig = true;
            break;
        default:
            printf("Error found! %s -c config_file -s socket_path\n",argv[0]);
            ::exit(1);
        };
    };


    config = Config::Instance();
    config->LoadConfig(config_file);

    if(fTestConfig)
    {
        std::cerr<<"Config: Ok"<<std::endl;
        ::exit(0);
    }

    if(fPrintPidFile)
    {
        std::cerr<<config->pid_file_<<std::endl;
        ::exit(0);
    }

#ifndef DEBUG
    new Server(config->lock_file_, config->pid_file_);
#endif

    if( sock_path.size() > 8 )
    {
        config->server_socket_path_ = sock_path;
    }

    CgiService().run();

    return 0;
}
