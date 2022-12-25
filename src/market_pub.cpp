/**

    行情分发器，用于分发tick行情

**/

#include "bus/market_business.hpp"
#include "bus/trader_business.hpp"
#include "config/yaml_config_base.h"
#include "ctp_market.hpp"
#include "ctp_trade.hpp"
#include "log.hpp"
#include "model/market.hpp"
#include "pubsub/pub.hpp"
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <signal.h>
#include <string>
#include <unistd.h>

// bool exit_market_pub = false;

// void signal_exit_handler(int sig)
// {
//     LOG_INFO("退出程序.");
//     exit_market_pub = true;
// }

int main(int argc, char *argvs[]) {

  std::shared_ptr<YamlConfigBase> conf;
  if (argc < 2) {
    std::cout << "Usage : " << argvs[0] << " <yaml-config-file>" << std::endl;
    std::exit(EXIT_SUCCESS);
  }
  try {
    if (std::filesystem::exists(argvs[1])) {
      conf = std::shared_ptr<YamlConfigBase>(new YamlConfigBase(argvs[1]));
    }
  } catch (YAML::ParserException &e) {
    std::cout << "Parser " << argvs[1] << " failed!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // signal(SIGTERM, signal_exit_handler);
  // signal(SIGINT, signal_exit_handler);

  auto using_address = (conf->get_config())["using_address"];
  const std::string pattern = using_address["pattern"].as<std::string>();
  const int idx = using_address["index"].as<int>() - 1;

  auto ctp = using_address["ctp"];
  auto market_addr_conf = ctp[pattern.data()]["market"][idx];
  auto trade_addr_conf = ctp[pattern.data()]["trade"][idx];

  std::string userId = ctp["userID"].as<std::string>();
  std::string password = ctp["password"].as<std::string>();
  std::string brokerId = ctp["brokerID"].as<std::string>();
  std::string investorId = ctp["investorID"].as<std::string>();

  // 初始化行情获取配置
  std::string md_dir = using_address["dir"].as<std::string>();
  std::string md_addr = market_addr_conf["protocol"].as<std::string>() + "://" +
                        market_addr_conf["ip"].as<std::string>() + ":" +
                        market_addr_conf["port"].as<std::string>();

  // 初始化交易配置
  std::string td_dir = using_address["dir"].as<std::string>();
  std::string td_addr = trade_addr_conf["protocol"].as<std::string>() + "://" +
                        trade_addr_conf["ip"].as<std::string>() + ":" +
                        trade_addr_conf["port"].as<std::string>();

  LOG_INFO("md_dir = ", md_dir.data(), ",  md_addr = ", md_addr.data(),
           ",  td_dir = ", td_dir.data(), ",  td_addr = ", td_addr.data(),
           ",  userId = ", userId.data(), ",  password = ", password.data(),
           ",  brokerId = ", brokerId.data(),
           ",  investorId = ", investorId.data());

  // 初始化td
  std::shared_ptr<CtpTrade> ctpTrade = std::shared_ptr<CtpTrade>(
      new CtpTrade(userId, password, brokerId, investorId));
  ctpTrade->connect(td_dir, td_addr);
  while (!ctpTrade->isConnected()) {
    LOG_INFO("等待交易前置连接成功......");
    sleep(3);
  }

  // 初始化trader
  // std::shared_ptr<TraderBusiness> tdBus = std::shared_ptr<TraderBusiness>(new
  // TraderBusiness(traderUrl)); tdBus->runAsync();

  //查询全市场合约行情
  ctpTrade->qryAllInstrument();
  while (!SymbolHolder::getInstance()->isReady()) {
    sleep(1);
  }
  LOG_INFO("all symbols length {0}.",
           SymbolHolder::getInstance()->getAllInstrument().size());

  // 准备要订阅的合约
  std::vector<std::string> contracts;
  std::vector<std::string> allContracts =
      SymbolHolder::getInstance()->getAllInstrument();
  for (int i = 0; i < allContracts.size(); i++) {
    contracts.push_back(allContracts[i]);
  }

  // 测试使用
  // contracts.clear();
  // contracts.push_back("cu2305");

  // 初始化行情分发器配置
  auto market_data = (conf->get_config())["market_data"];
  std::string market_data_chn = market_data["channel"].as<std::string>();
  std::string market_data_ip = market_data["ip"].as<std::string>();
  auto market_data_port = market_data["port"].as<std::int32_t>();
  std::string market_data_passwd = "";
#if 0  // !!! TOFIX 这里暂时有点问题
  if (!market_data["password"].IsNull()) {
    market_data_passwd = market_data["password"].as<std::string>();
  }
#endif // !!! TOFIX 这里暂时有点问题
  // std::string traderUrl = Config::instance()->getTraderUrl();
  LOG_INFO("market_data channel = ", market_data_chn.data(),
           ", ip = ", market_data_ip.data(),
           ", port = ", std::to_string(market_data_port),
           ", passwd = ", market_data_passwd.data());

  // 初始化ctp行情获取
  std::shared_ptr<MarketPub> pub(new MarketPub(
      market_data_chn, market_data_ip, market_data_port, market_data_passwd));
  pub->connect(md_dir, md_addr);
  while (!pub->isConnected()) {
    LOG_INFO("等待行情前置连接成功......");
    sleep(3);
  }
  pub->subscribe(contracts);

  // 等待
  while (true) {
    sleep(1);
  }
}
