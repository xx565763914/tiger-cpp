/**

    行情分发器，用于分发tick行情

**/

#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include "ctp_market.hpp"
#include "ctp_trade.hpp"
#include "log.hpp"
#include "pubsub/pub.hpp"
#include "config.hpp"
#include "model/market.hpp"

class MarketPub : public CtpMarket {
    public:

        MarketPub(std::shared_ptr<Publish> pub) {
            this->pub = pub;
        }

        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
            Tick tick(*pDepthMarketData);
            LOG_INFO("receive market data {0}", tick.last_price);
            std::string data = "receive market data: " + std::to_string(tick.last_price);
            pub->send(data);
        }

    private:
        std::shared_ptr<Publish> pub;
};

int main() {
    // 初始化行情获取配置
    std::string md_dir = Config::instance()->getMdDir();
    std::string md_addr = Config::instance()->getMdAddr();

    // 初始化交易配置
    std::string td_dir = Config::instance()->getTdDir();
    std::string td_addr = Config::instance()->getTdAddr();
    std::string userId = Config::instance()->getUserId();
    std::string password = Config::instance()->getPassword();
    std::string brokerId = Config::instance()->getBrokerId();
    std::string investorId = Config::instance()->getInvestorId();

    // 初始化行情分发器
    std::string url = Config::instance()->getMarketPubUrl();

    // 获取市场全部合约
    CtpTrade ctp_trade(userId, password, brokerId, investorId);
    ctp_trade.connect(td_dir, td_addr);
    while(!ctp_trade.isConnected()) {
        LOG_INFO("等待交易前置连接成功......");
        sleep(3);
    }
    ctp_trade.qryAllInstrument();
    while(!SymbolHolder::getInstance()->isReady()) {
        sleep(1);
    }
    LOG_INFO("all symbols length {0}.", SymbolHolder::getInstance()->getAllInstrument().size());

    // 准备要订阅的合约
    std::vector<std::string> contracts;
    std::vector<std::string> allContracts = SymbolHolder::getInstance()->getAllInstrument();
    for (int i = 0; i < allContracts.size(); i++) {
        contracts.push_back(allContracts[i]);
    }

    // // 初始化行情分发器
    std::shared_ptr<Publish> p(new Publish(url));

    // 初始化ctp行情获取
    std::shared_ptr<MarketPub> pub(new MarketPub(p));
    pub->connect(md_dir, md_addr);
    while(!pub->isConnected()) {
        LOG_INFO("等待行情前置连接成功......");
        sleep(3);
    }
    pub->subscribe(contracts);

    while(true) {
        sleep(1);
    }
}
