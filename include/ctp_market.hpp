#ifndef CTP_MARKET_H
#define CTP_MARKET_H
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unistd.h>
#include "ThostFtdcMdApi.h"
#include "log.hpp"

class CtpMarket : public CThostFtdcMdSpi {

    public:
        // 主动操作
        // 连接交易所
        void connect(std::string md_dir, std::string md_addr) {
            LOG_INFO("prepare to connect md md_dir {0}, md_addr {1}.", md_dir, md_addr);
            md_ptr = CThostFtdcMdApi::CreateFtdcMdApi(const_cast<char*>(md_dir.c_str()), true, true);
            md_ptr->RegisterSpi(this);
            md_ptr->RegisterFront(const_cast<char*>(md_addr.c_str()));
            md_ptr->Init();
        }

        // 行情登陆，不穿用户名密码也可以
        void login() {
            CThostFtdcReqUserLoginField loginReq = {0};
            while(md_ptr->ReqUserLogin(&loginReq, 1) != 0) {
                LOG_INFO("try login......");
                sleep(3);
            }
        }

        // 订阅行情
        void subscribe(std::vector<std::string> &contracts) {
            char **instruments = new char*[contracts.size()];
            for (int i = 0; i < contracts.size(); i++) {
                instruments[i] = const_cast<char*>(contracts[i].c_str());
            }
            while(md_ptr->SubscribeMarketData(instruments, 1) != 0) {
                LOG_INFO("try subscribe {0}.", contracts.size());
                sleep(3);
            }
            LOG_INFO("subscribe succeed.");
            delete instruments;
        }

        // 交易所回调

        //前置连接通知
        void OnFrontConnected() {
            // std::cout<<"行情前置连接成功，准备登陆..."<<std::endl;
            LOG_INFO("succeed to connect front addr, prepare to login.");
            login();
        }
        
        // 行情通知
        virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)=0;

        // 错误通知
        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

        }

    private:
        CThostFtdcMdApi* md_ptr;

};

#endif