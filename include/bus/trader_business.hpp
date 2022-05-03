#pragma once

#include <memory>
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
                LOG_INFO("{0} {1} {2} {3}.", adPos.ts_sec, adPos.strategy_name,
                    adPos.instrument_id, adPos.target_pos);
            }
            return "ok: " + recvData;
        }
        
    private:
        std::shared_ptr<CtpTrade> ctpTrade;
};



