#pragma once

#include <string>
#include <memory>

/**
    项目配置全部收敛于此
**/

// 此类将会被淘汰
class Config {

    public:
        // 行情分发地址
        std::string getMarketPubUrl() {
            return market_url;
        }

        // trader url地址
        std::string getTraderUrl() {
            return trader_url;
        }

        // 获取行情相关的配置
        std::string getMdDir() {
            return md_dir;
        }
        std::string getMdAddr() {
            return md_addr;
        }

        // 获取交易相关配置
        std::string getTdDir() {
            return td_dir;
        }
        std::string getTdAddr() {
            return td_addr;
        }
        std::string getUserId() {
            return userId;
        }
        std::string getPassword() {
            return password;
        }
        std::string getBrokerId() {
            return brokerId;
        }
        std::string getInvestorId() {
            return investorId;
        }

        std::string getConnectionUrl() {
            return connectionUrl;
        }

        // 单例模式
        static std::shared_ptr<Config> instance() {
            return config;
        }

    private:
        std::string ctp_dir = "./ctp/";
        
        // 初始化行情获取配置
        std::string md_dir = ctp_dir;
        std::string md_addr = "tcp://180.168.146.187:10131";
        // std::string md_addr = "tcp://180.168.146.187:10211";

        // 初始化交易配置
        std::string td_dir = ctp_dir;
        std::string td_addr = "tcp://180.168.146.187:10130";
        // std::string td_addr = "tcp://180.168.146.187:10201";
        std::string userId = "113458";
        std::string password = "XX1024xx!@#$";
        std::string brokerId = "9999";
        std::string investorId = "113458";

        // 初始化行情分发器
        std::string market_url = "tcp://127.0.0.1:5555";

        // 初始化trader地址
        std::string trader_url = "tcp://127.0.0.1:8888";
        
        // 数据库相关配置
        //dbname=tiger user=postgres password= hostaddr=127.0.0.1 port=5432
        std::string connectionUrl = "dbname=tiger user=postgres password= hostaddr=127.0.0.1 port=5432";

        static std::shared_ptr<Config> config;
};

std::shared_ptr<Config> Config::config = std::shared_ptr<Config>(new Config);
