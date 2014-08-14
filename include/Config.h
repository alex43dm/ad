#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

#include <tinyxml.h>
#include "redis_server.h"
#include "DataBase.h"
#include "RedisClient.h"

extern unsigned long request_processed_;
extern unsigned long last_time_request_processed;
extern unsigned long offer_processed_;
extern unsigned long social_processed_;

#define REDIS_DEFAULT_TTL 3600*14

class Config
{
public:
    std::vector<redis_server> redis_short_term_, redis_retargeting_;

    std::vector<std::string> mongo_main_host_;
    std::string mongo_main_db_;
    std::string mongo_main_set_;
    bool mongo_main_slave_ok_;
    std::string mongo_main_login_;
    std::string mongo_main_passwd_;

    std::string mq_path_;

    //new params
    std::string server_ip_;
    std::string server_socket_path_;
    int server_children_;

    std::string dbpath_;
    std::string db_dump_path_;
    DataBase *pDb;
    Module *module;
    std::string offerSqlAll;

    std::string cookie_name_;
    std::string cookie_domain_;
    std::string cookie_path_;
    std::string cookie_tracking_name_;
    std::string cookie_tracking_domain_;
    std::string cookie_tracking_path_;

    std::string geocity_path_;

    int         instanceId;
    std::string lock_file_;
    std::string pid_file_;
    std::string user_;

    std::string template_out_;
    std::string template_error_;
    int time_update_;
    bool logCoretime, logKey, logCountry, logRegion, logContext, logSearch, logAccountId, logOutPutOfferIds;

    static Config* Instance();
    bool LoadConfig(const std::string fName);
    bool ReLoad();
    virtual ~Config();

    bool to_bool(std::string const& s)
    {
        return s != "false";
    }
    float to_float(std::string const& s)
    {
        return atof(s.c_str());
    }
    int to_int(std::string const& s)
    {
        return atoi(s.c_str());
    }

protected:
private:
    static Config* mInstance;
    Config();
    bool mIsInited;
    std::string mes;
    std::string mFileName;
    std::string cfgFilePath;

    int getTime(const char *p);
    std::string getFileContents(const std::string &fileName);
    void exit(const std::string &mes);
    bool checkPath(const std::string &path_, bool checkWrite, bool isFile, std::string &mes);
    bool Load();
};

extern Config *config;

#endif // CONFIG_H

