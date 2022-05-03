#pragma once

#include <map>
#include <string>

class Direction {
    public:
        static std::string LONG;
        static std::string SHORT;
};
std::string Direction::LONG = "long";
std::string Direction::SHORT = "short";


class CombOffset {
    public:
        static std::string OPEN;
        static std::string CLOSE;
};
std::string CombOffset::OPEN = "open";
std::string CombOffset::CLOSE = "close";

class OrderStatus {
    public:
        static std::string CREATE;
};
std::string OrderStatus::CREATE = "create";
