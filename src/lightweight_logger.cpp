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
    this->_continue.store(false, std::memory_order_relaxed);
    _m_list_empty.notify_one();
    this->_thread.get();

}

void lightweight_logger::add_message(std::string m) {
    std::unique_lock<std::mutex> l(this->_m_access_message);
    this->_messages.push_back(m);
    _m_list_empty.notify_one();
}

void lightweight_logger::thread_write_message() {
    while (this->_continue.load(std::memory_order_relaxed)) {
        std::unique_lock<std::mutex> l(this->_m_access_message);
        if (this->_messages.size()) {
            std::cout << std::endl << this->_messages.front() << std::endl;
            this->_messages.pop_front();
        } else {
            this->_m_list_empty.wait(l);
        }
    }
}