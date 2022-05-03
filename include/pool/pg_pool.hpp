#pragma once

#include <pqxx/pqxx>
#include <memory>
#include <string>
#include "queue/blocking_queue.hpp"
#include "config.hpp"
#include "log.hpp"

class PGConnectionPool {

    public:
        
        PGConnectionPool(int connectionSize) {
            this->connectionSize = connectionSize;

            // 初始化连接配置
            for (int i = 0; i < connectionSize; i++) {
                pqxx::connection* connPtr = getNewConnection();
                if (connPtr->is_open()) {
                    LOG_INFO("连接postgres数据库成功.");
                } else {
                    LOG_ERROR("连接postgresq数据库失败.");
                    throw "连接postgresql数据库失败,请检查连接url配置是否正确!";
                }
                connectionQueue.put(connPtr);
            }   
        }

        std::shared_ptr<pqxx::connection> getConn() {
            pqxx::connection* conn = connectionQueue.take();
            if (!conn->is_open()) {
                conn = getNewConnection();
            }

            auto releaseFunc = [&](pqxx::connection* connPtr){
                LOG_INFO("释放数据库连接");
                connectionQueue.put(connPtr);
            };

            return std::shared_ptr<pqxx::connection>(conn, releaseFunc);
        }

        static std::shared_ptr<PGConnectionPool> getInstance() {
            return instance;
        }

    private:
        //申请一个新的数据库连接
        pqxx::connection* getNewConnection() {
            std::string connectionUrl = Config::instance()->getConnectionUrl();
            pqxx::connection* connPtr = new pqxx::connection(connectionUrl);
            return connPtr;
        }


    private:
        int connectionSize;
        BlockingQueue<pqxx::connection*> connectionQueue;
        static std::shared_ptr<PGConnectionPool> instance;

};

std::shared_ptr<PGConnectionPool> PGConnectionPool::instance = std::shared_ptr<PGConnectionPool>(new PGConnectionPool(5));
