#ifndef LIGHTWEIGHT_LOGGER_H_
#define LIGHTWEIGHT_LOGGER_H_

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
  std::atomic<bool> continue_;
  std::mutex m_access_message_;
  std::mutex m_time_;
  std::condition_variable cv_list_event_;
  std::future<void> thread_;

  std::list<std::string> logged_messages_;

  lwl_lvl level_log_setup_;
  lwl_lvl level_current_;
  bool log_in_file_;
  std::ofstream output_file_;
  std::string current_log_lvl_;
  std::string current_time_;
  std::stringstream current_log_text_;

  void set_time();
  void thread_write_message();

  lightweight_logger() = delete;
  lightweight_logger(const lightweight_logger &) = delete;

};

#endif // LIGHTWEIGHT_LOGGER_H_
