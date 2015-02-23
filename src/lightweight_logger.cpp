/*
 * File:   lightweight_logger.cpp
 * Author: Maxime ROBIN
 *
 * Created on 20 février 2015, 10:12
 */

#include <lightweight_logger.h>

lightweight_logger::lightweight_logger(const lwl_lvl &log_level) {
    this->_level_log_setup = log_level;
    if (this->_level_log_setup != lwl_lvl::LWL_OFF) {
        this->_log_in_file = false;
        this->_continue.store(true, std::memory_order_relaxed);
        this->_thread = std::async(std::launch::async, &lightweight_logger::thread_write_message, this);
    }
}

lightweight_logger::lightweight_logger(const lwl_lvl &log_level, const std::string &filename) {
    this->_level_log_setup = log_level;
    if (this->_level_log_setup != lwl_lvl::LWL_OFF) {
        this->_log_in_file = true;
        this->_output_file.open(filename);
        this->_continue.store(true, std::memory_order_relaxed);
        this->_thread = std::async(std::launch::async, &lightweight_logger::thread_write_message, this);
    }
}

lightweight_logger::~lightweight_logger() {
    if (this->_level_log_setup != lwl_lvl::LWL_OFF) {
        while (this->_logged_messages.size()) {
            _cv_list_event.notify_one();
        }

        this->_continue.store(false, std::memory_order_relaxed);
        _cv_list_event.notify_one();
        this->_thread.get();

        if (this->_log_in_file) {
            this->_output_file.close();
        }
    }
}

void lightweight_logger::thread_write_message() {
    while (this->_continue.load(std::memory_order_relaxed)) {
        std::unique_lock<std::mutex> l(this->_m_access_message);
        if (this->_logged_messages.size()) {
            if (this->_log_in_file) {
                this->_output_file << this->_logged_messages.front() << std::endl;
            } else {
                std::cout << this->_logged_messages.front() << std::endl;
            }
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
    s << " " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "\t";

    std::lock_guard<std::mutex> l(this->_m_time);
    this->_current_time = s.str();
}

lightweight_logger &lightweight_logger::operator<<(const lwl_lvl l_lvl) {
    this->_level_current = l_lvl;

    if (this->_level_log_setup != lwl_lvl::LWL_OFF && this->_level_log_setup <= this->_level_current) {
        std::future<void> _t = std::async(std::launch::async, &lightweight_logger::set_time, this);

        std::lock_guard<std::mutex> l(this->_m_access_message);
        switch (this->_level_current) {
            case lwl_lvl::LWL_TRACE:
                this->_current_log_lvl = "[TRACE]\t";
                break;
            case lwl_lvl::LWL_INFO:
                this->_current_log_lvl = "[INFO]\t";
                break;
            case lwl_lvl::LWL_DEBUG:
                this->_current_log_lvl = "[DEBUG]\t";
                break;
            case lwl_lvl::LWL_WARNING:
                this->_current_log_lvl = "[WARNING]\t";
                break;
            case lwl_lvl::LWL_ERROR:
                this->_current_log_lvl = "[ERROR]\t";
                break;
            case lwl_lvl::LWL_VERBOSE:
                this->_current_log_lvl = "[VERBOSE]\t";
                break;
            default:
                break;
        }

        _t.get();
    }

    return *this;
}

lightweight_logger &lightweight_logger::operator<<(ostream_manipulator s) {
    if (this->_level_log_setup != lwl_lvl::LWL_OFF && this->_level_log_setup <= this->_level_current) {
        std::unique_lock<std::mutex> l(this->_m_access_message);
        std::lock_guard<std::mutex> m(this->_m_time);
        this->_logged_messages.push_back(this->_current_time + this->_current_log_lvl + this->_current_log_text.str());
        this->_current_log_text.str("");
        _cv_list_event.notify_one();
    }
    return *this;
}

lightweight_logger &lightweight_logger::operator<<(const std::string s) {
    if (this->_level_log_setup != lwl_lvl::LWL_OFF && this->_level_log_setup <= this->_level_current) {
        std::lock_guard<std::mutex> l(this->_m_access_message);
        this->_current_log_text << s;
    }
    return *this;
}

lightweight_logger &lightweight_logger::operator<<(const int s) {
    if (this->_level_log_setup != lwl_lvl::LWL_OFF && this->_level_log_setup <= this->_level_current) {
        std::lock_guard<std::mutex> l(this->_m_access_message);
        this->_current_log_text << s;
    }
    return *this;
}

lightweight_logger &lightweight_logger::operator<<(const double s) {
    if (this->_level_log_setup != lwl_lvl::LWL_OFF && this->_level_log_setup <= this->_level_current) {
        std::lock_guard<std::mutex> l(this->_m_access_message);
        this->_current_log_text << s;
    }
    return *this;
}

lightweight_logger &lightweight_logger::operator<<(const float s) {
    if (this->_level_log_setup != lwl_lvl::LWL_OFF && this->_level_log_setup <= this->_level_current) {
        std::lock_guard<std::mutex> l(this->_m_access_message);
        this->_current_log_text << s;
    }
    return *this;
}

lightweight_logger &lightweight_logger::operator<<(const short s) {
    if (this->_level_log_setup != lwl_lvl::LWL_OFF && this->_level_log_setup <= this->_level_current) {
        std::lock_guard<std::mutex> l(this->_m_access_message);
        this->_current_log_text << s;
    }
    return *this;
}

lightweight_logger &lightweight_logger::operator<<(const char s) {
    if (this->_level_log_setup != lwl_lvl::LWL_OFF && this->_level_log_setup <= this->_level_current) {
        std::lock_guard<std::mutex> l(this->_m_access_message);
        this->_current_log_text << s;
    }
    return *this;
}
