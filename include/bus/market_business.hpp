#pragma once

#include "ctp_market.hpp"
#include "ctp_trade.hpp"
#include "log.hpp"
#include "model/market.hpp"
#include "pubsub/redis_pub.hpp"
#include "symbol_tick_holder.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

/**
 * @brief 行情分发业务
 *
 */
class MarketPub : public CtpMarket {
public:
  // MarketPub(std::string url) {
  //     this->url = url;
  //     this->pub = std::shared_ptr<Publish>(new Publish(url));
  // }
  MarketPub(std::string channel, std::string ip, std::int32_t port,
            std::string password = "")
      : pub(new redisPublish(ip, port, password)), pub_channel(channel) {}

  void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
    Tick tick(*pDepthMarketData);
    json data = tick;
    SymbolTick::getInstance()->insertTick(tick);
    pub->send(pub_channel, data.dump());
  }

private:
  // std::string url;
  std::string pub_channel;
  std::shared_ptr<redisPublish> pub;
};
