import json

class BaseBar:
    
    _ticks : list
    _bar : dict

    @staticmethod
    def _str_trans_to_json_decoder(tick:str) -> json.decoder:
        if tick[-1] == '\0':
            tick = tick[:-1]
        return json.loads(tick)