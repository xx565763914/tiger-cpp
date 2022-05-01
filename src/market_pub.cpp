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
#include "bus/trader_business.hpp"

class MarketPub : public CtpMarket {
    public:

        MarketPub(std::shared_ptr<Publish> pub) {
            this->pub = pub;
        }

        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
            Tick tick(*pDepthMarketData);
            json data = tick;
            pub->send(data.dump());
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

    // 初始化行情分发器配置
    std::string url = Config::instance()->getMarketPubUrl();
    std::string traderUrl = Config::instance()->getTraderUrl();

    // 初始化td
    std::shared_ptr<CtpTrade> ctpTrade = std::shared_ptr<CtpTrade>(new CtpTrade(userId, password, brokerId, investorId));
    ctpTrade->connect(td_dir, td_addr);
    while(!ctpTrade->isConnected()) {
        LOG_INFO("等待交易前置连接成功......");
        sleep(3);
    }

    // 初始化trader
    std::shared_ptr<TraderBusiness> tdBus = std::shared_ptr<TraderBusiness>(new TraderBusiness(traderUrl));
    tdBus->runAsync();

    //查询全市场合约行情
    ctpTrade->qryAllInstrument();
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
