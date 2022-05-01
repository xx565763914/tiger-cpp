#pragma once 

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct AdjPos {
    // 调仓指令
    long long ts_sec;
    std::string strategy_name;
    std::string instrument_id;
    double target_pos;
};

// 生成AdjPos对象
void from_json(const json& j, AdjPos& p) {
    j.at("ts_sec").get_to(p.ts_sec);
    j.at("strategy_name").get_to(p.strategy_name);
    j.at("instrument_id").get_to(p.instrument_id);
    j.at("target_pos").get_to(p.target_pos);
}
