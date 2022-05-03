#pragma once

#include <boost/atomic.hpp>
#include <vector>
#include <memory>
#include <regex>
#include <map>
#include "log.hpp"
#include <ctp/ThostFtdcTraderApi.h>

class SymbolHolder {
    public:

        SymbolHolder() {
            contractReg = std::regex("[a-zA-Z]+[0-9]+$");
        }

        bool isReady() {
            return ready;
        }

        void setReady() {
            ready.exchange(true);
            LOG_INFO("期货合约查询完毕..");
        }

        // 在这里不必纠结性能问题
        void addSymbol(CThostFtdcInstrumentField symbol) {
            std::string contract = Conv::GBKToUTF8(symbol.InstrumentID);
            bool isMatch = std::regex_match(contract, contractReg);
            LOG_INFO("查询到期货合约 {0} {1} {2}.", symbol.InstrumentID, isMatch, symbol.ExchangeID);
            symbolInfoMap[contract] = symbol;
            symbols.push_back(symbol);
        }

        std::vector<std::string> getAllInstrument() {
            
            if (ready == false) {
                LOG_ERROR("期货合约尚未查询完毕，无法获取所有的期货品种");
                return {};
            }

            std::vector<std::string> result;
            for (int i = 0; i < symbols.size(); i++) {
                std::string contract = Conv::GBKToUTF8(symbols[i].InstrumentID);
                if (!std::regex_match(contract, contractReg)) {
                    continue;
                }
                result.push_back(contract);
            }
            return result;
        }

        std::string getExchangeId(std::string instrumentId) {
            if (symbolInfoMap.find(instrumentId) != symbolInfoMap.end()) {
                return Conv::GBKToUTF8(symbolInfoMap[instrumentId].ExchangeID);
            }
            return "";
        }

        static std::shared_ptr<SymbolHolder> getInstance() {
            return instance;
        }

    private:
        std::vector<CThostFtdcInstrumentField> symbols;
        boost::atomic<bool> ready {false};
        std::regex contractReg;
        static std::shared_ptr<SymbolHolder> instance;
        std::map<std::string, CThostFtdcInstrumentField> symbolInfoMap;
};

std::shared_ptr<SymbolHolder> SymbolHolder::instance = std::shared_ptr<SymbolHolder>(new SymbolHolder());
