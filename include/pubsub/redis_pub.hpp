#pragma once

#include "redis_base.hpp"
#include <map>
#include <string>

class Publish : public redis_base {
public:
  Publish(std::string ip, std::int32_t port, std::string password = "")
      : redis_base(ip, port, password) {
    connect();
  };

  void send(std::string channel, std::string message) {
    instance()->publish(channel, message);
  };

  void send(std::string channel,
            std::map<std::string, std::string> map_message) {
    std::string msg{""};
    for (auto &iter : map_message) {
      msg += iter.first;
      msg += ":";
      msg += iter.second;
      msg += ";";
    }
    send(channel, msg);
  };
};
