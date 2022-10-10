#pragma once

#include <nanomsg/nn.hpp>
#include <nanomsg/pubsub.h>
#include <memory>
#include <unistd.h>
#include <string>
#include "log.hpp"

    // nn::socket s = nn::socket(AF_SP, NN_SUB);
    // s.setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
    // s.connect(url.c_str());
    // for (;;) {
    //     char *buf = NULL;
    //     int bytes = s.recv(&buf, NN_MSG, 0);
    //     if (bytes < 0) {

    //     }
    //     LOG_INFO("client {0} received {1}.", name, buf);
    //     nn::freemsg(buf);
    // }
    
// 该类废弃
class Subscribe {
    public:
        Subscribe(std::string url, std::string name) {
            this->url = url;
            this->name = name;
            sock.reset(new nn::socket(AF_SP, NN_SUB));
            sock->setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
            sock->connect(url.c_str());
            sleep(3);
        }

        void run() {
            for (;;) {
                char *buf = NULL;
                int bytes = sock->recv(&buf, NN_MSG, 0);
                if (bytes < 0) {
                    // sub没有接收到数据则忽略
                    LOG_ERROR("client {} failed to recv data.", this->name);
                    continue;
                }
                notify(buf);
                nn::freemsg(buf);
            }
        }

        virtual void notify(const char *data)=0;

    private:
        std::unique_ptr<nn::socket> sock;
        std::string url;
        std::string name;

};
