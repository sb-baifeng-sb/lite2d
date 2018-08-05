//
//  TouchEventRef.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/7.
//

#include "TouchEventRef.hpp"

NS_BF_BEGIN

class EventListenerTouchOneByOneImp : public cocos2d::EventListenerTouchOneByOne {
public:
    CREATE_FUNC(EventListenerTouchOneByOneImp);
    EventListenerTouchOneByOneImp():mEventRef(nullptr) {
        
    }
    ~EventListenerTouchOneByOneImp() {
        CC_SAFE_RELEASE(mEventRef);
    }
public:
    void setTouchEventRef(cocos2d::Ref* ref) {
        CC_SAFE_RELEASE(mEventRef);
        mEventRef = ref;
        CC_SAFE_RETAIN(mEventRef);
    }
private:
    cocos2d::Ref* mEventRef;
};

TouchEventRef::TouchEventRef():mTargetView(nullptr) {
    
}

bool TouchEventRef::init() {
    return true;
}

cocos2d::EventListenerTouchOneByOne* TouchEventRef::NewTouchListener(cocos2d::Node* target, TouchEventRef::TouchBeganFunc began, TouchEventRef::TouchEventFunc moved, TouchEventRef::TouchEventFunc ended, TouchEventRef::TouchEventFunc cancel) {
    auto obj = TouchEventRef::create();
    obj->mTouchBegan = began;
    obj->mTouchMoved = moved;
    obj->mTouchEnded = ended;
    obj->mTouchCancel = cancel;
    obj->mTargetView = target;
    auto listener = EventListenerTouchOneByOneImp::create();
    listener->onTouchBegan = CC_CALLBACK_2(TouchEventRef::onTouchBegan, obj);
    listener->onTouchMoved = CC_CALLBACK_2(TouchEventRef::onTouchMoved, obj);
    listener->onTouchEnded = CC_CALLBACK_2(TouchEventRef::onTouchEnded, obj);
    listener->onTouchCancelled = CC_CALLBACK_2(TouchEventRef::onTouchCancel, obj);
    listener->setTouchEventRef(obj);
    return listener;
}

bool TouchEventRef::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    if (mTouchBegan != nullptr) {
        return mTouchBegan(this, pTouch, pEvent);
    }
    return false;
}

void TouchEventRef::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    if (mTouchMoved != nullptr) {
        mTouchMoved(this, pTouch, pEvent);
    }
}

void TouchEventRef::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    if (mTouchEnded != nullptr) {
        mTouchEnded(this, pTouch, pEvent);
    }
}

void TouchEventRef::onTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    if (mTouchCancel != nullptr) {
        mTouchCancel(this, pTouch, pEvent);
    }
}

NS_BF_END
