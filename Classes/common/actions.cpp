//
//  actions.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/25.
//

#include "actions.hpp"
#include "tools/jsengine.hpp"
#include "app/App.hpp"
#include "proxy/StoryScriptProxy.hpp"

NS_BF_BEGIN

#define JSB_PRECONDITION2( condition, context, ret_value, ...) do {             \
    if( ! (condition) ) {                                                       \
        JSEngine::Log("jsb: ERROR: File %s: Line: %d, Function: %s", __FILE__, __LINE__, __FUNCTION__ );         \
        JSEngine::Log(__VA_ARGS__);                                        \
        if( ! JS_IsExceptionPending( context ) ) {                          \
            JS_ReportError( context, __VA_ARGS__ );                             \
        }                                                                       \
        return ret_value;                                                       \
    }                                                                           \
} while(0)

JSCallbackAction* JSCallbackAction::create(JSEnginePtr engine, std::string const& code) {
    auto action = new (std::nothrow) JSCallbackAction(engine, code);
    if (action != nullptr) {
        action->autorelease();
        return action;
    }
    return nullptr;
}

JSCallbackAction::JSCallbackAction(JSEnginePtr engine, std::string const& code):mJSEngine(engine), mCode(code) {
    
}

void JSCallbackAction::execute() {
    if (this->mJSEngine != nullptr) {
        this->mJSEngine->doCode(this->mCode);
    }
}

JSCallbackAction* JSCallbackAction::reverse() const {
    return this->clone();
}

JSCallbackAction* JSCallbackAction::clone() const {
    return JSCallbackAction::create(this->mJSEngine, this->mCode);
}


ActionHelper::FuncMap ActionHelper::mFuncMap;

void ActionHelper::registerMakeActionFuncs() {
    if (mFuncMap.size() != 0) {
        return;
    }
    mFuncMap["move_by"] = ActionHelper::makeMoveByAction;
    mFuncMap["move_to"] = ActionHelper::makeMoveToAction;
    mFuncMap["scale_by"] = ActionHelper::makeScaleByAction;
    mFuncMap["scale_to"] = ActionHelper::makeScaleToAction;
    mFuncMap["rotate_by"] = ActionHelper::makeRotateByAction;
    mFuncMap["rotate_to"] = ActionHelper::makeRotateToAction;
    mFuncMap["jump_by"] = ActionHelper::makeJumpByAction;
    mFuncMap["jump_to"] = ActionHelper::makeJumpToAction;
    mFuncMap["tint_by"] = ActionHelper::makeTintByAction;
    mFuncMap["tint_to"] = ActionHelper::makeTintToAction;
    mFuncMap["fade_in"] = ActionHelper::makeFadeInAction;
    mFuncMap["fade_out"] = ActionHelper::makeFadeOutAction;
    mFuncMap["ease_elastic_in"] = ActionHelper::makeEaseElasticInAction;
    mFuncMap["ease_elastic_out"] = ActionHelper::makeEaseElasticOutAction;
    
    mFuncMap["blink"] = ActionHelper::makeBlinkAction;
    mFuncMap["delay"] = ActionHelper::makeDelayAction;
    mFuncMap["repeat"] = ActionHelper::makeRepeatAction;
    mFuncMap["sequence"] = ActionHelper::makeSequenceAction;
    mFuncMap["callback"] = ActionHelper::makeCallbackAction;
}

ActionHelper::Action* ActionHelper::makeAction(JSContext* cx, JS::HandleObject root) {
    registerMakeActionFuncs();
    bool ok = true;
    JS::RootedValue name(cx);
    ok &= JS_GetProperty(cx, root, "name", &name);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeAction : Error processing arguments");
    std::string nameStr;
    ok &= jsval_to_std_string(cx, name, &nameStr);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeAction : Error processing arguments");
    auto iter = mFuncMap.find(nameStr);
    if (iter != mFuncMap.end()) {
        return iter->second(cx, root);
    }
    return nullptr;
}

ActionHelper::Action* ActionHelper::makeMoveToAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), x(cx), y(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeMoveToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "x", &x);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeMoveToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "y", &y);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeMoveToAction : Error processing arguments");
    return cocos2d::MoveTo::create(
                                   duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                   cocos2d::Vec2(x.isDouble() ? x.toDouble() : x.toInt32(), y.isDouble() ? y.toDouble() : y.toInt32()));
}

