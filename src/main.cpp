#include <unistd.h>
#include <iostream>
#include <string>
#include <ctp/ThostFtdcMdApi.h>
#include "ctp_market.hpp"
#include "log.hpp"
#include <nanomsg/nn.hpp>
#include <nanomsg/pubsub.h>
#include <time.h>
#include <pqxx/pqxx> 
#include "pubsub/pub.hpp"
#include "pubsub/sub.hpp"
#include "ctp_trade.hpp"
#include "tools/conv.hpp"
#include "symbol_holder.hpp"
#include "reqrep/server.hpp"
#include "queue/blocking_queue.hpp"
#include "pool/pg_pool.hpp"
#include "tools/string_util.hpp"
#include "dao/strategy_order_dao.hpp"
#include <junction/ConcurrentMap_Grampa.h>
#include <junction/ConcurrentMap_Linear.h>
#include <tbb/concurrent_hash_map.h>

char* date(void) {
        time_t now = time(&now);
        struct tm *info = localtime(&now);
        char *text = asctime(info);
        text[strlen(text)-1] = '\0'; // remove '\n'
        return (text);
}

void server(std::string url) {
    nn::socket s {AF_SP, NN_PUB};
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
    nn::socket s {AF_SP, NN_SUB};
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

class ReqServer1 : public RepServer {
    public:

        ReqServer1(std::string url):RepServer(url) {

        }

        std::string notify(const std::string &recvData) {
            LOG_INFO("target pos: {0}.", recvData);
            return std::string(recvData);
        }
};

// void threadfunc(std::shared_ptr<BlockingQueue<int>> q)
// {
//     sleep(2);
//     q->put(10);
//     q->put(5);
// }

void test_get_conn() {
    std::shared_ptr<pqxx::connection> conn = PGConnectionPool::getInstance()->getConn();
}

    // std::string order_id;
    // std::string direction;
    // std::string comb_offset;
    // std::string instrument_id;
    // double price;
    // int volume;
    // int traded;
    // std::string order_status;
    // std::string strategy_name;
    // long long create_time;
    // long long update_time;

struct Foo {
    std::string msg;
};

int main(int argc, char ** argv) {

    
    tbb::concurrent_hash_map<std::string, Foo> hashMap;
    Foo foo{"this is value."};
    tbb::concurrent_hash_map<std::string, Foo>::value_type hashMapValuePair("xxxx", foo);
    hashMap.insert(hashMapValuePair);
    Foo foo1{"xx"};
    tbb::concurrent_hash_map<std::string, Foo>::value_type hashMapValuePair3("xxxx", foo1);
    hashMap.insert(hashMapValuePair3);
    tbb::concurrent_hash_map<std::string, Foo>::value_type hashMapValuePair1("ab", foo);
    hashMap.insert(hashMapValuePair1);
    LOG_INFO("map size: {0}.", hashMap.size());

    tbb::concurrent_hash_map<std::string, Foo>::const_accessor hashAccessor;
    if(hashMap.find(hashAccessor,"xxxx")){
        std::cout << "find : " << std::endl;
        std::cout << "key : " << hashAccessor->first << std::endl;
        std::cout << "value : " << hashAccessor->second.msg << std::endl;
    }

    // myMap.exchange(14, foo);
    // myMap.assign(15, foo);
    // Foo* foo = myMap.get(14);
    // foo = myMap.exchange(14, new Foo);
    // delete foo;
    // foo = myMap.erase(14);
    // delete foo;

    // StrategyOrderDao::getOpenPosition("unknow_strategy");

    // StrategyOrder order;
    // order.order_id = "test_order_id";
    // order.direction = "long";
    // order.comb_offset = "open";
    // order.instrument_id = "rb2210";
    // order.price = 89.3;
    // order.volume = 2;
    // order.traded = 0;
    // order.order_status = "create";
    // order.strategy_name = "unknow_strategy",
    // order.create_time = 1123123;
    // order.update_time = 123424;

    // StrategyOrderDao::createOrder(order);

    // order.order_status = "update_status";
    // StrategyOrderDao::updateOrderStatus(order);

    // order.traded = 2;
    // StrategyOrderDao::updateTrade(order);

    // std::string a{"xxxx"};
    // LOG_INFO("{0}", string_format("%s.", a.c_str()));

    // std::shared_ptr<BlockingQueue<int>> q = std::shared_ptr<BlockingQueue<int>>(new BlockingQueue<int>());
    // std::thread t1(threadfunc, q);
    // q->take();
    // int val = q->take();
    // LOG_INFO("{0}", val);
    // t1.join();
    // sleep(3);

    // pqxx::connection c{"postgresql://postgres@localhost:5432/tiger"};
    //    try{
    //       pqxx::connection C("dbname=tiger user=postgres password= hostaddr=127.0.0.1 port=5432");
    //       if (C.is_open()) {
    //          std::cout << "Opened database successfully: " << C.dbname() << std::endl;
    //       } else {
    //          std::cout << "Can't open database" << std::endl;
    //          return 1;
    //       }
    //       C.disconnect ();
    //       std::cout<<"xxxxxxx"<<std::endl;
    //    }catch (const std::exception &e){
    //       std::cerr << e.what() << std::endl;
    //       return 1;
    //    }

    // ReqServer1 s("tcp://127.0.0.1:8888");
    // s.run();
    // while(1) {
    //     sleep(1);
    // }

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

    // if (strcmp(argv[1], "server") == 0) {
    //     // server("ipc:///tmp/pubsub.ipc");
    //     std::unique_ptr<Publish> pub(new Publish("tcp://127.0.0.1:5555"));
    //     for (;;) {
    //         pub->send("hello worldxxxxxxxx");
    //         sleep(1);
    //     }
    // }

    // if (strcmp(argv[1], "client") == 0) {
    //     // client("ipc:///tmp/pubsub.ipc", argv[2]);
    //     std::string name(argv[2]);
    //     Sub1 sub("tcp://127.0.0.1:5555", name);
    //     sub.run();
    // }

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