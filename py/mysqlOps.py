import pymysql
import datetime
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
                maxusage=1,
                setsession=None, 
                reset=True,
                charset='utf8mb4'
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
        count, result = self.__execute(sql, param)
        return count, [mysqlClient.__dict_datetime_obj_to_str(row_dict) for row_dict in result]

    @staticmethod
    def __dict_datetime_obj_to_str(result_dict) -> dict:
        """把字典里面的datatime对象转成字符串，使json转换不出错"""
        if result_dict:
            result_replace = {k: v.__str__() for k, v in result_dict.items() if isinstance(v, datetime.datetime)}
            result_dict.update(result_replace)
        return result_dict

class mysqlOps:
    pass

if __name__ == "__main__":
    mysql_cli = mysqlClient(host="localhost", port=3306, 
                            db="mysql",
                            user="root", passwd="root")
    # cnt, res_list = mysql_cli.execute("SHOW DATABASES;")
    cnt, res_list = mysql_cli.execute("SELECT * FROM user")
    print(res_list)
    print(cnt, len(res_list))
