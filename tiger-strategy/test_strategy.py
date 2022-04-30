import tiger_strategy_wrapper
import time

class Test(tiger_strategy_wrapper.StrategyTemplate):
    market_url: str
    strategy_name: str

    def __init__(self, market_url, strategy_name):
        super().__init__(market_url, strategy_name)
        self.market_url = market_url
        self.strategy_name

    def on_sub(self, data):
        print(data)
    

if __name__ == '__main__':
    url = 'tcp://127.0.0.1:5555'
    strategy_name = 'test_strategy'

    strategy = Test(url, strategy_name)
    strategy.start()

    while True:
        time.sleep(1)
