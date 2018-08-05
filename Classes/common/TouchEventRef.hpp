//
//  TouchEventRef.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/7.
//

#ifndef TouchEventRef_hpp
#define TouchEventRef_hpp

#include <stdio.h>
#include "app/macros.h"
#include "cocos2d.h"

NS_BF_BEGIN

class TouchEventRef : public cocos2d::Ref {
public:
    typedef std::function<bool(TouchEventRef const* ref, cocos2d::Touch *pTouch, cocos2d::Event *pEvent)> TouchBeganFunc;
    typedef std::function<void(TouchEventRef const* ref, cocos2d::Touch *pTouch, cocos2d::Event *pEvent)> TouchEventFunc;
public:
    CREATE_FUNC(TouchEventRef);
    TouchEventRef();
    virtual ~TouchEventRef(){}
public:
    static cocos2d::EventListenerTouchOneByOne* NewTouchListener(cocos2d::Node* target, TouchBeganFunc began = nullptr, TouchEventFunc moved = nullptr, TouchEventFunc ended = nullptr, TouchEventFunc cancel = nullptr);
public:
    BF_GETTER(cocos2d::Node*, TargetView);
protected:
    virtual bool init();
protected:
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
protected:
    TouchBeganFunc mTouchBegan;
    TouchEventFunc mTouchMoved, mTouchEnded, mTouchCancel;
};

NS_BF_END

#endif /* TouchEventRef_hpp */
