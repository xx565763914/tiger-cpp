#pragma once

#include <nanomsg/nn.hpp>
#include <nanomsg/reqrep.h>
#include <unistd.h>
#include <string>

class RepServer {

    public:
        RepServer(std::string url) {
            this->url = url;
            sock.reset(new nn::socket(AF_SP, NN_REP));
            sock->bind(const_cast<char*>(url.c_str()));
            sleep(3);
            LOG_INFO("初始化RepServer成功");
        }

        void run() {
            char *buf = NULL;
            int bytes;
            for (;;) {
                // 接收数据
                if ((bytes = sock->recv(&buf, NN_MSG, 0)) < 0) {
                    LOG_ERROR("接受客户端请求失败.");
                    continue;  //接收失败，服务端不做任何处理，等待客户端重试
                }

                // 处理数据
                std::string rspData = notify(std::string(buf));

                // 返回数据
                int sz_d = strlen(rspData.c_str()) + 1;
                if ((bytes = sock->send(rspData.c_str(), sz_d, 0)) < 0) {
                    LOG_ERROR("发送数据到客户端失败 {0}.", rspData);
                    continue; // 发送失败，服务端不做任何处理，等待客户端重试
                }
                nn::freemsg(buf);
            }
        }

        virtual std::string notify(const std::string &recvData) = 0;
        
    private:
        std::string url;
        std::unique_ptr<nn::socket> sock;
};
