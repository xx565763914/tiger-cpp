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

class redisSubscribe : public redis_base {
private:
  typedef moodycamel::BlockingConcurrentQueue<std::string>
      messageQueue; // blocking version
  std::unique_ptr<sw::redis::Subscriber> _sub{};
  std::unique_ptr<messageQueue> _msgq;
  std::atomic_bool _is_running;

public:
  redisSubscribe(std::string ip, std::int32_t port, std::string password = "")
      : redis_base(ip, port, password), _is_running(false),
        _msgq(new messageQueue) {
    connect();
    std::unique_ptr<sw::redis::Subscriber> new_sub{
        new sw::redis::Subscriber(instance()->subscriber())};
    _sub.swap(new_sub);
  };

  inline void add_channel(std::string channel) { _sub->subscribe(channel); };

  void run() {
    _sub->on_message([this](std::string channel, std::string msg) {
      LOG_TRACE("channel = ", channel.data(), ", message = ", msg.data());
      auto &msg_q = this->_msgq;
      msg_q->enqueue(msg);
    });

    if (_is_running.exchange(true)) {
      LOG_ERROR("redisSubscribe has already running");
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

  void stop() {
    _is_running.exchange(false);
    LOG_WARN("left message num = ", _msgq->size_approx());
  };

  std::string get_channel_message() {
    std::string ret;
    _msgq->wait_dequeue(ret);
    return {std::move(ret)};
  };
};
