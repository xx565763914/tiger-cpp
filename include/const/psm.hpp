#pragma once

#include <string>

// 不同服务的psm定义
class PSM {
    public:
        static std::string MARKET_PSM;
        static std::string TRADE_PSM;
};

std::string PSM::MARKET_PSM = "market";
std::string PSM::TRADE_PSM = "trade";
