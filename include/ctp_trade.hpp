#pragma once

#include <string>
#include <memory>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <boost/atomic.hpp>
#include <ctp/ThostFtdcTraderApi.h>
#include "log.hpp"
#include "tools/conv.hpp"
#include "symbol_holder.hpp"
#include "dao/strategy_order_dao.hpp"
#include "tools/time_util.hpp"
#include "const/order.hpp"

class CtpTrade : public CThostFtdcTraderSpi {
    public:
        
        CtpTrade(std::string userId, std::string password, std::string brokerId, std::string investorId) {
            this->userId = userId;
            this->password = password;
            this->brokerId = brokerId;
            this->investorId = investorId;
            orderIdInit = time((time_t*)NULL);
            requestId = orderIdInit;
            LOG_INFO("orderIdInit {0}.", orderIdInit);
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
            while(m_ptraderapi->ReqUserLogin(&t, requestId++) != 0) {
                LOG_INFO("try login td ......");
                sleep(1);
            }
        }

        void settlementinfoConfirm() {
            CThostFtdcSettlementInfoConfirmField t = {0};
            strcpy(t.BrokerID, brokerId.c_str());
            strcpy(t.InvestorID, investorId.c_str());
            while(m_ptraderapi->ReqSettlementInfoConfirm(&t, requestId++)!=0) {
                LOG_INFO("try settlement confirm ......");
                sleep(1);
            }
        }

        void qryAllInstrument() {
            CThostFtdcQryInstrumentField t = { 0 };
            while (m_ptraderapi->ReqQryInstrument(&t, requestId++) != 0) {
                LOG_INFO("try query all instrument ......");
                sleep(1);
            }
        }

        bool isConnected() {
            return connected;
        }

        // 产生一个order_id
        std::string generateOrderId() {
            return std::to_string(orderIdInit++);
        }

        // 报单相关 返回orderId
        std::string buy(std::string strategyName, std::string instrumentId, double price, int volume) {

            std::string exchangeId = SymbolHolder::getInstance()->getExchangeId(instrumentId);
            std::string orderId = generateOrderId();

            CThostFtdcInputOrderField ord = { 0 };
            strcpy(ord.BrokerID, const_cast<char*>(brokerId.c_str()));
            strcpy(ord.InvestorID, const_cast<char*>(investorId.c_str())); 
            strcpy(ord.ExchangeID, const_cast<char*>(exchangeId.c_str()));
            strcpy(ord.InstrumentID, const_cast<char*>(instrumentId.c_str()));
            strcpy(ord.UserID, const_cast<char*>(userId.c_str()));
            strcpy(ord.OrderRef, const_cast<char*>(orderId.c_str()));
            ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//限价
            ord.Direction = THOST_FTDC_D_Buy;//买
            ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;//开
            ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;//投机
            ord.LimitPrice = price;
            ord.VolumeTotalOriginal = volume;
            ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
            ord.VolumeCondition = THOST_FTDC_VC_AV;///任意数量
            ord.MinVolume = 1;
            ord.ContingentCondition = THOST_FTDC_CC_Immediately;
            ord.StopPrice = 0;
            ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
            ord.IsAutoSuspend = 0;

            // 记录订单信息
            long long curSecs = getCurSecs();
            StrategyOrder order;
            order.order_id = orderId;
            order.direction = Direction::LONG;
            order.comb_offset = CombOffset::OPEN;
            order.instrument_id = instrumentId;
            order.price = price;
            order.volume = volume;
            order.traded = 0;
            order.order_status = OrderStatus::CREATE;
            order.strategy_name = strategyName,
            order.create_time = curSecs;
            order.update_time = curSecs;
            StrategyOrderDao::createOrder(order);

            // 发送订单请求
            m_ptraderapi->ReqOrderInsert(&ord, requestId++);
            return orderId;
        }

