from .base_bar import BaseBar

class BarsManager:
    __maps:dict

    def __init__(self):
        self.__maps = {}

    def manageBar(self, bar:BaseBar) -> None:
        self.__maps[bar.instrument_id()] = bar

    def manageTicks(self, tick:str) -> bool:
        instrument_id = BaseBar._str_trans_to_json_decoder(tick)["instrument_id"]

        if instrument_id in self.__maps:
            if self.__maps[instrument_id].addTick(tick):
                return True
            else:
                self.__maps[instrument_id].getBar()
                self.__maps[instrument_id].store_bar()

        return False