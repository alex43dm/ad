#include "DB.h"
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>

#include <ctime>
#include <cstdlib>
#include <sstream>

#include <mongo/util/net/hostandport.h>

#include "../config.h"

#include "Log.h"
#include "BaseCore.h"
#include "Offer.h"
#include "Config.h"

BaseCore::BaseCore()
{
    time_service_started_ = boost::posix_time::second_clock::local_time();

    pdb = new ParentDB();

    LoadAllEntities();

    InitMessageQueue();
}

BaseCore::~BaseCore()
{
    delete amqp_;
}

std::string BaseCore::toString(AMQPMessage *m)
{
    unsigned len;
    char *pMes;

#ifdef AMQPCPP_OLD
    pMes = m->getMessage();
    len = strlen(pMes);
#else
    pMes = m->getMessage(&len);
#endif // AMQPCPP_OLD

    return std::string(pMes,len);
}

bool BaseCore::ProcessMQ()
{
    time_mq_check_ = boost::posix_time::second_clock::local_time();
    try
    {
        // Проверка сообщений advertise.#
        std::string m1, ofrId, cmgId;
        mq_advertise_->Get(AMQP_NOACK);
        AMQPMessage *m = mq_advertise_->getMessage();
        while(m->getMessageCount() > -1)
        {
            Log::gdb("%s advertise: %s",__func__,m->getRoutingKey().c_str());
            m1 = toString(m);
            if(m->getRoutingKey() == "advertise.update")
            {
                if(cmdParser(m1,ofrId,cmgId))
                {
                    pdb->OfferLoad(QUERY("guid" << ofrId));
                }
            }
            else if(m->getRoutingKey() == "advertise.delete")
            {
                if(cmdParser(m1,ofrId,cmgId))
                {
                    pdb->OfferRemove(ofrId);
                }
            }
            m = mq_advertise_->getMessage();
        }
    }
    catch (AMQPException &ex)
    {
        Log::err("%s: AMQPException: %s",__func__,ex.getMessage().c_str());
    }
    return false;
}


/*
*  Загружает из основной базы данных следующие сущности:
*
*  - рекламные предложения;
*  - рекламные кампании;
*  - информеры.
*
*  Если в кампании нет рекламных предложений, она будет пропущена.
*/
void BaseCore::LoadAllEntities()
{
    if(config->pDb->reopen)
    {
        Log::warn("%s: sqlite database didnot open",__func__);
        return;
    }

    //Загрузили все предложения
    pdb->OfferLoad(mongo::Query("{\"retargeting\":true}"));

    Config::Instance()->pDb->indexRebuild();
}

/** \brief  Инициализация очереди сообщений (AMQP).

    Если во время инициализации произошла какая-либо ошибка, то сервис
    продолжит работу, но возможность оповещения об изменениях и горячего
    обновления будет отключена.
*/
void BaseCore::InitMessageQueue()
{
    try
    {
        // Объявляем точку обмена
        amqp_ = new AMQP(config->mq_path_);
        exchange_ = amqp_->createExchange();
        exchange_->Declare("getmyad", "topic", AMQP_AUTODELETE);

        // Составляем уникальные имена для очередей
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        std::string postfix = to_iso_string(now);
        boost::replace_first(postfix, ".", ",");

        std::string mq_advertise_name( "getmyad.advertise." + postfix );

        // Объявляем очереди
        mq_advertise_ = amqp_->createQueue();
        mq_advertise_->Declare(mq_advertise_name, AMQP_AUTODELETE | AMQP_EXCLUSIVE);

        // Привязываем очереди
        exchange_->Bind(mq_advertise_name, "advertise.#");

        Log::info("%s: created ampq queues: %s",__func__,
                  mq_advertise_name.c_str());
    }
    catch (AMQPException &ex)
    {
        Log::err("%s Error in AMPQ init: %s, Feature will be disabled.",__func__, ex.getMessage().c_str());
    }
}

/** Возвращает данные о состоянии службы
 *  TODO Надоб переписать с учётом использования boost::formater красивее будет как некак :)
 */
std::string BaseCore::Status()
{
    // Обработано запросов на момент прошлого обращения к статистике
    static int last_time_request_processed = 0;

    // Время последнего обращения к статистике
    static boost::posix_time::ptime last_time_accessed;

    boost::posix_time::time_duration d;

    // Вычисляем количество запросов в секунду
    if (last_time_accessed.is_not_a_date_time())
        last_time_accessed = time_service_started_;
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    int millisecs_since_last_access =
        (now - last_time_accessed).total_milliseconds();
    int millisecs_since_start =
        (now - time_service_started_).total_milliseconds();
    int requests_per_second_current = 0;
    int requests_per_second_average = 0;
    if (millisecs_since_last_access)
        requests_per_second_current =
            (request_processed_ - last_time_request_processed) * 1000 /
            millisecs_since_last_access;
    if (millisecs_since_start)
        requests_per_second_average = request_processed_ * 1000 /
                                      millisecs_since_start;

    last_time_accessed = now;
    last_time_request_processed = request_processed_;

    std::stringstream out;
    out << "<html>\n"
        "<head><meta http-equiv=\"content-type\" content=\"text/html; "
        "charset=UTF-8\">\n"
        "<style>\n"
        "body { font-family: Arial, Helvetica, sans-serif; }\n"
        "h1, h2, h3 {font-family: \"georgia\", serif; font-weight: 400;}\n"
        "table { border-collapse: collapse; border: 1px solid gray; }\n"
        "td { border: 1px dotted gray; padding: 5px; font-size: 10pt; }\n"
        "th {border: 1px solid gray; padding: 8px; font-size: 10pt; }\n"
        "</style>\n"
        "</head>"
        "<body>\n<h1>Состояние службы Yottos  retargeting</h1>\n"
        "<table>"
        "<tr>"
        "<td>Обработано запросов:</td> <td><b>" << request_processed_ <<
        "</b> (" << requests_per_second_current << "/сек, "
        " в среднем " << requests_per_second_average << "/сек) "
        "</td></tr>\n";
    out << "<tr><td>Имя сервера: </td> <td>" <<
        (getenv("SERVER_NAME")? getenv("SERVER_NAME"): "неизвестно") <<
        "</td></tr>\n";
    out << "<tr><td>Текущее время: </td> <td>" <<
        boost::posix_time::second_clock::local_time() <<
        "</td></tr>\n";

    out << "<tr><td>Время запуска:</td> <td>" << time_service_started_ <<
        "</td></tr>" <<
        out <<  "<tr><td>Сборка: </td><td>" << __DATE__ << " " << __TIME__ <<
        "</td></tr>";
    out << "</table>\n";
    out << "</body>\n</html>\n";

    return out.str();
}

bool BaseCore::cmdParser(const std::string &cmd, std::string &offerId, std::string &campaignId)
{
    boost::regex exp("Offer:(.*),Campaign:(.*)");
    boost::cmatch pMatch;

    if(boost::regex_match(cmd.c_str(), pMatch, exp))
    {
        offerId = pMatch[1];
        campaignId = pMatch[2];
        return true;
    }
    return false;
}
