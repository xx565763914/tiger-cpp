#include "config/yaml_config_base.h"
#include <iostream>
#include <string>

int main(int argc, char *argvs[]) {

  YamlConfigBase yaml(argvs[1]);

  auto conf = yaml.get_config();
  auto simnow_simulation = conf["simnow"]["simulation"];
  auto simnow_simulation_market = simnow_simulation["market"];
  auto simnow_simulation_trade = simnow_simulation["trade"];

  for (std::size_t i = 0; i < simnow_simulation_market.size(); i++) {
    auto market_url_conf = simnow_simulation_market[i];
    auto trade_url_conf = simnow_simulation_trade[i];
    std::string market_url = market_url_conf["protocol"].as<std::string>() +
                             "://" + market_url_conf["ip"].as<std::string>() +
                             ":" + market_url_conf["port"].as<std::string>();
    std::string trade_url = trade_url_conf["protocol"].as<std::string>() +
                            "://" + trade_url_conf["ip"].as<std::string>() +
                            ":" + trade_url_conf["port"].as<std::string>();
    std::cout << i + 1 << "\n";
    std::cout << "\t market url = " << market_url << "\n";
    std::cout << "\t trade url = " << trade_url << "\n";
  }

  auto userID = conf["simnow"]["userID"];
  auto password = conf["simnow"]["password"];
  auto brokerID = conf["simnow"]["brokerID"];
  auto investorID = conf["simnow"]["investorID"];
  std::cout << userID << ", " << password << ", " << brokerID << ", "
            << investorID << std::endl;

  return 0;
}