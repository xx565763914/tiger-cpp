import nnpy
import time

class ReqClient:
    RETRY_TIME = 3 # 请求失败重试次数
    url: str
    client_name: str
    sock: nnpy.Socket

    def __init__(self, url, name):
        self.url = url
        self.client_name = name
        # todo: 这里可能有坑，需要设置recv连接超时时间
        self.sock = nnpy.Socket(nnpy.AF_SP, nnpy.REQ)
        self.sock.connect(url)
        time.sleep(1)

    def req(self, req_str: str):
        '''
            请求server
        '''
        for _ in range(ReqClient.RETRY_TIME):
            try:
                send_data = bytes(req_str, encoding='utf-8')
                self.sock.send(send_data)
                recv_data = self.sock.recv()
                return recv_data.decode('UTF-8')
            except Exception as e:
                print("error occurred while receive data.", e)
                # 恢复重新连接server
                
        raise Exception("请求数据的时候发生错误。")
                
if __name__ == '__main__':

    r = ReqClient(r'tcp://127.0.0.1:8888', 'test_client')
    for _ in range(100):
        resp = r.req("hello server")
        print(resp)
        time.sleep(1)

    
