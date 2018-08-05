//
//  simpleapi.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/3.
//

#ifndef simpleapi_hpp
#define simpleapi_hpp

#include <stdio.h>
#include <ctime>
#include "macros.h"

NS_BF_BEGIN

typedef struct {
    int Year;
    int Mon;
    int Day;
    int Hour;
    int Min;
    int Second;
} Times;
Times stamp_to_standard(int stampTime); // 时间戳转日期
std::time_t getTimeStamp(); // 获取时间戳

NS_BF_END

#endif /* simpleapi_hpp */
