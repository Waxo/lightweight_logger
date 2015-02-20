/*
 * File:   lightweight_logger.cpp
 * Author: Maxime ROBIN
 *
 * Created on 20 février 2015, 10:12
 */

#include <lightweight_logger.h>

lightweight_logger::lightweight_logger() {
    this->_continue.store(true, std::memory_order_relaxed);
    this->_thread = std::async(std::launch::async, &lightweight_logger::thread_write_message, this);
}

lightweight_logger::~lightweight_logger() {
    while (this->_logged_messages.size()) {
        _cv_list_event.notify_one();
    }
    this->_continue.store(false, std::memory_order_relaxed);
    this->_thread.get();

}

void lightweight_logger::thread_write_message() {
    while (this->_continue.load(std::memory_order_relaxed)) {
        std::unique_lock<std::mutex> l(this->_m_access_message);
        if (this->_logged_messages.size()) {
            std::cout << this->_logged_messages.front() << std::endl;
            this->_logged_messages.pop_front();
        } else {
            this->_cv_list_event.wait(l);
        }
    }
}

void lightweight_logger::set_time() {
    time_t t = time(0);
    tm *now = localtime(&t);

    std::stringstream s;
    s << now->tm_mday << "-" << (now->tm_mon + 1) << "-" << (now->tm_year + 1900);
    s << " " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "\t- ";

    std::lock_guard<std::mutex> l(this->_m_time);
    this->_current_time = s.str();
}

lightweight_logger &lightweight_logger::operator<<(const lw_log_lvl l_lvl) {

    std::future<void> _t = std::async(std::launch::async, &lightweight_logger::set_time, this);

    std::lock_guard<std::mutex> l(this->_m_access_message);
    if (l_lvl == lightweight_logger::lw_log_lvl::LW_ERROR) {
        this->_current_log_lvl = "[ERROR]\t- ";
    } else if (l_lvl == lightweight_logger::lw_log_lvl::LW_WARNING) {
        this->_current_log_lvl = "[WARNING]\t- ";
    } else if (l_lvl == lightweight_logger::lw_log_lvl::LW_INFO) {
        this->_current_log_lvl = "[INFO]\t- ";
    }

    _t.get();

    return *this;
}

lightweight_logger &lightweight_logger::operator<<(const std::string s) {
    std::unique_lock<std::mutex> l(this->_m_access_message);
    std::lock_guard<std::mutex> m(this->_m_time);
    this->_logged_messages.push_back(this->_current_time + this->_current_log_lvl + s);
    _cv_list_event.notify_one();

    return *this;
}