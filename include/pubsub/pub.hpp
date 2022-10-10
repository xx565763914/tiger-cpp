#pragma once

#include <nanomsg/nn.hpp>
#include <nanomsg/pubsub.h>
#include <memory>
#include <unistd.h>
#include <string>
#include "log.hpp"


// 该类废弃
class Publish {
    /**
        1. 单次使用
    **/
    public:
        Publish(std::string url) {
            this->url = url;
            sock.reset(new nn::socket(AF_SP, NN_PUB));
            // bind 需要
            eid = sock->bind(url.c_str());
            sleep(3);
        }

        ~Publish() {
            // 析构函数会自动释放socket
        }

        void send(std::string data) {
            const char *d = data.c_str();
            int sz_d = strlen(d) + 1;
            int bytes = sock->send(d, sz_d, 0);
        }

    private:
        std::string url;
        std::unique_ptr<nn::socket> sock;
        int eid;
};

