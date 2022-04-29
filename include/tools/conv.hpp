#pragma once

#include <string>
#include <boost/locale.hpp>

class Conv {

    public:

        static std::string GBKToUTF8(const std::string& strGBK) {
            return boost::locale::conv::to_utf<char>(strGBK.c_str(), std::string("gb2312"));
        }

        static std::string GBKToUTF8(char *input) {
            std::string gbkStr(input);
            return GBKToUTF8(gbkStr);
        }
};

