#ifndef CGISERVICE_H
#define CGISERVICE_H

#include <string>

#include <fcgiapp.h>

#include "Config.h"
#include "BaseCore.h"

class Core;


/**
\brief Класс, оборачивающий проект как FastCgi (или просто CGI) сервис.


 Этот класс изолирует ядро сервиса от всей специфичной для веб-сервера
 информации. Он извлекает настройки сервера из командной строки, парсит
 строку запроса и передаёт эти "чистые" данные в экземляр класса \rel Core,
 который уже занимается реальной обработкой запроса.

	\see Serve().
 */
class CgiService
{

public:
    int socketId;
    /** Инициализирует сервис.
     *
     * Читает настройки из переменных окружения.
     *
     * Реально подключения к базам данных, RabbitMQ и другому происходит
     * при первом запросе в методе Serve().
     */
    CgiService();
    ~CgiService();

    /** Начало обработки поступающих от FastCgi (или CGI) сервера запросов.
     *
     * Если запущен CGI сервером или просто из командной строки, то обработает
     * один запрос и вернёт управление.
     *
     * Если запущен FastCgi сервером, то будет ждать и обрабатывать следующие
     * запросы.
     */
    static void *Serve(void*);
    void run();

private:
    /** Возвращает ответ FastCgi серверу.
     *
     * \param out       указатель на строку ответа.
     * \param status    HTTP статус ответа (например, 200, 404 или 500).
     * \param content_type Заголовок "Content-Type".
     * \param cookie    Заголовок "Set-Cookie". Параметр передается в виде обертки Cookie
     */
    void Response(FCGX_Request *stream, const std::string &out,
                  const std::string &c = "",
                  const std::string &c1 = "");

    void Response(FCGX_Request *stream, int);

    /** Обработка одного запроса.
     *
     * \param query     строка запроса (всё, что начинается после
     *                  вопросительного знака в URL запроса).
     * \param ip        IP посетителя, сделавшего запрос.
     * \param script_name название скрипта (то, что идёт до вопросительного
     *                  знака). Необходимо получать это значение от веб-сервера,
     *                  поскольку может меняться в зависимости от настроек
     *                  FastCGI. Используется для построения ссылки на новую
     *                  порцию предложений.
     */
    void ProcessRequest(FCGX_Request*, Core *);
    static void SignalHandler(int signum);
private:
    pthread_t *threads;
    BaseCore *bcore;
};

#endif
