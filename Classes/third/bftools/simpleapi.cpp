//
//  simpleapi.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/3.
//

#include "simpleapi.hpp"
#include <ctime>
#include <chrono>
#include <stdlib.h>

NS_BF_BEGIN

Times stamp_to_standard(int stampTime) {
    time_t tick = (time_t)stampTime;
    struct tm tm;
    char s[100];
    Times standard;
    
    //tick = time(NULL);
    tm = *localtime(&tick);
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
    //printf("%d: %s\n", (int)tick, s);
    
    standard.Year = atoi(s);
    standard.Mon = atoi(s+5);
    standard.Day = atoi(s+8);
    standard.Hour = atoi(s+11);
    standard.Min = atoi(s+14);
    standard.Second = atoi(s+17);
    
    return standard;
}

std::time_t getTimeStamp() {
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    //std::time_t timestamp = tmp.count();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(tp);
    return timestamp;
}

NS_BF_END
