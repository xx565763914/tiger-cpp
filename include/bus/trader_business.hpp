#pragma once

#include <memory>
#include <cmath>
#include <nlohmann/json.hpp>
#include "reqrep/server.hpp"
#include "log.hpp"
#include "ctp_trade.hpp"
#include "model/event.hpp"

using json = nlohmann::json;

/**
 * @brief 交易业务
 * 
 */
class TraderBusiness : public RepServer{

    public:
        TraderBusiness(std::string url):RepServer(url) {
            
        }

        std::string notify(const std::string &recvData) {
            json eventData = json::parse(recvData);
            std::string event_type = eventData["event_type"].get<std::string>();
            if(event_type == "adj_pos") {
                AdjPos adPos = eventData["event_data"].get<AdjPos>();
                adjPosition(adPos);
            }
            return "ok: " + recvData;
        }
        
    private:
        void adjPosition(AdjPos pos) {
            // 做简单调仓策略
            std::string strategy_name = pos.strategy_name;
            OpenPosition curStrategyPos = StrategyOrderDao::getOpenPosition(strategy_name);
            int targetPos = pos.target_pos;
            OpenPosition targetStrategyPos = {0};
            if (targetPos >= 0) {
                targetStrategyPos.long_position = targetPos;
                targetStrategyPos.short_position = 0;
            } else {
                targetStrategyPos.long_position = 0;
                targetStrategyPos.short_position = std::abs(targetPos);
            }

            int deltaLong = targetStrategyPos.long_position - curStrategyPos.long_position;
            int deltaShort = targetStrategyPos.short_position - curStrategyPos.short_position;

            std::string strategyName = strategy_name;
            std::string instrumentId = pos.instrument_id;
            // double price = SymbolTick::getInstance()->getTick(instrumentId);
            if (deltaLong > 0) {
                LOG_INFO("{0} {1} 开多 {2}", strategyName, instrumentId, deltaLong);
            } else {
                LOG_INFO("{0} {1} 平多 {2}", strategyName, instrumentId, std::abs(deltaLong));
            }

            if (deltaShort > 0) {
                LOG_INFO("{0} {1} 开空 {2}", strategyName, instrumentId, deltaShort);
            } else {
                LOG_INFO("{0} {1} 平空 {2}", strategyName, instrumentId, std::abs(deltaShort));
            }
        }

    private:
        std::shared_ptr<CtpTrade> ctpTrade;
};



