#pragma once

#include <iconv.h>


class Conv {

    public:

        ~Conv() {
            iconv_close(gbkToUtf8);
        }

        static std::string gbk2Utf8(std::string &in) {
            char buffer[512];
            std::string out;

            size_t outbytesleft = sizeof(buffer);
            size_t inbytesleft = in.size();
            char* inbuf = const_cast<char*>(in.c_str());
            char* outbuf = buffer;
            int ret = 0;
            for (;;) {
                ret = iconv(gbkToUtf8, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
                if (0 == ret)
                {
                    out.insert(out.end(), buffer, buffer + sizeof(buffer)-outbytesleft);
                    break;
                }
                else {
                    assert(inbytesleft > 0);
                    assert(outbytesleft == 0);
                    out.insert(out.end(), buffer, buffer + sizeof(buffer));
                    outbytesleft = sizeof(buffer);
                    outbuf = buffer;
                }
            }
            return out;
        }  

    private:
        static iconv_t gbkToUtf8;
};

iconv_t Conv::gbkToUtf8 = iconv_open("gbk", "utf-8");
