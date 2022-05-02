#pragma once 

#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include "ctp_market.hpp"
#include "ctp_trade.hpp"
#include "log.hpp"
#include "pubsub/pub.hpp"
#include "model/market.hpp"

/**
 * @brief 行情分发业务
 * 
 */
class MarketPub : public CtpMarket {
    public:
        MarketPub(std::string url) {
            this->url = url;
            this->pub = std::shared_ptr<Publish>(new Publish(url));
        }

        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
            Tick tick(*pDepthMarketData);
            json data = tick;
            pub->send(data.dump());
        }

    private:
        std::string url;
        std::shared_ptr<Publish> pub;
};

