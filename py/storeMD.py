from yamlConfig import yamlConfig
from redisSub import redisSub
import json

def sub_redis_to_store_market_data(cfg_file : str):
    cfg = yamlConfig(cfg_file)
    market_data = cfg.get_cfg("market_data")
    sub_redis_chn = market_data["channel"]
    sub_redis_ip = market_data["ip"]
    sub_redis_port = market_data["port"]
    
    redis_suber = redisSub(ip=sub_redis_ip, port=sub_redis_port)
    redis_suber.sub_channel(sub_redis_chn)
    redis_suber.start()

    while 1:
        data_str = str(redis_suber.get_msg()['data'])
        data_dict = json.loads(data_str)
        print( data_dict )

if __name__ == "__main__":
    sub_redis_to_store_market_data("/data/home/lervisnh/tiger-cpp/templates/configs/ctp.yaml")