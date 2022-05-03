#pragma once 

#include "pool/pg_pool.hpp"
#include "tools/string_util.hpp"

    // order_id varchar(30) primary key,
    // direction varchar(10),
    // comb_offset varchar(10),
    // instrument_id varchar(50),
    // price double precision,
    // volume int,
    // traded int,
    // order_status varchar(20),
    // strategy_name varchar(50),
    // create_time bigint,
    // update_time bigint

struct StrategyOrder{
    std::string order_id;
    std::string direction;
    std::string comb_offset;
    std::string instrument_id;
    double price;
    int volume;
    int traded;
    std::string order_status;
    std::string strategy_name;
    long long create_time;
    long long update_time;
};

class StrategyOrderDao {

    public:

        // 创建订单
        static void createOrder(StrategyOrder& order) {

            std::string createTemplate = R"(insert into strategy_order values(
                '%s',
                '%s',
                '%s',
                '%s',
                '%lf',
                '%d',
                '%d',
                '%s',
                '%s',
                '%ld',
                '%ld'
            ))";
            std::string sql = string_format(createTemplate.c_str(),
                order.order_id.c_str(), order.direction.c_str(), order.comb_offset.c_str(), order.instrument_id.c_str(), 
                order.price, order.volume, order.traded, order.order_status.c_str(), order.strategy_name.c_str(),
                order.create_time, order.update_time);
            // LOG_INFO("sql {0}.", sql);
            try {
                std::shared_ptr<pqxx::connection> conn = PGConnectionPool::getInstance()->getConn();
                pqxx::work W(*conn.get());
                W.exec(sql);
                W.commit();
            } catch (const std::exception &e) {
                LOG_ERROR("创建订单失败. {0}.", e.what());
                throw e;
            }
        }

        static void updateOrderStatus(const StrategyOrder& order) {
            std::string updateOrderstatusTemplate = R"(
                update strategy_order
                set order_status = '%s', update_time = '%ld'
                where order_id = '%s'
            )";
            std::string sql = string_format(updateOrderstatusTemplate.c_str(),
                order.order_status.c_str(), order.update_time, order.order_id.c_str());
            try {
                std::shared_ptr<pqxx::connection> conn = PGConnectionPool::getInstance()->getConn();
                pqxx::work W(*conn.get());
                W.exec(sql);
                W.commit();
            } catch (const std::exception &e) {
                LOG_ERROR("更新订单状态失败. {0}.", e.what());
                throw e;
            }
        }

        static void updateTrade(const StrategyOrder& order) {
            std::string tradeTemplate = R"(
                update strategy_order
                set traded = traded + %d, update_time = '%ld'
                where order_id = '%s'
            )";

            std::string sql = string_format(tradeTemplate.c_str(),
            order.traded, order.update_time, order.order_id.c_str());
            try {
                std::shared_ptr<pqxx::connection> conn = PGConnectionPool::getInstance()->getConn();
                pqxx::work W(*conn.get());
                W.exec(sql);
                W.commit();
            } catch (const std::exception &e) {
                LOG_ERROR("更新订单成交失败. {0}.", e.what());
                throw e;
            }
        }

};
