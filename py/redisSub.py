import redis
import queue
import threading

class redisSub(threading.Thread):
    def __init__(self, ip : str, port : str or int) -> None:
        if isinstance(port, str): port = int(port)
        
        self.__redis = redis.Redis( host = ip,
                                    port = port,
                                    password = None)
        self.__redis_sub = self.__redis.pubsub()
        # print("sub redis ip = {} port = {}".format(ip, port))
        self.__q = queue.Queue(maxsize = ( 1<<31 ))
        threading.Thread.__init__(self)
        self.setDaemon(True)

    def sub_channel(self, channel : str) -> None:
        ret = self.__redis_sub.subscribe(channel)
        # print(ret)
        # print("sub redis chn = {}".format(channel))

    def __run(self) -> None:
        # print("sub redis to listen")
        for msg in self.__redis_sub.listen():
            self.__q.put(msg)
        # print("__run finished")

    def run(self) -> None:
        self.__run()

    def get_msg(self) -> str:
        while True:
            try:
                msg = self.__q.get(timeout=0.25)
                return msg
            except queue.Empty:
                # print("no subscribed data")
                pass