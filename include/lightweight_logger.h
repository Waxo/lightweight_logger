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
#include <sstream>
#include <string>

class lightweight_logger {
public:

    enum class lw_log_lvl {
        LW_ERROR,
        LW_WARNING,
        LW_INFO
    };

    lightweight_logger &operator<<(const lw_log_lvl l_lvl);
    lightweight_logger &operator<<(const std::string s);

    lightweight_logger();
    ~lightweight_logger();

private:
    std::atomic<bool> _continue;
    std::mutex _m_access_message;
    std::mutex _m_time;
    std::condition_variable _cv_list_event;
    std::future<void> _thread;

    std::list<std::string> _logged_messages;

    std::string _current_log_lvl;
    std::string _current_time;

    void set_time();
    void thread_write_message();

    lightweight_logger(const lightweight_logger &) = delete;

};

#endif /* LIGHTWEIGHT_LOGGER_H */
