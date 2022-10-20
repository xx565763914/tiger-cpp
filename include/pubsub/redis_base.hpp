#pragma once

#include "log.hpp"
#include "sw/redis++/redis++.h"
#include "sw/redis++/redis.h"
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>

using namespace sw::redis;

class redis_base {
private:
  std::string _ip;
  std::int32_t _port;
  std::string _passwd;

  std::int32_t _pool_size = 3;
  std::shared_ptr<Redis> _conn_cxt{};

public:
  redis_base(std::string ip, std::int32_t port, std::string password = "")
      : _ip(ip), _port(port), _passwd(password){};
  // virtual ~redis_base(){};

  void connect() {
    ConnectionOptions connection_options;
    connection_options.host = _ip;
    connection_options.port = _port;
    connection_options.password = _passwd;

    ConnectionPoolOptions pool_options;
    pool_options.size = _pool_size;

    _conn_cxt =
        std::shared_ptr<Redis>(new Redis(connection_options, pool_options));
  };

  std::shared_ptr<Redis> instance() { return _conn_cxt; };
};