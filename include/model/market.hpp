#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "tools/conv.hpp"

using json = nlohmann::json;

struct Tick {
    // 交易日
    std::string trading_day;
    // 交易所id
    std::string exchange_id;
    // 最新价格
    double last_price;
    // 上次结算价
    double pre_settlement_price;
    // 昨收盘价
    double pre_close_price;
	///昨持仓量
	double pre_open_interest;
    ///今开盘
	double open_price;
    ///最高价
	double highest_price;
    ///最低价
	double lowest_price;
    ///数量
	int	volume;
    ///成交金额
	double turnover;
    ///持仓量
	double open_interest;
    ///今收盘
	double close_price;
    ///本次结算价
	double settlement_price;
    ///涨停板价
	double upper_limit_price;
    ///跌停板价
	double lower_limit_price;
    ///最后修改时间
	std::string update_time;
    ///最后修改毫秒
	int	update_millisec;
    ///申买价一
	double bid_price1;
	///申买量一
	int bid_volume1;
	///申卖价一
	double ask_price1;
	///申卖量一
	int ask_volume1;
    ///合约代码
	std::string instrument_id;
    
    std::string exchange_inst_id;

    Tick(CThostFtdcDepthMarketDataField &d) {
        trading_day = Conv::GBKToUTF8(d.TradingDay);
        exchange_id = Conv::GBKToUTF8(d.ExchangeID);
        last_price = d.LastPrice;
        pre_settlement_price = d.PreSettlementPrice;
        pre_close_price = d.PreClosePrice;
        pre_open_interest = d.PreOpenInterest;
        open_price = d.OpenPrice;
        highest_price = d.HighestPrice;
        lowest_price = d.LowestPrice;
        volume = d.Volume;
        turnover = d.Turnover;
        open_interest = d.OpenInterest;
        close_price = d.ClosePrice;
        settlement_price = d.SettlementPrice;
        upper_limit_price = d.UpperLimitPrice;
        lower_limit_price = d.LowerLimitPrice;
        update_time = Conv::GBKToUTF8(d.UpdateTime);
        update_millisec = d.UpdateMillisec;
        bid_price1 = d.BidPrice1;
        bid_volume1 = d.BidVolume1;
        ask_price1 = d.AskPrice1;
        ask_volume1 = d.AskVolume1;
        instrument_id = Conv::GBKToUTF8(d.InstrumentID);
        exchange_inst_id = Conv::GBKToUTF8(d.ExchangeInstID);
    }
};

void to_json(json& j, const Tick& tick) {
    j = json{
        {"trading_day", tick.trading_day},
        {"exchange_id", tick.exchange_id},
        {"last_price", tick.last_price},
        {"pre_settlement_price", tick.pre_settlement_price},
        {"pre_close_price", tick.pre_close_price},
        {"pre_open_interest", tick.pre_open_interest},
        {"open_price", tick.open_price},
        {"highest_price", tick.highest_price},
        {"lowest_price", tick.lowest_price},
        {"volume", tick.volume},
        {"turnover", tick.turnover},
        {"open_interest", tick.open_interest},
        {"close_price", tick.close_price},
        {"settlement_price", tick.settlement_price},
        {"upper_limit_price", tick.upper_limit_price},
        {"lower_limit_price", tick.lower_limit_price},
        {"update_time", tick.update_time},
        {"update_millisec", tick.update_millisec},
        {"bid_price1", tick.bid_price1},
        {"bid_volume1", tick.bid_volume1},
        {"ask_price1", tick.ask_price1},
        {"ask_volume1", tick.ask_volume1},
        {"instrument_id", tick.instrument_id},
        {"exchange_inst_id", tick.exchange_inst_id}
    };
}