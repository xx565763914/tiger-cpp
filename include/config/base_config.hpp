#pragma once

#include <confuse.h>
#include <string>

class BaseConfig {
    public:
        // 设置psm
        void setPSM(const std::string &psm) {
            this->psm = psm;
        }

        virtual std::string getConfig(std::string configKey)=0; // 读取config配置

    protected:
        std::string psm;
};