        std::string sell(std::string strategyName, std::string instrumentId, double price, int volume) {
            std::string exchangeId = SymbolHolder::getInstance()->getExchangeId(instrumentId);
            std::string orderId = generateOrderId();

            CThostFtdcInputOrderField ord = { 0 };
            strcpy(ord.BrokerID, const_cast<char*>(brokerId.c_str()));
            strcpy(ord.InvestorID, const_cast<char*>(investorId.c_str())); 
            strcpy(ord.ExchangeID, const_cast<char*>(exchangeId.c_str()));
            strcpy(ord.InstrumentID, const_cast<char*>(instrumentId.c_str()));
            strcpy(ord.UserID, const_cast<char*>(userId.c_str()));
            strcpy(ord.OrderRef, const_cast<char*>(orderId.c_str()));
            ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//限价
            ord.Direction = THOST_FTDC_D_Sell;//卖
            ord.CombOffsetFlag[0] = THOST_FTDC_OF_Close;//平
            ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;//投机
            ord.LimitPrice = price;
            ord.VolumeTotalOriginal = volume;
            ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
            ord.VolumeCondition = THOST_FTDC_VC_AV;///任意数量
            ord.MinVolume = 1;
            ord.ContingentCondition = THOST_FTDC_CC_Immediately;
            ord.StopPrice = 0;
            ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
            ord.IsAutoSuspend = 0;

            // 记录订单信息
            long long curSecs = getCurSecs();
            StrategyOrder order;
            order.order_id = orderId;
            order.direction = Direction::SHORT;
            order.comb_offset = CombOffset::CLOSE;
            order.instrument_id = instrumentId;
            order.price = price;
            order.volume = volume;
            order.traded = 0;
            order.order_status = OrderStatus::CREATE;
            order.strategy_name = strategyName,
            order.create_time = curSecs;
            order.update_time = curSecs;
            StrategyOrderDao::createOrder(order);

            // 发送订单请求
            m_ptraderapi->ReqOrderInsert(&ord, requestId++);
            return orderId;
        }

        std::string sellShort(std::string strategyName, std::string instrumentId, double price, int volume) {
            std::string exchangeId = SymbolHolder::getInstance()->getExchangeId(instrumentId);
            std::string orderId = generateOrderId();

            CThostFtdcInputOrderField ord = { 0 };
            strcpy(ord.BrokerID, const_cast<char*>(brokerId.c_str()));
            strcpy(ord.InvestorID, const_cast<char*>(investorId.c_str())); 
            strcpy(ord.ExchangeID, const_cast<char*>(exchangeId.c_str()));
            strcpy(ord.InstrumentID, const_cast<char*>(instrumentId.c_str()));
            strcpy(ord.UserID, const_cast<char*>(userId.c_str()));
            strcpy(ord.OrderRef, const_cast<char*>(orderId.c_str()));
            ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//限价
            ord.Direction = THOST_FTDC_D_Sell;//卖
            ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;//开
            ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;//投机
            ord.LimitPrice = price;
            ord.VolumeTotalOriginal = volume;
            ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
            ord.VolumeCondition = THOST_FTDC_VC_AV;///任意数量
            ord.MinVolume = 1;
            ord.ContingentCondition = THOST_FTDC_CC_Immediately;
            ord.StopPrice = 0;
            ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
            ord.IsAutoSuspend = 0;

            // 记录订单信息
            long long curSecs = getCurSecs();
            StrategyOrder order;
            order.order_id = orderId;
            order.direction = Direction::SHORT;
            order.comb_offset = CombOffset::OPEN;
            order.instrument_id = instrumentId;
            order.price = price;
            order.volume = volume;
            order.traded = 0;
            order.order_status = OrderStatus::CREATE;
            order.strategy_name = strategyName,
            order.create_time = curSecs;
            order.update_time = curSecs;
            StrategyOrderDao::createOrder(order);

            // 发送订单请求
            m_ptraderapi->ReqOrderInsert(&ord, requestId++);
            return orderId;
        }

