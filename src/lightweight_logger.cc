#include <lightweight_logger.h>

lightweight_logger::lightweight_logger(const lwl_lvl &log_level) {
  this->level_log_setup_ = log_level;
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF) {
    this->log_in_file_ = false;
    this->continue_.store(true, std::memory_order_relaxed);
    this->thread_ = std::async(std::launch::async, &lightweight_logger::thread_write_message, this);
  }
}

lightweight_logger::lightweight_logger(const lwl_lvl &log_level, const std::string &filename) {
  this->level_log_setup_ = log_level;
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF) {
    this->log_in_file_ = true;
    this->output_file_.open(filename);
    this->continue_.store(true, std::memory_order_relaxed);
    this->thread_ = std::async(std::launch::async, &lightweight_logger::thread_write_message, this);
  }
}

lightweight_logger::~lightweight_logger() {
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF) {
    while (this->logged_messages_.size()) {
      cv_list_event_.notify_one();
    }

    this->continue_.store(false, std::memory_order_relaxed);
    cv_list_event_.notify_one();
    this->thread_.get();

    if (this->log_in_file_) {
      this->output_file_.close();
    }
  }
}

void lightweight_logger::thread_write_message() {
  while (this->continue_.load(std::memory_order_relaxed)) {
    std::unique_lock<std::mutex> l(this->m_access_message_);
    if (this->logged_messages_.size()) {
      if (this->log_in_file_) {
        this->output_file_ << this->logged_messages_.front() << std::endl;
      } else {
        std::cout << this->logged_messages_.front() << std::endl;
      }
      this->logged_messages_.pop_front();
    } else {
      this->cv_list_event_.wait(l);
    }
  }
}

void lightweight_logger::set_time() {
  time_t t = time(0);
  tm *now = localtime(&t);

  std::stringstream s;
  s << now->tm_mday << "-" << (now->tm_mon + 1) << "-" << (now->tm_year + 1900);
  s << " " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "\t";

  std::lock_guard<std::mutex> l(this->m_time_);
  this->current_time_ = s.str();
}

lightweight_logger &lightweight_logger::operator<<(const lwl_lvl l_lvl) {
  this->level_current_ = l_lvl;

  if (this->level_log_setup_ != lwl_lvl::LWL_OFF && this->level_log_setup_ <= this->level_current_) {
    std::future<void> _t = std::async(std::launch::async, &lightweight_logger::set_time, this);

    std::lock_guard<std::mutex> l(this->m_access_message_);
    switch (this->level_current_) {
      case lwl_lvl::LWL_TRACE:
        this->current_log_lvl_ = "[TRACE]\t";
        break;
      case lwl_lvl::LWL_DEBUG:
        this->current_log_lvl_ = "[DEBUG]\t";
        break;
      case lwl_lvl::LWL_INFO:
        this->current_log_lvl_ = "[INFO]\t";
        break;
      case lwl_lvl::LWL_WARNING:
        this->current_log_lvl_ = "[WARNING]\t";
        break;
      case lwl_lvl::LWL_ERROR:
        this->current_log_lvl_ = "[ERROR]\t";
        break;
      case lwl_lvl::LWL_VERBOSE:
        this->current_log_lvl_ = "[VERBOSE]\t";
        break;
      default:
        break;
    }

    _t.get();
  }

  return *this;
}

lightweight_logger &lightweight_logger::operator<<(ostream_manipulator s) {
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF && this->level_log_setup_ <= this->level_current_) {
    std::unique_lock<std::mutex> l(this->m_access_message_);
    std::lock_guard<std::mutex> m(this->m_time_);
    this->logged_messages_.push_back(this->current_time_ + this->current_log_lvl_ + this->current_log_text_.str());
    this->current_log_text_.str("");
    cv_list_event_.notify_one();
  }
  return *this;
}

lightweight_logger &lightweight_logger::operator<<(const std::string s) {
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF && this->level_log_setup_ <= this->level_current_) {
    std::lock_guard<std::mutex> l(this->m_access_message_);
    this->current_log_text_ << s;
  }
  return *this;
}

lightweight_logger &lightweight_logger::operator<<(const int s) {
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF && this->level_log_setup_ <= this->level_current_) {
    std::lock_guard<std::mutex> l(this->m_access_message_);
    this->current_log_text_ << s;
  }
  return *this;
}

lightweight_logger &lightweight_logger::operator<<(const double s) {
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF && this->level_log_setup_ <= this->level_current_) {
    std::lock_guard<std::mutex> l(this->m_access_message_);
    this->current_log_text_ << s;
  }
  return *this;
}

lightweight_logger &lightweight_logger::operator<<(const float s) {
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF && this->level_log_setup_ <= this->level_current_) {
    std::lock_guard<std::mutex> l(this->m_access_message_);
    this->current_log_text_ << s;
  }
  return *this;
}

lightweight_logger &lightweight_logger::operator<<(const short s) {
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF && this->level_log_setup_ <= this->level_current_) {
    std::lock_guard<std::mutex> l(this->m_access_message_);
    this->current_log_text_ << s;
  }
  return *this;
}

lightweight_logger &lightweight_logger::operator<<(const char s) {
  if (this->level_log_setup_ != lwl_lvl::LWL_OFF && this->level_log_setup_ <= this->level_current_) {
    std::lock_guard<std::mutex> l(this->m_access_message_);
    this->current_log_text_ << s;
  }
  return *this;
}
