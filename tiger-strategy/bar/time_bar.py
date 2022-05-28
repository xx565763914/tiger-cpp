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

        if self.instrument_id() != tick["instrument_id"]:
            print(self.instrument_id(), " add ",tick["instrument_id"]," tick error !")

        bar_date_time = dt.strptime(tick["trading_day"]+tick["update_time"],
                                    '%Y%m%d%H:%M:%S')
        bar_date_time = bar_date_time.replace(second=0, microsecond=0)

        if self.check_date_time(bar_date_time):
            self._ticks.append( tick )
            return True
        else:
            return False
