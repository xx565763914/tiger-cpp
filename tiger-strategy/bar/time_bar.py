from datetime import datetime as dt
from .base_bar import BaseBar

class OneMinTimeBar(BaseBar):
    def __init__(self, tick:str) -> None:
        tick = BaseBar._str_trans_to_json_decoder(tick)
        self._ticks = [ tick ]

        bar_date_time = dt.strptime(tick["trading_day"]+tick["update_time"],
                                    '%Y%m%d%H:%M:%S')
        bar_date_time = bar_date_time.replace(second=0, microsecond=0)

        super(OneMinTimeBar, self).__init__(instrument_id=tick["instrument_id"], date_time=bar_date_time)

    def addTick(self, tick:str) -> bool:
        tick = BaseBar._str_trans_to_json_decoder(tick)

        # print(self.instrument_id(), " add ",tick["instrument_id"]," tick")
        bar_date_time = dt.strptime(tick["trading_day"]+tick["update_time"],
                                    '%Y%m%d%H:%M:%S')
        bar_date_time = bar_date_time.replace(second=0, microsecond=0)

        if self.check_date_time(bar_date_time):
            self._ticks.append( tick )
            cur_price = tick["last_price"]
            self._bar["high_price"] = max(cur_price, self._bar["high_price"])
            self._bar["low_price"] = min(cur_price, self._bar["low_price"])
            return True
        else:
            return False       

    def getBar(self) -> dict:
        self._bar["open_price"] = self._ticks[0]["last_price"]
        self._bar["close_price"] = self._ticks[-1]["last_price"]

        self._bar["ask_price1"] = self._ticks[-1]["ask_price1"]
        self._bar["ask_volume1"] = self._ticks[-1]["ask_volume1"]
        self._bar["bid_price1"] = self._ticks[-1]["bid_price1"]
        self._bar["bid_volume1"] = self._ticks[-1]["bid_volume1"]

        self._bar["open_interest"] = self._ticks[-1]["open_interest"]
        self._bar["volume"] = self._ticks[-1]["volume"] - self._ticks[0]["volume"]
        self._bar["turnover"] = self._ticks[-1]["turnover"] - self._ticks[0]["turnover"]

        return self._bar