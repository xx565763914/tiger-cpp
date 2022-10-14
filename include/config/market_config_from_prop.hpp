#pragma once

#include "config/base_config.hpp"
#include "const/psm.hpp"
#include <memory>
#include <confuse.h>
#include <vector>
#include <assert.h>
#include <stdexcept>

class MarketConfigFromProp : public BaseConfig {
    public:
        MarketConfigFromProp() {
            this->setPSM(PSM::MARKET_PSM);
            assert(opts.size() > 0);
            this->cfg = cfg_init(&opts[0], CFGF_NONE);
            if (cfg_parse(this->cfg, this->configFile.c_str()) == CFG_PARSE_ERROR) {
                throw std::runtime_error("加载行情服务配置失败!");
            }
        }

        ~MarketConfigFromProp() {
            if (cfg != nullptr) {
                cfg_free(cfg);
            }
        }

        std::string getConfig(std::string configKey) {
            std::string key = this->psm + "." + configKey;
            return std::string(cfg_getstr(this->cfg, key.c_str()));
        }

    private:
        std::string configFile = "./config/" + PSM::MARKET_PSM +".properties";
        cfg_t *cfg = nullptr;
        std::vector<cfg_opt_t> opts = {
            CFG_STR("market.t", "", CFGF_NONE),
            CFG_END()
        };
};
