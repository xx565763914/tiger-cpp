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

struct OpenPosition {
    int long_position;
    int short_position;
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

        static OpenPosition getOpenPosition(std::string strategy_name) {
            // 分别统计多头头寸和空头头寸
            std::string sqlTemplate = R"(
                select 
                    (select case when sum(traded) is null then 0 else sum(traded) end from strategy_order where direction = 'long' and comb_offset = 'open' and strategy_name = '%s') - (select case when sum(traded) is null then 0 else sum(traded) end from strategy_order where direction = 'short' and comb_offset = 'close' and strategy_name = '%s') as long,
                    (select case when sum(traded) is null then 0 else sum(traded) end from strategy_order where direction = 'short' and comb_offset = 'open' and strategy_name = '%s') - (select case when sum(traded) is null then 0 else sum(traded) end from strategy_order where direction = 'long' and comb_offset = 'close' and strategy_name = '%s') as short;
            )";
            std::string sql = string_format(sqlTemplate.c_str(), strategy_name.c_str(), strategy_name.c_str(), 
                strategy_name.c_str(), strategy_name.c_str());
            try {
                std::shared_ptr<pqxx::connection> conn = PGConnectionPool::getInstance()->getConn();
                pqxx::work W(*conn.get());
                pqxx::result R(W.exec(sql));
                OpenPosition position;
                for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
                    position.long_position = c[0].as<int>();
                    position.short_position = c[1].as<int>();
                }
                LOG_INFO("long {0} short {1}.", position.long_position, position.short_position);
                W.commit();
                return position;
            } catch (const std::exception &e) {
                LOG_ERROR("更新订单成交失败. {0}.", e.what());
                throw e;
            }
        }

};
