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

class MarketPub : public CtpMarket {
    public:

        MarketPub(std::shared_ptr<Publish> pub) {
            this->pub = pub;
        }

        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
            LOG_INFO("receive market data {0}", pDepthMarketData->LastPrice);
            std::string data = "receive market data: " + std::to_string(pDepthMarketData->LastPrice);
            pub->send(data);
        }

    private:
        std::shared_ptr<Publish> pub;
};

int main() {
    // 初始化行情获取配置
    std::string md_dir = "./flow_md/";
    std::string md_addr = "tcp://180.168.146.187:10131";
    std::string url = "ipc:///tmp/pubsub.ipc";

    // 初始化交易配置
    std::string td_dir = "./flow_md/";
    std::string td_addr = "tcp://180.168.146.187:10130";
    std::string userId = "113458";
    std::string password = "XX1024xx!@#$";
    std::string brokerId = "9999";
    std::string investorId = "113458";

    // 获取市场全部合约
    CtpTrade ctp_trade(userId, password, brokerId, investorId);
    ctp_trade.connect(td_dir, td_addr);
    ctp_trade.qryAllInstrument();
    while(!SymbolHolder::getInstance()->isReady()) {
        sleep(1);
    }

    LOG_INFO("all symbols length {0}.", SymbolHolder::getInstance()->getAllInstrument().size());

    // 准备要订阅的合约
    std::vector<std::string> contracts;
    std::vector<std::string> allContracts = SymbolHolder::getInstance()->getAllInstrument();
    for (int i = 0; i < allContracts.size(); i++) {
        LOG_INFO("prepare to subscribe {0}.", allContracts[i]);
        contracts.push_back(allContracts[i]);
    }

    // // 初始化行情分发器
    std::shared_ptr<Publish> p(new Publish(url));

    // 初始化ctp行情获取
    std::shared_ptr<MarketPub> pub(new MarketPub(p));
    pub->connect(md_dir, md_addr);
    pub->subscribe(contracts);

    sleep(30);
}
