/*
 * File:   lightweight_logger.h
 * Author: Maxime ROBIN
 *
 * Created on 20 février 2015, 10:12
 */

#ifndef LIGHTWEIGHT_LOGGER_H
#define LIGHTWEIGHT_LOGGER_H

#include <unistd.h>
#include <iostream>
#include <mutex>
#include <future>
#include <list>
#include <string>

class lightweight_logger {
public:

    void add_message(std::string m);
    void stop();

    lightweight_logger();
    ~lightweight_logger();

private:
    std::atomic<bool> _continue;
    std::mutex _m_access_message;
    std::condition_variable _m_list_empty;
    std::future<void> _thread;

    std::list<std::string> _messages;


    void thread_write_message();

    lightweight_logger(const lightweight_logger&)=delete;

};

#endif /* LIGHTWEIGHT_LOGGER_H */
