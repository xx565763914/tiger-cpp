from yamlConfig import yamlConfig
from redisSub import redisSub
from mysqlOps import mysqlOps
import json

def sub_redis_to_store_market_data(cfg_file : str):
    cfg = yamlConfig(cfg_file)
    market_data = cfg.get_cfg("market_data")
    sub_redis_chn = market_data["channel"]
    sub_redis_ip = market_data["ip"]
    sub_redis_port = market_data["port"]
    
    redis_suber = redisSub(ip=sub_redis_ip, port=sub_redis_port)
    redis_suber.sub_channel(sub_redis_chn)
    sub_rsp = redis_suber.get_msg()
    # print(sub_rsp)
    if sub_rsp['type'] != 'subscribe' \
    or str(sub_rsp['channel'], encoding='utf-8') != sub_redis_chn \
    or int(sub_rsp['data']) != 1:
        raise Exception("Redis subscribe failed !")

    mysql_cfg = cfg.get_cfg("store_md")
    mysql_opser = mysqlOps( host=mysql_cfg["ip"], port=int(mysql_cfg["port"]), db=mysql_cfg["database"],
                            user=mysql_cfg["user"], passwd=mysql_cfg["password"])
    mysql_opser.create_market_data_table(mysql_cfg["table"])

    while 1:
        msg = redis_suber.get_msg() # dict
        data_str = str(msg['data'], encoding='utf-8')
        # print(data_str)
        if not mysql_opser.update_tick(mysql_cfg["table"], data_str):
            print("Update tick \'{}\' to {}://{}@{}:{} failed !".format(data_str, 
                                                                        mysql_cfg["table"],
                                                                        mysql_cfg["user"],
                                                                        mysql_cfg["ip"],
                                                                        mysql_cfg["port"]
                                                                    ))

if __name__ == "__main__":
    sub_redis_to_store_market_data("/data/home/lervisnh/tiger-cpp/templates/configs/ctp.yaml")