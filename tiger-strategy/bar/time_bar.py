from cmath import inf
from datetime import datetime as dt
from .base_bar import BaseBar

class OneMinTimeBar(BaseBar):
    def __init__(self, tick:str) -> None:
        tick = BaseBar._str_trans_to_json_decoder(tick)
        self._ticks = [ tick ]

        bar_date_time = dt.strptime(tick["trading_day"]+tick["update_time"],
                                    '%Y%m%d%H:%M:%S')
        bar_date_time = bar_date_time.replace(second=0, microsecond=0)

        self._bar = {
            "instrument_id":tick["instrument_id"],
            # "trading_day":tick["trading_day"],
            "date_time":bar_date_time.strftime("%Y-%m-%d %H:%M"), # 1min
            "open": inf, "close":inf, "high":-inf, "low":inf,
            "volume":inf, "turnover":inf, "open_interest":inf,
            "ask_price1":inf, "ask_volume1":inf,
            "bid_price1":inf, "bid_volume1":inf
        }

        self._bar["open"] = tick["last_price"]

    def addTick(self, tick:str) -> bool:
        tick = BaseBar._str_trans_to_json_decoder(tick)

        bar_date_time = dt.strptime(tick["trading_day"]+tick["update_time"],
                                    '%Y%m%d%H:%M:%S')
        bar_date_time = bar_date_time.replace(second=0, microsecond=0)

        if bar_date_time.strftime("%Y-%m-%d %H:%M") == self._bar["date_time"]:
            self._ticks.append( tick )
            cur_price = tick["last_price"]
            self._bar["high"] = max(cur_price, self._bar["high"])
            self._bar["low"] = min(cur_price, self._bar["low"])
            return True
        else:
            return False       

    def getBar(self) -> dict:
        self._bar["close"] = self._ticks[-1]["last_price"]
        self._bar["open_interest"] = self._ticks[-1]["open_interest"]
        self._bar["ask_price1"] = self._ticks[-1]["ask_price1"]
        self._bar["ask_volume1"] = self._ticks[-1]["ask_volume1"]
        self._bar["bid_price1"] = self._ticks[-1]["bid_price1"]
        self._bar["bid_volume1"] = self._ticks[-1]["bid_volume1"]

        self._bar["volume"] = self._ticks[-1]["volume"] - self._ticks[0]["volume"]
        self._bar["turnover"] = self._ticks[-1]["turnover"] - self._ticks[0]["turnover"]

        return self._bar