ActionHelper::Action* ActionHelper::makeMoveByAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), x(cx), y(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeMoveByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "x", &x);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeMoveByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "y", &y);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeMoveByAction : Error processing arguments");
    return cocos2d::MoveBy::create(
                                   duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                   cocos2d::Vec2(x.isDouble() ? x.toDouble() : x.toInt32(), y.isDouble() ? y.toDouble() : y.toInt32()));
}

ActionHelper::Action* ActionHelper::makeScaleToAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), x(cx), y(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeScaleToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "x", &x);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeScaleToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "y", &y);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeScaleToAction : Error processing arguments");
    return cocos2d::ScaleTo::create(
                                    duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                    x.isDouble() ? x.toDouble() : x.toInt32(),
                                    y.isDouble() ? y.toDouble() : y.toInt32());
}

ActionHelper::Action* ActionHelper::makeScaleByAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), x(cx), y(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeScaleByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "x", &x);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeScaleByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "y", &y);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeScaleByAction : Error processing arguments");
    return cocos2d::ScaleBy::create(
                                    duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                    x.isDouble() ? x.toDouble() : x.toInt32(),
                                    y.isDouble() ? y.toDouble() : y.toInt32());
}

ActionHelper::Action* ActionHelper::makeRotateToAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), x(cx), y(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeRotateToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "x", &x);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeRotateToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "y", &y);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeRotateToAction : Error processing arguments");
    return cocos2d::RotateTo::create(
                                     duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                     x.isDouble() ? x.toDouble() : x.toInt32(),
                                     y.isDouble() ? y.toDouble() : y.toInt32());
}

ActionHelper::Action* ActionHelper::makeRotateByAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), x(cx), y(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeRotateByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "x", &x);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeRotateByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "y", &y);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeRotateByAction : Error processing arguments");
    return cocos2d::RotateBy::create(
                                     duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                     x.isDouble() ? x.toDouble() : x.toInt32(),
                                     y.isDouble() ? y.toDouble() : y.toInt32());
}

ActionHelper::Action* ActionHelper::makeTintToAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), r(cx), g(cx), b(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeTintToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "r", &r);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeTintToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "g", &g);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeTintToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "b", &b);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeTintToAction : Error processing arguments");
    return cocos2d::TintTo::create(
                                   duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                   r.toInt32(), g.toInt32(), b.toInt32());
}

ActionHelper::Action* ActionHelper::makeTintByAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), r(cx), g(cx), b(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeTintByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "r", &r);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeTintByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "g", &g);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeTintByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "b", &b);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeTintByAction : Error processing arguments");
    return cocos2d::TintBy::create(
                                   duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                   r.toInt32(), g.toInt32(), b.toInt32());
}

ActionHelper::Action* ActionHelper::makeJumpToAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), x(cx), y(cx), height(cx), times(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "x", &x);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "y", &y);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "height", &height);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpToAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "times", &times);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpToAction : Error processing arguments");
    return cocos2d::JumpTo::create(
                                   duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                   cocos2d::Vec2(x.isDouble() ? x.toDouble() : x.toInt32(), y.isDouble() ? y.toDouble() : y.toInt32()),
                                   height.isDouble() ? height.toDouble() : height.toInt32(), times.toInt32());
}

ActionHelper::Action* ActionHelper::makeJumpByAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), x(cx), y(cx), height(cx), times(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "x", &x);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "y", &y);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "height", &height);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpByAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "times", &times);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeJumpByAction : Error processing arguments");
    return cocos2d::JumpBy::create(
                                   duration.isDouble() ? duration.toDouble() : duration.toInt32(),
                                   cocos2d::Vec2(x.isDouble() ? x.toDouble() : x.toInt32(), y.isDouble() ? y.toDouble() : y.toInt32()),
                                   height.isDouble() ? height.toDouble() : height.toInt32(), times.toInt32());
}

ActionHelper::Action* ActionHelper::makeFadeInAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeFadeInAction : Error processing arguments");
    return cocos2d::FadeIn::create(duration.isDouble() ? duration.toDouble() : duration.toInt32());
}

ActionHelper::Action* ActionHelper::makeFadeOutAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeFadeOutAction : Error processing arguments");
    return cocos2d::FadeOut::create(duration.isDouble() ? duration.toDouble() : duration.toInt32());
}

