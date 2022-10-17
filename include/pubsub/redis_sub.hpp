#pragma once

#include "concurrentqueue/blockingconcurrentqueue.h"
#include "log.hpp"
#include "redis_base.hpp"
#include "sw/redis++/subscriber.h"
#include <algorithm>
#include <atomic>
#include <memory>
#include <string>
#include <vector>

class Subscribe : public redis_base {
private:
  typedef moodycamel::BlockingConcurrentQueue<std::string>
      messageQueue; // blocking version
  std::unique_ptr<sw::redis::Subscriber> _sub{};
  std::map<std::string, std::unique_ptr<messageQueue>> _chn_msgq;
  std::atomic_bool _is_running;

public:
  Subscribe(std::string ip, std::int32_t port, std::string password = "")
      : redis_base(ip, port, password), _is_running(false) {
    connect();
    std::unique_ptr<sw::redis::Subscriber> new_sub{
        new sw::redis::Subscriber(instance()->subscriber())};
    _sub.swap(new_sub);
  };

  void add_channel(std::string channel) {
    _sub->subscribe(channel);
    _chn_msgq[channel] = std::unique_ptr<messageQueue>(new messageQueue);
  };

  void run() {
    _sub->on_message([this](std::string channel, std::string msg) {
      auto &msg_q = this->_chn_msgq[channel];
      if (msg_q) {
        msg_q->enqueue(msg);
      } else {
        // error log
      }
    });

    if (_is_running.exchange(true)) {
      LOG_ERROR("Subscribe has already running");
      return;
    }

    while (_is_running) {
      _sub->consume();
    }
  };

  void run(std::vector<std::string> channels) {
    for (auto &chn : channels) {
      add_channel(chn);
    }
    run();
  };

  void stop() { _is_running.exchange(false); };

  std::string get_channel_message(std::string channel) {
    auto &msg_q = _chn_msgq[channel];
    std::string ret;
    msg_q->wait_dequeue(ret);
    return {std::move(ret)};
  };
};
