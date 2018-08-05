//
//  JSApiRegisterFunc.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/13.
//

#ifndef JSApiRegisterFunc_hpp
#define JSApiRegisterFunc_hpp

#include <stdio.h>
#include "app/macros.h"
#include "jsapi.h"

NS_BF_BEGIN

void register_story_api(JSContext *cx, JS::HandleObject global);

NS_BF_END

#endif /* JSApiRegisterFunc_hpp */
