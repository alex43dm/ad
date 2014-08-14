#ifndef CORE_H
#define CORE_H

#include <vector>

#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>

#include "Params.h"
#include "RedisClient.h"

/// Класс, который связывает воедино все части системы.
class Core
{
public:
    /** \brief  Создаёт ядро.
     *
     * Производит все необходимые инициализации:
     *
     * - Загружает все сущности из базы данных;
     * - Подключается к RabbitMQ и создаёт очереди сообщений;
     * - При необходимости создаёт локальную базу данных MongoDB с нужными
     *   параметрами.
     */
    Core();

    /** Пытается красиво закрыть очереди RabbitMQ, но при работе с FastCGI
     *  никогда не вызывается (как правило, процессы просто снимаются).
     */
    ~Core();

    /** \brief  Обработка запроса на показ рекламы.
     * Самый главный метод. Возвращает HTML-строку, которую нужно вернуть
     * пользователю.
     */
    void Process(Params *params);
    void PostProcess();

private:
    pthread_t tid;
    std::vector<RedisClient *> rcRetargeting, rcShortTerm;
    Params *params;
    char *cmd;

    bool getOffer(std::vector<long> &result);
};

#endif // CORE_H
