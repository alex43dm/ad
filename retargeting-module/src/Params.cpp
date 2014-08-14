#include <sstream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/regex/icu.hpp>
#include <boost/date_time.hpp>

#include <string>

#include "Params.h"
#include "GeoIPTools.h"
#include "Log.h"
#include "Config.h"

Params::Params()
{
    time_ = boost::posix_time::second_clock::local_time();
    tracking_time_ = 365;
}

/// IP посетителя.
Params &Params::ip(const std::string &ip)
{
    struct in_addr ipval;

    ip_ = ip;
    country_ = country_code_by_addr(ip_);
    region_ = region_code_by_addr(ip_);

    if(inet_pton(AF_INET, ip_.c_str(), &ipval))
    {
        key_long = ipval.s_addr;
    }
    else
    {
        key_long = 0;
    }

    return *this;
}

/// ID посетителя, взятый из cookie
Params &Params::cookie_id(const std::string &cookie_id)
{

    cookie_id_ = cookie_id;
    key_long = key_long << 32;
    key_long = key_long | strtol(cookie_id_.c_str(),NULL,10);
    return *this;
}

Params &Params::cookie_tracking_id(const std::string &cookie_tracking_id)
{

    cookie_tracking_id_ = cookie_tracking_id;
    key_tracking_long = key_tracking_long << 32;
    key_tracking_long = key_tracking_long | strtol(cookie_tracking_id_.c_str(),NULL,10);
    return *this;
}

/// Время. По умолчанию равно текущему моменту.
Params &Params::time(const boost::posix_time::ptime &time)
{
    time_ = time;
    return *this;
}
/*
время тракинга в сутках
*/
Params &Params::tracking_time(const std::string &d)
{
    if(!d.empty())
    {
        tracking_time_ = boost::lexical_cast<int>(d);
    }
    else
    {
        tracking_time_ = config->redis_retargeting_.begin()->ttl;
    }
    return *this;
}

long Params::trackingTime() const
{
    return tracking_time_;
}

/** \brief  Двухбуквенный код страны посетителя.

    Если не задан, то страна будет определена по IP.

    Этот параметр используется служебными проверками работы информеров
    в разных странах и в обычной работе не должен устанавливаться.

    \see region()
    \see ip()
*/
Params &Params::country(const std::string &country)
{
    if(!country_.size())
    {
        country_ = country;
    }
    return *this;
}

/** \brief  Гео-политическая область в написании MaxMind GeoCity.

    Если не задана, то при необходимости будет определена по IP.

    Вместе с параметром country() используется служебными проверками
    работы информеров в разных странах и в обычной работе не должен
    устанавливаться.

    \see country()
    \see ip()
*/
Params &Params::region(const std::string &region)
{
    if(!region_.size())
    {
        region_ = region;
    }
    return *this;
}
/** \brief  Виртуальный путь и имя вызываемого скрипта.

    Используется для построения ссылок на самого себя. Фактически,
    сюда должна передаваться сервреная переменная SCRIPT_NAME.
*/
Params &Params::script_name(const char *script_name)
{
    script_name_ = script_name? script_name : "";
    return *this;
}

/** \brief  Адрес страницы, на которой показывается информер.

    Обычно передаётся javascript загрузчиком.
*/
Params &Params::location(const char *location)
{
    return this->location(location? location : "");
}

/** \brief  Адрес страницы, на которой показывается информер.

    Обычно передаётся javascript загрузчиком.
*/
Params &Params::location(const std::string &location)
{
    location_ = location;
    return *this;
}

/**
 * строка, содержашяя контекст страницы
 */
Params &Params::context(const std::string &context)
{
    context_ = context;
    return *this;
}
Params &Params::context(const char *context)
{
    return this->context(context? context : "");
}
/**
 * строка, содержашяя поисковый запрос
 */
Params &Params::search(const std::string &search)
{
    search_ = search;
    return *this;
}

Params &Params::search(const char *search)
{
    return this->search(search? search : "");
}

std::string Params::getIP() const
{
    return ip_;
}
std::string Params::getCookieId() const
{
    return cookie_id_;
}

std::string Params::getUserKey() const
{
    return cookie_id_ + "-" + ip_;
}

unsigned long long Params::getUserKeyLong() const
{
    return key_long;
}

std::string Params::getCountry() const
{
    return country_;
}
std::string Params::getRegion() const
{
    return region_;
}

boost::posix_time::ptime Params::getTime() const
{
    return time_;
}

std::string Params::getScriptName() const
{
    return script_name_;
}

std::string Params::getLocation() const
{
    return location_;
}

std::string Params::getContext() const
{
    return context_;
}

std::string Params::getSearch() const
{
    return search_;
}

Params &Params::host(const std::string &host)
{
    host_ = host;
    return *this;
}

std::string Params::getHost() const
{
    return host_;
}

Params &Params::retargeting_id(const std::string &retargeting_id)
{
    retargetingId_ = boost::to_lower_copy(retargeting_id);
    return *this;
}

std::string Params::retargetingId() const
{
    return retargetingId_;
}

Params &Params::account_id(const std::string &account_id)
{
    account_id_ = boost::to_lower_copy(account_id);
    return *this;
}

std::string Params::accountId() const
{
    return account_id_;
}
