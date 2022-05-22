from datetime import datetime
import json
from cmath import inf
import psycopg2

class BaseBar:
    
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

    def store_bar(self, ):
        with psycopg2.connect("dbname=tiger user=postgres host=127.0.0.1") as conn:
            cur = conn.cursor()

            columns = ','.join(self._bar.keys())
            values = ','.join([ (str(v) if not isinstance(v, str) else '\''+v+'\'') for v in self._bar.values()])
            insert_sql_str = 'INSERT INTO bar_store ({0}) VALUES ({1})'.format(columns, values)
            # print(insert_sql_str)
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