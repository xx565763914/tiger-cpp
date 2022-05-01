import json
import time
from .sub_client import Sub
from .req_client import ReqClient

class StrategyTemplate(Sub):
    market_sub_url: str
    trader_url: str
    strategy_name: str
    market_sub: Sub

    def __init__(self, market_pub_url, trader_url, strategy_name):
        self.market_sub_url = market_pub_url
        self.trader_url = trader_url
        self.strategy_name = strategy_name
        
        self.market_sub = Sub(self.market_sub_url, self.strategy_name)
        self.market_sub.register_func(self.on_sub)

        self.trader_client = ReqClient(self.trader_url, self.strategy_name)

    
    def on_sub(self, data):
        '''
            行情订阅回调
        '''
        ...

    def target_pos(self, instrument_id, target_pos):
        adj_pos = dict(
            ts_sec = int(time.time()),
            strategy_name = self.strategy_name,
            instrument_id = instrument_id,
            target_pos = target_pos
        )
        target_pos_req = json.dumps(dict(
            event_type = "adj_pos",
            event_data = adj_pos
        ))
        # todo: 这里有坑，req函数可能抛出异常 从而导致策略进程失败
        self.trader_client.req(target_pos_req)

    def start(self):
        self.market_sub.run()
    