        std::string buyCover(std::string strategyName, std::string instrumentId, double price, int volume) {
            std::string exchangeId = SymbolHolder::getInstance()->getExchangeId(instrumentId);
            std::string orderId = generateOrderId();

            CThostFtdcInputOrderField ord = { 0 };
            strcpy(ord.BrokerID, const_cast<char*>(brokerId.c_str()));
            strcpy(ord.InvestorID, const_cast<char*>(investorId.c_str())); 
            strcpy(ord.ExchangeID, const_cast<char*>(exchangeId.c_str()));
            strcpy(ord.InstrumentID, const_cast<char*>(instrumentId.c_str()));
            strcpy(ord.UserID, const_cast<char*>(userId.c_str()));
            strcpy(ord.OrderRef, const_cast<char*>(orderId.c_str()));
            ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//限价
            ord.Direction = THOST_FTDC_D_Buy;//买
            ord.CombOffsetFlag[0] = THOST_FTDC_OF_Close;//平
            ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;//投机
            ord.LimitPrice = price;
            ord.VolumeTotalOriginal = volume;
            ord.TimeCondition = THOST_FTDC_TC_GFD;///当日有效
            ord.VolumeCondition = THOST_FTDC_VC_AV;///任意数量
            ord.MinVolume = 1;
            ord.ContingentCondition = THOST_FTDC_CC_Immediately;
            ord.StopPrice = 0;
            ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
            ord.IsAutoSuspend = 0;

            // 记录订单信息
            long long curSecs = getCurSecs();
            StrategyOrder order;
            order.order_id = orderId;
            order.direction = Direction::LONG;
            order.comb_offset = CombOffset::CLOSE;
            order.instrument_id = instrumentId;
            order.price = price;
            order.volume = volume;
            order.traded = 0;
            order.order_status = OrderStatus::CREATE;
            order.strategy_name = strategyName,
            order.create_time = curSecs;
            order.update_time = curSecs;
            StrategyOrderDao::createOrder(order);

            // 发送订单请求
            m_ptraderapi->ReqOrderInsert(&ord, requestId++);
            return orderId;
        }

        // 交易所回调

        void OnFrontConnected() {
            LOG_INFO("succeed to connect td addr.");
            this->login();
        }

        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            LOG_INFO("成功登陆交易前置.");
            frontId = pRspUserLogin->FrontID;
            sessionId = pRspUserLogin->SessionID;
            this->settlementinfoConfirm();
        }

        void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            LOG_INFO("成功进行价格结算.");
            connected.exchange(true);
        }

        void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            SymbolHolder::getInstance()->addSymbol(*pInstrument);
            if (bIsLast == true) {
                SymbolHolder::getInstance()->setReady();
            }
        }

        // 订单回报
        void OnRtnOrder(CThostFtdcOrderField *pOrder) {
            // 更新订单状态
            long long curSecs = getCurSecs();
            StrategyOrder order;
            order.order_id = std::string(pOrder->OrderRef);
            order.order_status = std::string(1, pOrder->OrderStatus);
            order.update_time = curSecs;
            StrategyOrderDao::updateOrderStatus(order);
        }

        // 理论上还应该有错误回报，不过当前更新了成交回报就可以满足诉求，后期可以加其他回报
        void OnRtnTrade(CThostFtdcTradeField *pTrade) {
            // 更新成交量
            long long curSecs = getCurSecs();
            StrategyOrder order;
            order.order_id = std::string(pTrade->OrderRef);
            order.traded = pTrade->Volume;
            order.update_time = curSecs;
            StrategyOrderDao::updateTrade(order);
        }

        // 订单相关回报
        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            LOG_ERROR("交易前置报错 {0}.", pRspInfo->ErrorMsg);
        }

    private:
        CThostFtdcTraderApi *m_ptraderapi;
        std::string userId;
        std::string password;
        std::string brokerId;
        std::string investorId;
        boost::atomic<bool> connected {false};
        // requestId
        unsigned int requestId;
        // 记录frontId和sessionId下单使用
        int frontId;
        int sessionId;
        // 获取当前时间作为订单id
        long long orderIdInit;
};