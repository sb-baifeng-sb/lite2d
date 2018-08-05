//
//  actions.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/25.
//

#ifndef actions_hpp
#define actions_hpp

#include <stdio.h>
#include "app/macros.h"
#include "cocos2d.h"
#include "jsapi.h"

NS_BF_BEGIN

class JSEngine;
class JSCallbackAction : public cocos2d::CallFunc {
public:
    typedef std::shared_ptr<JSEngine> JSEnginePtr;
public:
    static JSCallbackAction* create(JSEnginePtr engine, std::string const& code);
    JSCallbackAction(JSEnginePtr engine, std::string const& code);
private:
    virtual void execute() override;
    virtual JSCallbackAction* reverse() const override;
    virtual JSCallbackAction* clone() const override;
private:
    JSEnginePtr mJSEngine;
    std::string mCode;
};

class ActionHelper {
public:
    typedef cocos2d::FiniteTimeAction Action;
    typedef std::function<Action*(JSContext* cx, JS::HandleObject root)> MakeActionFunc;
    typedef std::map<std::string, MakeActionFunc> FuncMap;
public:
    static Action* makeAction(JSContext* cx, JS::HandleObject root);
private:
    static Action* makeMoveToAction(JSContext* cx, JS::HandleObject root);
    static Action* makeMoveByAction(JSContext* cx, JS::HandleObject root);
    static Action* makeScaleToAction(JSContext* cx, JS::HandleObject root);
    static Action* makeScaleByAction(JSContext* cx, JS::HandleObject root);
    static Action* makeRotateToAction(JSContext* cx, JS::HandleObject root);
    static Action* makeRotateByAction(JSContext* cx, JS::HandleObject root);
    static Action* makeTintToAction(JSContext* cx, JS::HandleObject root);
    static Action* makeTintByAction(JSContext* cx, JS::HandleObject root);
private:
    static Action* makeJumpToAction(JSContext* cx, JS::HandleObject root);
    static Action* makeJumpByAction(JSContext* cx, JS::HandleObject root);
    static Action* makeFadeInAction(JSContext* cx, JS::HandleObject root);
    static Action* makeFadeOutAction(JSContext* cx, JS::HandleObject root);
    static Action* makeEaseElasticInAction(JSContext* cx, JS::HandleObject root);
    static Action* makeEaseElasticOutAction(JSContext* cx, JS::HandleObject root);
private:
    static Action* makeBlinkAction(JSContext* cx, JS::HandleObject root);
    static Action* makeDelayAction(JSContext* cx, JS::HandleObject root);
    static Action* makeRepeatAction(JSContext* cx, JS::HandleObject root);
    static Action* makeSequenceAction(JSContext* cx, JS::HandleObject root);
    static Action* makeCallbackAction(JSContext* cx, JS::HandleObject root);
private:
    static void registerMakeActionFuncs();
private:
    static FuncMap mFuncMap;
};

NS_BF_END

#endif /* actions_hpp */
