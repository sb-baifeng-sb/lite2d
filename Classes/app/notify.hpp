//
//  notify.h
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/14.
//

#ifndef notify_h
#define notify_h

#include "app/macros.h"

NS_BF_BEGIN

class Event {
public:
    typedef enum {
        SystemFire = 10001,
    } System;
    
    typedef enum {
        PushStoryView = 10002,
        PopStoryView = 10003,
        PauseStoryScript = 10004,
        ResumeStoryScript = 10005,
        PauseBackGroundMusic = 10006,
        ResumeBackGroundMusic = 10007,
    } Logic;
};

NS_BF_END

#endif /* notify_h */
