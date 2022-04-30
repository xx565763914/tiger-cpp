#pragma once

#include <boost/atomic.hpp>
#include <vector>
#include <memory>
#include "log.hpp"
#include "ThostFtdcTraderApi.h"

class SymbolHolder {
    public:
        bool isReady() {
            return ready;
        }

        void setReady() {
            ready = true;
            LOG_INFO("期货合约查询完毕..");
        }

        // 在这里不必纠结性能问题
        void addSymbol(CThostFtdcInstrumentField symbol) {
            LOG_INFO("查询到期货合约 {0}.", symbol.InstrumentID);
            symbols.push_back(symbol);
        }

        std::vector<std::string> getAllInstrument() {
            
            if (ready == false) {
                LOG_ERROR("期货合约尚未查询完毕，无法获取所有的期货品种");
                return {};
            }

            std::vector<std::string> result;
            for (int i = 0; i < symbols.size(); i++) {
                result.push_back(Conv::GBKToUTF8(symbols[i].InstrumentID));
            }
            return result;
        }

        static std::shared_ptr<SymbolHolder> getInstance() {
            return instance;
        }

    private:
        std::vector<CThostFtdcInstrumentField> symbols;
        boost::atomic<bool> ready = false;
        static std::shared_ptr<SymbolHolder> instance;
};

std::shared_ptr<SymbolHolder> SymbolHolder::instance = std::shared_ptr<SymbolHolder>(new SymbolHolder());
