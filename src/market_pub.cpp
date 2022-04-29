/**

    行情分发器，用于分发tick行情

**/

#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include "ctp_market.hpp"
#include "log.hpp"
#include "pubsub/pub.hpp"

class MarketPub : public CtpMarket {
    public:

        MarketPub(std::shared_ptr<Publish> pub) {
            this->pub = pub;
        }

        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
            LOG_INFO("receive market data {0}", pDepthMarketData->LastPrice);

        }

    private:
        std::shared_ptr<Publish> pub;
};

int main() {
    // 初始化配置
    std::string md_dir = "./flow_md/";
    std::string md_addr = "tcp://180.168.146.187:10131";
    std::string url = "ipc:///tmp/pubsub.ipc";
    
    // 初始化行情分发器
    std::shared_ptr<Publish> p(nullptr);

    // 获取市场全部合约


    // 初始化ctp行情获取
    std::shared_ptr<MarketPub> pub(new MarketPub(p));
    pub->connect(md_dir, md_addr);
    std::vector<std::string> contracts = {"rb2210"};
    pub->subscribe(contracts);

    sleep(30);
}
