import tiger_strategy_wrapper
import time

class Test(tiger_strategy_wrapper.StrategyTemplate):
    market_url: str
    strategy_name: str

    def __init__(self, market_url, trader_url, strategy_name):
        super().__init__(market_url, trader_url, strategy_name)
        self.market_url = market_url
        self.strategy_name

    def on_sub(self, data):
        print(data)
        self.target_pos('rb2010', 1)
    

if __name__ == '__main__':
    market_url = 'tcp://127.0.0.1:5555'
    trader_url = 'tcp://127.0.0.1:8888'
    strategy_name = 'test_strategy'

    strategy = Test(market_url, trader_url, strategy_name)
    strategy.start()

    while True:
        time.sleep(1)
