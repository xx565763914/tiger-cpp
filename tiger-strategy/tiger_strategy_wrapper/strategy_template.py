from .sub_client import Sub

class StrategyTemplate(Sub):
    market_sub_url: str
    strategy_name: str
    market_sub: Sub

    def __init__(self, market_pub_url, strategy_name):
        self.market_sub_url = market_pub_url
        self.strategy_name = strategy_name
        
        self.market_sub = Sub(self.market_sub_url, self.strategy_name)
        self.market_sub.register_func(self.on_sub)
    
    def on_sub(self, data):
        '''
            行情订阅回调
        '''
        ...

    def start(self):
        self.market_sub.run()