ActionHelper::Action* ActionHelper::makeEaseElasticInAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue action(cx), period(cx);
    ok &= JS_GetProperty(cx, root, "period", &period);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeEaseElasticInAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "action", &action);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeEaseElasticInAction : Error processing arguments");
    if (!action.isObject()) {
        return nullptr;
    }
    JS::RootedObject actionobj(cx, &action.toObject());
    ActionHelper::Action* actionPtr = makeAction(cx, actionobj);
    if (actionPtr != nullptr) {
        auto action = dynamic_cast<cocos2d::ActionInterval*>(actionPtr);
        if (action != nullptr) {
            return cocos2d::EaseElasticIn::create(action, period.isDouble() ? period.toDouble() : period.toInt32());
        }
    }
    return nullptr;
}

ActionHelper::Action* ActionHelper::makeEaseElasticOutAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue action(cx), period(cx);
    ok &= JS_GetProperty(cx, root, "period", &period);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeEaseElasticOutAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "action", &action);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeEaseElasticOutAction : Error processing arguments");
    if (!action.isObject()) {
        return nullptr;
    }
    JS::RootedObject actionobj(cx, &action.toObject());
    ActionHelper::Action* actionPtr = makeAction(cx, actionobj);
    if (actionPtr != nullptr) {
        auto action = dynamic_cast<cocos2d::ActionInterval*>(actionPtr);
        if (action != nullptr) {
            return cocos2d::EaseElasticOut::create(action, period.isDouble() ? period.toDouble() : period.toInt32());
        }
    }
    return nullptr;
}

ActionHelper::Action* ActionHelper::makeBlinkAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx), times(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeBlinkAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "times", &times);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeBlinkAction : Error processing arguments");
    return cocos2d::Blink::create(duration.isDouble() ? duration.toDouble() : duration.toInt32(), times.toInt32());
}

ActionHelper::Action* ActionHelper::makeDelayAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue duration(cx);
    ok &= JS_GetProperty(cx, root, "duration", &duration);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeDelayAction : Error processing arguments");
    return cocos2d::DelayTime::create(duration.isDouble() ? duration.toDouble() : duration.toInt32());
}

ActionHelper::Action* ActionHelper::makeRepeatAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue action(cx), times(cx);
    ok &= JS_GetProperty(cx, root, "times", &times);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeRepeatAction : Error processing arguments");
    ok &= JS_GetProperty(cx, root, "action", &action);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeRepeatAction : Error processing arguments");
    if (!action.isObject()) {
        return nullptr;
    }
    int32_t _times = times.toInt32();
    if (_times < 0) {
        return nullptr;
    }
    JS::RootedObject actionobj(cx, &action.toObject());
    ActionHelper::Action* actionPtr = makeAction(cx, actionobj);
    if (actionPtr != nullptr) {
        if (_times == 0) {
            auto action = dynamic_cast<cocos2d::ActionInterval*>(actionPtr);
            if (action != nullptr) {
                return cocos2d::RepeatForever::create(action);
            }
        } else {
            return cocos2d::Repeat::create(actionPtr, _times);
        }
    }
    return nullptr;
}

ActionHelper::Action* ActionHelper::makeSequenceAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue actions(cx);
    ok &= JS_GetProperty(cx, root, "actions", &actions);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeSequenceAction : Error processing arguments");
    if (!JS_IsArrayObject(cx, actions)) {
        return nullptr;
    }
    cocos2d::Vector<ActionHelper::Action*> actionArray;
    JS::RootedObject obj(cx, &actions.toObject());
    uint32_t actionSize = 0;
    ok &= JS_GetArrayLength(cx, obj, &actionSize);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeSequenceAction : Error processing arguments");
    for (int i=0; i < actionSize; i++) {
        JS::RootedValue element(cx);
        ok &= JS_GetElement(cx, obj, i, &element);
        JSB_PRECONDITION2(ok, cx, nullptr, "makeSequenceAction : Error processing arguments");
        if (!element.isObject()) {
            break;
        }
        JS::RootedObject temp(cx, &element.toObject());
        auto action = makeAction(cx, temp);
        if (action != nullptr) {
            actionArray.pushBack(action);
        } else {
            break;
        }
    }
    return cocos2d::Sequence::create(actionArray);
}

ActionHelper::Action* ActionHelper::makeCallbackAction(JSContext* cx, JS::HandleObject root) {
    bool ok = true;
    JS::RootedValue code(cx);
    ok &= JS_GetProperty(cx, root, "code", &code);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeCallbackAction : Error processing arguments");
    std::string codeStr;
    ok &= jsval_to_std_string(cx, code, &codeStr);
    JSB_PRECONDITION2(ok, cx, nullptr, "makeCallbackAction : Error processing arguments");
    auto jsengine = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name)->getJSEngine();
    return JSCallbackAction::create(jsengine, codeStr);
}

NS_BF_END
