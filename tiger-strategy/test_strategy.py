import tiger_strategy_wrapper
import time
from bar import OneMinTimeBar, BarsManager

class Test(tiger_strategy_wrapper.StrategyTemplate):
    market_url: str
    strategy_name: str
    # bar : OneMinTimeBar
    bar_mng : BarsManager

    def __init__(self, market_url, trader_url, strategy_name):
        super().__init__(market_url, trader_url, strategy_name)
        self.market_url = market_url
        self.strategy_name
        self.bar = None
        self.bar_mng = BarsManager()

    def on_sub(self, data):
        # print(type(data))
        # print(data)
        # print(len(data))
        # self.target_pos('rb2010', 1)

        # if self.bar is None:
        #     self.bar = OneMinTimeBar(data)
        # elif not self.bar.addTick(data):
        #     print(self.bar.getBar())
        #     self.bar.store_bar()
        #     self.bar = None

        if not self.bar_mng.manageTicks(data):
            self.bar_mng.manageBar(OneMinTimeBar(data))

if __name__ == '__main__':
    market_url = 'tcp://127.0.0.1:5555'
    trader_url = 'tcp://127.0.0.1:8888'
    strategy_name = 'test_strategy'

    strategy = Test(market_url, trader_url, strategy_name)
    strategy.start()

    while True:
        time.sleep(1)
