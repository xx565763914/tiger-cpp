from datetime import datetime
import json
from cmath import inf
import psycopg2
from abc import ABCMeta, abstractmethod

class BaseBar:
    __metaclass__ = ABCMeta

    _ticks : list
    _bar : dict

    __bar_date_time_fmt = "%Y-%m-%d %H:%M:%S"

    def __init__(self, instrument_id:str, date_time:datetime) -> None:
        self._bar = {
            "instrument_id":instrument_id,
            "date_time":date_time.strftime(BaseBar.__bar_date_time_fmt),
            "open_price": inf, "close_price":inf, "high_price":-inf, "low_price":inf,
            "volume":inf, "turnover":inf, "open_interest":inf,
            "ask_price1":inf, "ask_volume1":inf,
            "bid_price1":inf, "bid_volume1":inf
        }

    @abstractmethod
    def addTick(self, tick:str) -> bool:
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

        for t in self._ticks:
            cur_price = t["last_price"]
            self._bar["high_price"] = max(cur_price, self._bar["high_price"])
            self._bar["low_price"] = min(cur_price, self._bar["low_price"])

        return self._bar

    def store_bar(self, ):
        with psycopg2.connect("dbname=tiger user=postgres host=127.0.0.1") as conn:
            cur = conn.cursor()

            columns = ','.join(self._bar.keys())
            values = ','.join([ (str(v) if not isinstance(v, str) else '\''+v+'\'') for v in self._bar.values()])
            insert_sql_str = 'INSERT INTO bar_store ({0}) VALUES ({1})'.format(columns, values)
            print(insert_sql_str)
            cur.execute(insert_sql_str)

    def instrument_id(self) -> str:
        return self._bar["instrument_id"]
        
    def date_time(self) -> str:
        return self._bar["date_time"]

    def check_date_time(self, other_date_time:datetime) -> bool:
        return other_date_time.strftime(BaseBar.__bar_date_time_fmt) == self._bar["date_time"]

    @staticmethod
    def _str_trans_to_json_decoder(tick:str) -> json.decoder:
        if tick[-1] == '\0':
            tick = tick[:-1]
        return json.loads(tick)