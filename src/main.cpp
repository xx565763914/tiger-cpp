#include <unistd.h>
#include <iostream>
#include <string>
#include "ThostFtdcMdApi.h"
#include "ctp_market.hpp"
#include "log.hpp"
#include <nanomsg/nn.hpp>
#include <nanomsg/pubsub.h>
#include <time.h>
#include "pubsub/pub.hpp"
#include "pubsub/sub.hpp"
#include "ctp_trade.hpp"
#include "tools/conv.hpp"
#include "symbol_holder.hpp"

char* date(void) {
        time_t now = time(&now);
        struct tm *info = localtime(&now);
        char *text = asctime(info);
        text[strlen(text)-1] = '\0'; // remove '\n'
        return (text);
}

void server(std::string url) {
    nn::socket s = nn::socket(AF_SP, NN_PUB);
    int eid = s.bind(url.c_str());
    for (int i = 0; i < 50; i++) {
        char *d = date();
        int sz_d = strlen(d) + 1;
        LOG_INFO("server: publish date {0}", d);
        int bytes = s.send(d, sz_d, 0);
        sleep(1);
    }
}

void client(std::string url, std::string name) {
    nn::socket s = nn::socket(AF_SP, NN_SUB);
    s.setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
    s.connect(url.c_str());
    for (;;) {
        char *buf = NULL;
        int bytes = s.recv(&buf, NN_MSG, 0);
        if (bytes < 0) {

        }
        LOG_INFO("client {0} received {1}.", name, buf);
        nn::freemsg(buf);
    }

}

class Sub1 : public Subscribe {
    public:

        Sub1(std::string url, std::string name):Subscribe(url, name) {

        }

        void notify(const char *data) {
            LOG_INFO("receive data {0}.", data);
        }
};

int main(int argc, char ** argv) {

    // 配置
    // std::string md_dir = "./flow_md/";
    // std::string md_addr = "tcp://180.168.146.187:10130";
    // std::string userId = "113458";
    // std::string password = "XX1024xx!@#$";
    // std::string brokerId = "9999";
    // std::string investorId = "113458";

    // CtpTrade ctp_trade (userId, password, brokerId, investorId);
    // ctp_trade.connect(md_dir, md_addr);
    // ctp_trade.qryAllInstrument();

    // while(!SymbolHolder::getInstance()->isReady()) {
    //     sleep(1);
    // }
    // sleep(3);

    if (strcmp(argv[1], "server") == 0) {
        // server("ipc:///tmp/pubsub.ipc");
        std::unique_ptr<Publish> pub(new Publish("tcp://127.0.0.1:5555"));
        for (int i = 0; i < 30; i++) {
            pub->send("hello worldxxxxxxxx");
            sleep(1);
        }
    }

    if (strcmp(argv[1], "client") == 0) {
        // client("ipc:///tmp/pubsub.ipc", argv[2]);
        std::string name(argv[2]);
        Sub1 sub("tcp://127.0.0.1:5555", name);
        sub.run();
    }

    // for (int i = 0; i < argc; i++) {
    //     std::cout<<argv[i]<<std::endl;
    // }
    // std::string md_dir = "./flow_md/";
    // std::string md_addr = "tcp://180.168.146.187:10131";
    // CtpMarket *ctp_market = new CtpMarket;
    // ctp_market->connect(md_dir, md_addr);
    // std::vector<std::string> contracts = {"rb2210"};
    // ctp_market->subscribe(contracts);
    // sleep(30);
}