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
#include <fstream>
#include <sstream>
#include <mutex>
#include <future>
#include <list>
#include <string>


typedef std::ostream &(*ostream_manipulator)(std::ostream &);

class lightweight_logger {
public:

    enum class lwl_lvl {
        LWL_TRACE,
        LWL_DEBUG,
        LWL_INFO,
        LWL_WARNING,
        LWL_ERROR,
        LWL_VERBOSE,
        LWL_OFF
    };

    lightweight_logger &operator<<(const lwl_lvl l_lvl);
    lightweight_logger &operator<<(const ostream_manipulator s);

    //basic types
    lightweight_logger &operator<<(const std::string s);
    lightweight_logger &operator<<(const int s);
    lightweight_logger &operator<<(const float s);
    lightweight_logger &operator<<(const double s);
    lightweight_logger &operator<<(const short s);
    lightweight_logger &operator<<(const char s);

    lightweight_logger(const lwl_lvl &log_level);
    lightweight_logger(const lwl_lvl &log_level, const std::string &filename);
    ~lightweight_logger();

private:
    std::atomic<bool> _continue;
    std::mutex _m_access_message;
    std::mutex _m_time;
    std::condition_variable _cv_list_event;
    std::future<void> _thread;

    std::list<std::string> _logged_messages;

    lwl_lvl _level_log_setup;
    lwl_lvl _level_current;
    bool _log_in_file;
    std::ofstream _output_file;
    std::string _current_log_lvl;
    std::string _current_time;
    std::stringstream _current_log_text;

    void set_time();
    void thread_write_message();

    lightweight_logger() = delete;
    lightweight_logger(const lightweight_logger &) = delete;

};

#endif /* LIGHTWEIGHT_LOGGER_H */
