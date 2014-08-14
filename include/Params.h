#ifndef PARAMS_H
#define PARAMS_H

#include <sstream>
#include <string>
#include <boost/date_time.hpp>

//#include "utils/GeoIPTools.h"

/** \brief Параметры, которые определяют показ рекламы */
class Params
{
public:
    Params();
    /// IP посетителя.
    Params &ip(const std::string &ip);
    /// ID посетителя, взятый из cookie
    Params &cookie_id(const std::string &cookie_id);
    Params &cookie_tracking_id(const std::string &cookie_tracking_id);
    /// Время. По умолчанию равно текущему моменту.
    Params &time(const boost::posix_time::ptime &time);
    /** \brief  Двухбуквенный код страны посетителя.

        Если не задан, то страна будет определена по IP.

        Этот параметр используется служебными проверками работы информеров
        в разных странах и в обычной работе не должен устанавливаться.

        \see region()
        \see ip()
    */
    Params &country(const std::string &country);
    /** \brief  Гео-политическая область в написании MaxMind GeoCity.

        Если не задана, то при необходимости будет определена по IP.

        Вместе с параметром country() используется служебными проверками
        работы информеров в разных странах и в обычной работе не должен
        устанавливаться.

        \see country()
        \see ip()
    */
    Params &region(const std::string &region);
    /** \brief  Виртуальный путь и имя вызываемого скрипта.

        Используется для построения ссылок на самого себя. Фактически,
        сюда должна передаваться сервреная переменная SCRIPT_NAME.
    */
    Params &script_name(const char *script_name);
    /** \brief  Адрес страницы, на которой показывается информер.

        Обычно передаётся javascript загрузчиком.
    */
    Params &location(const char *location);
    /** \brief  Адрес страницы, на которой показывается информер.

        Обычно передаётся javascript загрузчиком.
    */
    Params &location(const std::string &location);
    /**
     * строка, содержашяя контекст страницы
     */
    Params &context(const std::string &context);
    Params &context(const char *context);
    Params &account_id(const std::string &account_id);
    Params &host(const std::string &host);
    /**
     * строка, содержашяя поисковый запрос
     */
    Params &search(const std::string &search);
    Params &search(const char *search);
    Params &tracking_time(const std::string &);
    //*********************************************************************************************//
    std::string getIP() const;
    std::string getHost() const;
    std::string getCookieId() const;
    std::string getUserKey() const;
    unsigned long long getUserKeyLong() const;
    std::string getCountry() const;
    std::string getRegion() const;
    boost::posix_time::ptime getTime() const;
    std::string getScriptName() const;
    std::string getLocation() const;
    std::string getContext() const;
    std::string getSearch() const;
    std::string accountId() const;
    long trackingTime() const;
    std::string retargetingId() const;
    Params &retargeting_id(const std::string &retargeting_id);

    friend class Core;
    friend class GenerateToken;
    friend class HistoryManager;//добавлено RealInvest Soft

private:
    unsigned long long key_long, key_tracking_long;
    std::string ip_;
    std::string host_;
    std::string cookie_id_;
    std::string cookie_tracking_id_;
    std::string country_;
    std::string countryByIp_;
    std::string region_;
    boost::posix_time::ptime time_;
    std::string script_name_;
    std::string location_;
    std::string context_;//строка содержашяя контекст страницы
    std::string search_;
    std::string account_id_;
    long tracking_time_;
    std::string retargetingId_;
};

#endif // PARAMS_H
