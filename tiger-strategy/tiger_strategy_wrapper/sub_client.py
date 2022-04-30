import nnpy
import time

class Sub:
    url: str
    strategy_name: str
    listen_func: list = []

    def __init__(self, url, strategy_name):
        self.url = url
        self.strategy_name = strategy_name
        self.listen_func = []

        self.sub_client = nnpy.Socket(nnpy.AF_SP, nnpy.SUB)
        # subscribe message to receive by prefix, default is '' and receive all messages
        self.sub_client.setsockopt(nnpy.SUB, nnpy.SUB_SUBSCRIBE, '')
        # 3 establish a sever to push message
        self.sub_client.connect(url)
        time.sleep(3)
    
    def register_func(self, func):
        self.listen_func.append(func)

    def run(self):
        while True:
            recv_data = self.sub_client.recv()
            if not recv_data:
                continue
            recv_data = recv_data.decode('UTF-8')
            for f in self.listen_func:
                f(recv_data)


    
