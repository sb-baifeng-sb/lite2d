//
//  CharsetConvert.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/13.
//

#ifndef CharsetConvert_hpp
#define CharsetConvert_hpp

#include <stdio.h>
#include "app/macros.h"

NS_BF_BEGIN

class CharsetConvert {
public:
    static std::string GBKToUTF8(const char* msg);
    static  std::string UTF8ToGBK(const char* msg);
    static int IsTextUTF8(const char* str,int length);
private:
    static int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);
};

NS_BF_END

#endif /* CharsetConvert_hpp */
