import pymysql
import datetime
import json
from dbutils.pooled_db import PooledDB

class mysqlClient(object):
    __pool = None

    def __init__(self, 
                host:str, 
                port:int,
                db:str,
                user:str,
                passwd:str,
                mincached=10, 
                maxcached=20, 
                maxshared=10, 
                maxconnections=200, 
                blocking=True,
                maxusage=None,
                setsession=None, 
                reset=True,
                charset='utf8'
      ):
        """
        :param mincached:连接池中空闲连接的初始数量
        :param maxcached:连接池中空闲连接的最大数量
        :param maxshared:共享连接的最大数量
        :param maxconnections:创建连接池的最大数量
        :param blocking:超过最大连接数量时候的表现，为True等待连接数量下降，为false直接报错处理
        :param maxusage:单个连接的最大重复使用次数
        :param setsession:optional list of SQL commands that may serve to prepare
            the session, e.g. ["set datestyle to ...", "set time zone ..."]
        :param reset:how connections should be reset when returned to the pool
            (False or None to rollback transcations started with begin(),
            True to always issue a rollback for safety's sake)
        :param host:数据库ip地址
        :param port:数据库端口
        :param db:库名
        :param user:用户名
        :param passwd:密码
        :param charset:字符编码
        """

        if not self.__pool:
            self.__class__.__pool = PooledDB(
              pymysql,
              mincached, 
              maxcached,
              maxshared, 
              maxconnections, 
              blocking,
              maxusage, 
              setsession, 
              reset,
              ping=0,
              host=host, 
              port=port, 
              db=db,
              user=user, 
              passwd=passwd,
              charset=charset,
              cursorclass=pymysql.cursors.DictCursor
            )
        self.__db_conns = None
        self.__db_cursors = None
        self.__get_conn()

    def __get_conn(self):
        self.__db_conns = self.__pool.connection()
        self.__db_cursors = self.__db_conns.cursor()
        # print(type(self.__db_conns))
        # print(type(self.__db_cursors))

    def __del__(self):
        self.__close()

    def __close(self):
        self.__db_cursors.close()
        self.__db_conns.close()

    def __execute(self, sql, param=()):
        count = self.__db_cursors.execute(sql, param)
        # print(count)
        result = self.__db_cursors.fetchall()
        # print(result)
        return count, result

    def execute(self, sql, param=()):
        # print(sql)
        count, result = self.__execute(sql, param)
        return count, [mysqlClient.__dict_datetime_obj_to_str(row_dict) for row_dict in result]

    @staticmethod
    def __dict_datetime_obj_to_str(result_dict) -> dict:
        """把字典里面的datatime对象转成字符串，使json转换不出错"""
        if result_dict:
            result_replace = {k: v.__str__() for k, v in result_dict.items() if isinstance(v, datetime.datetime)}
            result_dict.update(result_replace)
        return result_dict

class mysqlOps(mysqlClient):
    def __init__(self, host: str, port: int, db: str, user: str, passwd: str):
        super().__init__(host, port, db, user, passwd)

    def create_market_data_table(self, table_name : str):
        sql = "CREATE TABLE IF NOT EXISTS `{}` (\
            `instrument_id` VARCHAR(8) NOT NULL,\
            `td_update_datetime` DATETIME(6) NOT NULL,\
            `raw_conent` Blob NOT NULL,\
            UNIQUE KEY (`instrument_id`,`td_update_datetime`) USING BTREE\
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;".format(table_name)
        return self.execute(sql)

    def update_tick(self, table_name : str, tick : str) -> bool:
        tick_dict = json.loads(tick)
        dt_str_splice = tick_dict['trading_day'] + tick_dict['update_time']
        # print(dt_str_splice)
        dt = datetime.datetime.strptime(dt_str_splice, '%Y%m%d%H:%M:%S').replace(microsecond=tick_dict["update_millisec"])
        # print(dt)
        dt_str = dt.strftime('%Y-%m-%d %H:%M:%S.%f')
        sql = "INSERT INTO {} (`instrument_id`,`td_update_datetime`,`raw_conent`) VALUES ('{}','{}','{}') ON DUPLICATE KEY UPDATE `raw_conent`='{}';".format(
            table_name, tick_dict['instrument_id'], dt_str, tick, tick
        )
        # print(sql)
        return 1 == self.execute(sql)[0]

    def load_all_ticks(self, table_name : str, instrument : str):
        sql = "SELECT `raw_conent` FROM {} WHERE `instrument_id`='{}'".format(table_name, instrument)
        num, rows = self.execute(sql)
        ticks = []
        for r in rows:
            one_json_tick = json.loads(str(r['raw_conent'], encoding='utf-8'))
            ticks.append( one_json_tick )
        if num != len(ticks):
            raise Exception("table {} loaded {} ticks num is inconsistent".format(table_name, instrument))
        return ticks

if __name__ == "__main__":
    mysql_opser = mysqlOps( host="localhost", port=3306, db="ctp",
                            user="ctp", passwd="cttppp")
    # cnt, res_list = mysql_opser.execute("SHOW DATABASES;")
    cnt, res_list = mysql_opser.execute("SHOW DATABASES;")
    print(res_list)
    print(cnt, len(res_list))
    table_name = "test_table_1000"
    print(mysql_opser.create_market_data_table(table_name))
    tick_str = "{\"ask_price1\":4872.0,\"ask_volume1\":1,\"bid_price1\":4868.0,\"bid_volume1\":30,\"close_price\":1.7976931348623157e+308,\"exchange_id\":\"\",\"exchange_inst_id\":\"\",\"highest_price\":4884.0,\"instrument_id\":\"ag2315\",\"last_price\":4872.0,\"lower_limit_price\":4355.0,\"lowest_price\":4821.0,\"open_interest\":19712.0,\"open_price\":4828.0,\"pre_close_price\":4837.0,\"pre_open_interest\":19623.0,\"pre_settlement_price\":4839.0,\"settlement_price\":1.7976931348623157e+308,\"trading_day\":\"20221122\",\"turnover\":216813465.0,\"update_millisec\":100,\"update_time\":\"22:29:07\",\"upper_limit_price\":5322.0,\"volume\":2973}"
    print(mysql_opser.update_tick(table_name, tick_str))
    ticks = mysql_opser.load_all_ticks(table_name, "IH2212")
    for _ in ticks:
        print(_, "\n")
    print(len(ticks))

