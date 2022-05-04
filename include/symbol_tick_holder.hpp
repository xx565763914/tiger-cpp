#pragma once

#include <memory>
#include <tbb/concurrent_hash_map.h>
#include "model/market.hpp"
#include <optional>

class SymbolTick {

    public:
        
        void insertTick(Tick tick) {
            // 此处应该添加过滤规则

            std::string instrumentId = tick.instrument_id;
            tbb::concurrent_hash_map<std::string, Tick>::value_type mPair(instrumentId, tick);
            tickMap.insert(mPair);
        }

        std::optional<Tick> getTick(std::string instrumentId) {
            tbb::concurrent_hash_map<std::string, Tick>::const_accessor has;
            if(tickMap.find(has, instrumentId)){
                return has->second;
            }
            return std::nullopt;
        }

        static std::shared_ptr<SymbolTick> getInstance() {
            return instance;
        }
    private:
        static std::shared_ptr<SymbolTick> instance;
        tbb::concurrent_hash_map<std::string, Tick> tickMap;
};

std::shared_ptr<SymbolTick> SymbolTick::instance = std::make_shared<SymbolTick>();
