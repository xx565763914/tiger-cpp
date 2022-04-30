#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <unistd.h>
#include <boost/atomic.hpp>
#include "ThostFtdcTraderApi.h"
#include "log.hpp"
#include "tools/conv.hpp"
#include "symbol_holder.hpp"

class CtpTrade : public CThostFtdcTraderSpi {
    public:
        
        CtpTrade(std::string userId, std::string password, std::string brokerId, std::string investorId) {
            this->userId = userId;
            this->password = password;
            this->brokerId = brokerId;
            this->investorId = investorId;
        }

        ~CtpTrade() {
            release();
        }

        void connect(std::string md_dir, std::string td_addr) {
            m_ptraderapi = CThostFtdcTraderApi::CreateFtdcTraderApi(const_cast<char*>(md_dir.c_str()));
            m_ptraderapi->RegisterSpi(this);
            m_ptraderapi->SubscribePublicTopic(THOST_TERT_QUICK);
            m_ptraderapi->SubscribePrivateTopic(THOST_TERT_QUICK);
            m_ptraderapi->RegisterFront(const_cast<char*>(td_addr.c_str()));
            m_ptraderapi->Init();
            LOG_INFO("trader api version {0}.", m_ptraderapi->GetApiVersion());
        }

        void release() {
            m_ptraderapi->Release();
        }

        void login() {
            CThostFtdcReqUserLoginField t = {0};
            strcpy(t.BrokerID, brokerId.c_str());
            strcpy(t.UserID, userId.c_str());
            strcpy(t.Password, password.c_str());
            while(m_ptraderapi->ReqUserLogin(&t, 1) != 0) {
                LOG_INFO("try login td ......");
                sleep(1);
            }
        }

        void settlementinfoConfirm() {
            CThostFtdcSettlementInfoConfirmField t = {0};
            strcpy(t.BrokerID, brokerId.c_str());
            strcpy(t.InvestorID, investorId.c_str());
            while(m_ptraderapi->ReqSettlementInfoConfirm(&t, 2)!=0) {
                LOG_INFO("try settlement confirm ......");
                sleep(1);
            }
        }

        void qryAllInstrument() {
            CThostFtdcQryInstrumentField t = { 0 };
            while (m_ptraderapi->ReqQryInstrument(&t, 4) != 0) {
                LOG_INFO("try query all instrument ......");
                sleep(1);
            }
        }

        bool isConnected() {
            return connected;
        }

        // 交易所回调

        void OnFrontConnected() {
            LOG_INFO("succeed to connect td addr.");
            this->login();
        }

        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            LOG_INFO("succeed to login td addr.");
            this->settlementinfoConfirm();
        }

        void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            LOG_INFO("succeed to confirm settlement.");
            connected = true;
        }

        void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            SymbolHolder::getInstance()->addSymbol(*pInstrument);
            if (bIsLast == true) {
                SymbolHolder::getInstance()->setReady();
            }
        }

        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            LOG_ERROR("交易前置报错 {0}.", pRspInfo->ErrorMsg);
        };

    private:
        CThostFtdcTraderApi *m_ptraderapi;
        std::string userId;
        std::string password;
        std::string brokerId;
        std::string investorId;
        boost::atomic<bool> connected = false;
};