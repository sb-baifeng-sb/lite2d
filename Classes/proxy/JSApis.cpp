//
//  JSApis.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/13.
//

#include "JSApis.hpp"
#include "StoryScriptProxy.hpp"
#include "ResProxy.hpp"
#include "MediaProxy.hpp"
#include "SaveProxy.hpp"
#include "BacklogProxy.hpp"
#include "KvProxy.hpp"
#include "DragonBonesFactory.hpp"
#include "tools/jsengine.hpp"
#include "mediator/StageMediator.hpp"
#include "app/App.hpp"
#include "view/SpriteStackView.hpp"
#include "view/BFImageView.hpp"
#include "view/ButtonView.hpp"
#include "view/GifView.hpp"
#include "view/DragonBonesView.hpp"
#include "view/TextView.hpp"
#include "view/SpeakView.hpp"
#include "view/MovieView.hpp"
#include "view/CcbUiView.hpp"
#include "view/UI/GameDataMgrView.hpp"
#include "view/UI/CGLibraryView.hpp"
#include "view/UI/MusicLibraryView.hpp"
#include "view/UI/MovieLibraryView.hpp"
#include "view/UI/GameOptionView.hpp"
#include "view/UI/GameMenuView.hpp"
#include "common/actions.hpp"

NS_BF_BEGIN

#if JSB_ASSERT_ON_FAIL
#define JSB_PRECONDITION( condition, error_msg) do { NSCAssert( condition, [NSString stringWithUTF8String:error_msg] ); } while(0)
#define JSB_PRECONDITION2( condition, context, ret_value, error_msg) do { NSCAssert( condition, [NSString stringWithUTF8String:error_msg] ); } while(0)
#define ASSERT( condition, error_msg) do { NSCAssert( condition, [NSString stringWithUTF8String:error_msg] ); } while(0)

#else
#define JSB_PRECONDITION( condition, ...) do {                          \
    if( ! (condition) ) {                                                       \
        JSEngine::Log("jsb: ERROR: File %s: Line: %d, Function: %s", __FILE__, __LINE__, __FUNCTION__ );         \
        JSEngine::Log(__VA_ARGS__);                                        \
        JSContext* globalContext = ScriptingCore::getInstance()->getGlobalContext();    \
        if( ! JS_IsExceptionPending( globalContext ) ) {                        \
            JS_ReportError( globalContext, __VA_ARGS__ );                           \
        }                                                                       \
        return false;                                                       \
    }                                                                           \
} while(0)
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
#define ASSERT( condition, error_msg) do {                                      \
    if( ! (condition) ) {                                                       \
        CCLOG("jsb: ERROR in %s: %s\n", __FUNCTION__, error_msg);               \
        return false;                                                           \
    }                                                                           \
} while(0)
#endif

bool js_print(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_print : Error processing arguments");
    JSEngine::Log("%s", arg0.c_str());
    return true;
}

bool js_test_obj(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    
    JS::RootedObject obj(cx, args.get(0).toObjectOrNull());
    JS::RootedValue a(cx), b(cx);
    
    bool ok = true;
    ok &= JS_GetProperty(cx, obj, "a", &a);
    JSB_PRECONDITION2(ok, cx, false, "js_test_obj : Error processing arguments");
    ok &= JS_GetProperty(cx, obj, "b", &b);
    JSB_PRECONDITION2(ok, cx, false, "js_test_obj : Error processing arguments");
    
    int32_t _a = a.toInt32();
    std::string _b;
    ok &= jsval_to_std_string(cx, b, &_b);
    JSB_PRECONDITION2(ok, cx, false, "js_test_obj : Error processing arguments");
    
    auto newobj = NewObject(cx);
    JS::RootedObject ret(cx, newobj);
    JS::RootedValue c(cx), d(cx);
    
    c.set(INT_TO_JSVAL(200));
    std::string str = "汉字";
    auto jsengine = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name)->getJSEngine();
    d.set(std_string_to_jsval(cx, str, jsengine->getJSContext(), jsengine->getJSGlobal()));
    
    ok &= JS_SetProperty(cx, ret, "c", c);
    JSB_PRECONDITION2(ok, cx, false, "js_test_obj : Error processing arguments");
    ok &= JS_SetProperty(cx, ret, "d", d);
    JSB_PRECONDITION2(ok, cx, false, "js_test_obj : Error processing arguments");
    
    args.rval().set(OBJECT_TO_JSVAL(ret.get()));
    return true;
}

bool js_window_size(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    auto newobj = NewObject(cx);
    JS::RootedObject ret(cx, newobj);
    JS::RootedValue w(cx), h(cx);
    auto director = cocos2d::Director::getInstance();
    auto const size = director->getWinSize();
    bool ok = true;
    w.set(INT_TO_JSVAL(size.width));
    h.set(INT_TO_JSVAL(size.height));
    ok &= JS_SetProperty(cx, ret, "w", w);
    JSB_PRECONDITION2(ok, cx, false, "js_window_size : Error processing arguments");
    ok &= JS_SetProperty(cx, ret, "h", h);
    JSB_PRECONDITION2(ok, cx, false, "js_window_size : Error processing arguments");
    args.rval().set(OBJECT_TO_JSVAL(ret.get()));
    return true;
}

bool js_app_exit(JSContext *cx, uint32_t argc, jsval *vp) {
    exit(0);
    return true;
}

bool js_do_script(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_do_script : Error processing arguments");
    auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    proxy->doScript(arg0);
    return true;
}

bool js_script_mode(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 1) {
        bool ok = true;
        int32_t arg0;
        ok &= jsval_to_int32(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_script_mode : Error processing arguments");
        auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        proxy->setMode((StoryScriptProxy::ScriptMode)arg0);
    } else {
        auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        auto mode = proxy->getMode();
        args.rval().set(INT_TO_JSVAL((int32_t)mode));
    }
    return true;
}

bool js_run_story_script(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_run_story_script : Error processing arguments");
    auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    proxy->runScript(arg0);
    return true;
}

bool js_jump_story_script(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_jump_story_script : Error processing arguments");
    auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    proxy->jumpScript(arg0);
    return true;
}

bool js_pause_story_script(JSContext *cx, uint32_t argc, jsval *vp) {
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        auto arg0 = args.get(0);
        proxy->pause(arg0.isDouble() ? arg0.toDouble() : arg0.toInt32());
        return true;
    }
    return false;
}

bool js_back_story_script(JSContext *cx, uint32_t argc, jsval *vp) {
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        auto arg0 = args.get(0);
        proxy->back_step(arg0.isDouble() ? arg0.toDouble() : arg0.toInt32());
        return true;
    } else {
        auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        proxy->back_step(1);
        return true;
    }
    return false;
}

bool js_exit_story_script(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    proxy->popScript();
    auto ret = INT_TO_JSVAL(proxy->getStackSize());
    args.rval().set(ret);
    return true;
}

bool js_goto_story_tag(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_goto_story_tag : Error processing arguments");
    auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    proxy->seek(arg0);
    return true;
}

bool js_goto_story_tag_if(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg1;
    if (args.get(0).isBoolean() && args.get(0).toBoolean()) {
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_gotoif_story_tag : Error processing arguments");
        auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        proxy->seek(arg1);
        return true;
    }
    return true;
}

bool js_do_code_if(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg1;
    if (args.get(0).isBoolean() && args.get(0).toBoolean()) {
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_do_code_if : Error processing arguments");
        auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        proxy->doCode(arg1);
        return true;
    }
    return true;
}

bool js_script_start(JSContext *cx, uint32_t argc, jsval *vp) {
    auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    script->setPlayTime(0);
    return true;
}

bool js_script_chapter(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_script_chapter : Error processing arguments");
        auto save = App::getProxy<SaveProxy>(SaveProxy::Name);
        save->setChapter(arg0);
    } else if (argc == 0) {
        auto save = App::getProxy<SaveProxy>(SaveProxy::Name);
        args.rval().set(std_string_to_jsval(cx, save->getChapter()));
    }
    return true;
}

// ======================================================================================

bool js_stage_push(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    uint32_t arg1;
    ok &= jsval_to_uint32(cx, args.get(0), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_push_stage : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    mediator->pushSpriteStack(arg1);
    return true;
}

bool js_stage_pop(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    mediator->popSpriteStack();
    auto ret = INT_TO_JSVAL(mediator->getSpriteStackSize());
    args.rval().set(ret);
    return true;
}

bool js_stage_all_clear(JSContext *cx, uint32_t argc, jsval *vp) {
    
    auto free = false;
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    if (argc == 0) {
        auto view = mediator->topSpriteStack();
        if (view != nullptr) {
            view->removeAllSprite();
            free = true;
        }
    } else if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        bool ok = true;
        int32_t arg0;
        ok &= jsval_to_int32(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_stage_all_clear : Error processing arguments");
        auto view = mediator->getSpriteStack(arg0);
        if (view != nullptr) {
            view->removeAllSprite();
            free = true;
        }
    }
    
    if (mediator->getSpriteStackSize() <= 1 && free) {
        // 移除骨骼动画缓存
        auto dbfactory = DragonBonesFactory::getInstance();
        dbfactory->clear(true);
        // 移除贴图缓存
        auto director = cocos2d::Director::getInstance();
        director->getTextureCache()->removeUnusedTextures();
    }
    return true;
}

bool js_stage_pos(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_stage_pos : Error processing arguments");
    
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    auto storyView = mediator->getSpriteStack(arg0);
    if (argc == 1) {
        // 返回
        if (storyView != nullptr) {
            auto pos = storyView->getPosition();
            auto newobj = NewObject(cx);
            JS::RootedObject ret(cx, newobj);
            JS::RootedValue x(cx), y(cx);
            x.set(DOUBLE_TO_JSVAL(pos.x));
            y.set(DOUBLE_TO_JSVAL(pos.y));
            ok &= JS_SetProperty(cx, ret, "x", x);
            ok &= JS_SetProperty(cx, ret, "y", y);
            JSB_PRECONDITION2(ok, cx, false, "js_stage_pos : Error processing arguments");
            args.rval().set(OBJECT_TO_JSVAL(ret.get()));
        }
    } else if (argc == 3) {
        // 设置
        float arg1, arg2;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        ok &= jsval_to_float(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_stage_pos : Error processing arguments");
        if (storyView != nullptr) {
            storyView->setPosition(arg1, arg2);
        }
    }
    return true;
}

bool js_stage_anchor(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_stage_anchor : Error processing arguments");
    
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    auto storyView = mediator->getSpriteStack(arg0);
    if (argc == 1) {
        // 返回
        if (storyView != nullptr) {
            auto pos = storyView->getAnchorPoint();
            auto newobj = NewObject(cx);
            JS::RootedObject ret(cx, newobj);
            JS::RootedValue x(cx), y(cx);
            x.set(DOUBLE_TO_JSVAL(pos.x));
            y.set(DOUBLE_TO_JSVAL(pos.y));
            ok &= JS_SetProperty(cx, ret, "x", x);
            ok &= JS_SetProperty(cx, ret, "y", y);
            JSB_PRECONDITION2(ok, cx, false, "js_stage_anchor : Error processing arguments");
            args.rval().set(OBJECT_TO_JSVAL(ret.get()));
        }
    } else if (argc == 3) {
        // 设置
        float arg1, arg2;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        ok &= jsval_to_float(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_stage_anchor : Error processing arguments");
        if (storyView != nullptr) {
            storyView->setAnchorPoint(cocos2d::Vec2(arg1, arg2));
        }
    }
    return true;
}

bool js_stage_scale(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_stage_scale : Error processing arguments");
    
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    auto storyView = mediator->getSpriteStack(arg0);
    if (argc == 1) {
        // 返回
        if (storyView != nullptr) {
            auto pos = cocos2d::Vec2(storyView->getScaleX(), storyView->getScaleY());
            auto newobj = NewObject(cx);
            JS::RootedObject ret(cx, newobj);
            JS::RootedValue x(cx), y(cx);
            x.set(DOUBLE_TO_JSVAL(pos.x));
            y.set(DOUBLE_TO_JSVAL(pos.y));
            ok &= JS_SetProperty(cx, ret, "x", x);
            ok &= JS_SetProperty(cx, ret, "y", y);
            JSB_PRECONDITION2(ok, cx, false, "js_stage_scale : Error processing arguments");
            args.rval().set(OBJECT_TO_JSVAL(ret.get()));
        }
    } else if (argc == 3) {
        // 设置
        float arg1, arg2;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        ok &= jsval_to_float(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_stage_scale : Error processing arguments");
        if (storyView != nullptr) {
            storyView->setScale(arg1, arg2);
        }
    } else if (argc == 2) {
        // 设置
        float arg1;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_stage_scale : Error processing arguments");
        if (storyView != nullptr) {
            storyView->setScale(arg1);
        }
    }
    return true;
}

bool js_stage_rotate(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_stage_rotate : Error processing arguments");
    
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    auto storyView = mediator->getSpriteStack(arg0);
    if (argc == 1) {
        // 返回
        if (storyView != nullptr) {
            auto rotate = storyView->getRotation();
            args.rval().set(DOUBLE_TO_JSVAL(rotate));
        }
    } else if (argc == 2) {
        // 设置
        float arg1;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_stage_rotate : Error processing arguments");
        if (storyView != nullptr) {
            storyView->setRotation(arg1);
        }
    }
    return true;
}

bool js_stage_show(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_stage_show : Error processing arguments");
    
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    auto storyView = mediator->getSpriteStack(arg0);
    if (argc == 1) {
        // 返回
        if (storyView != nullptr) {
            args.rval().set(BOOLEAN_TO_JSVAL(storyView->isVisible()));
        }
    } else if (argc == 2) {
        // 设置
        if (storyView != nullptr && args.get(1).isBoolean()) {
            storyView->setVisible(args.get(1).toBoolean());
        }
    }
    return true;
}

bool js_stage_action(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_stage_action : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    auto storyView = mediator->getSpriteStack(arg0);
    if (storyView != nullptr) {
        if (args.get(1).isObject()) {
            // run action
            JS::RootedObject obj(cx, args.get(1).toObjectOrNull());
            auto action = ActionHelper::makeAction(cx, obj);
            if (action != nullptr) {
                storyView->runAction(action);
            }
        } else {
            // stop action
            storyView->stopAllActions();
        }
    }
    return true;
}

bool js_stage_num(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    auto ret = INT_TO_JSVAL(mediator->getSpriteStackSize());
    args.rval().set(ret);
    return true;
}

// ======================================================================================

bool js_sprite_load(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    std::string arg1;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    ok &= jsval_to_std_string(cx, args.get(1), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_load : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    if (mediator && mediator->topSpriteStack()) {
        auto storyView = mediator->topSpriteStack();
        return storyView->loadSprite(arg0, arg1, cx, argc, args);
    }
    return false;
}

bool js_sprite_null(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_null : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    if (mediator && mediator->topSpriteStack()) {
        auto storyView = mediator->topSpriteStack();
        storyView->setSprite(arg0, nullptr);
    }
    return true;
}

bool js_sprite_play(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_play : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    if (mediator && mediator->topSpriteStack()) {
        auto storyView = mediator->topSpriteStack();
        if (storyView->isSpriteType(arg0, AvgSprite::TYPE_GIF)) {
            int32_t arg1;
            ok &= jsval_to_int32(cx, args.get(1), &arg1);
            JSB_PRECONDITION2(ok, cx, false, "js_sprite_play : Error processing arguments");
            auto view = dynamic_cast<GifView*>(storyView->getSpriteAsNode(arg0));
            view->play(arg1);
        } else if (storyView->isSpriteType(arg0, AvgSprite::TYPE_DB)) {
            std::string arg1;
            int32_t arg2;
            ok &= jsval_to_std_string(cx, args.get(1), &arg1);
            ok &= jsval_to_int32(cx, args.get(2), &arg2);
            JSB_PRECONDITION2(ok, cx, false, "js_sprite_play : Error processing arguments");
            auto view = dynamic_cast<DragonBonesView*>(storyView->getSpriteAsNode(arg0));
            view->play(arg1, arg2);
        } else if (storyView->isSpriteType(arg0, AvgSprite::TYPE_HALF_SPEAK)) {
            std::string arg1;
            float arg2;
            ok &= jsval_to_std_string(cx, args.get(1), &arg1);
            ok &= jsval_to_float(cx, args.get(2), &arg2);
            JSB_PRECONDITION2(ok, cx, false, "js_sprite_play : Error processing arguments");
            auto view = dynamic_cast<HalfSpeakView*>(storyView->getSpriteAsNode(arg0));
            view->setSpeed(arg2);
            view->setString(arg1);
        } else if (storyView->isSpriteType(arg0, AvgSprite::TYPE_CCB_UI)) {
            std::string arg1, arg2;
            ok &= jsval_to_std_string(cx, args.get(1), &arg1);
            ok &= jsval_to_std_string(cx, args.get(2), &arg2);
            JSB_PRECONDITION2(ok, cx, false, "js_sprite_play : Error processing arguments");
            auto view = dynamic_cast<CcbUiView*>(storyView->getSpriteAsNode(arg0));
            view->setAnimationEndCallbackCode(arg1, arg2);
            view->playAnimation(arg1);
        }
    }
    return true;
}

bool js_sprite_pos(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_pos : Error processing arguments");
    
    if (argc == 1) {
        // 返回
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                auto const pos = node->getPosition();
                auto newobj = NewObject(cx);
                JS::RootedObject ret(cx, newobj);
                JS::RootedValue x(cx), y(cx);
                x.set(DOUBLE_TO_JSVAL(pos.x));
                y.set(DOUBLE_TO_JSVAL(pos.y));
                ok &= JS_SetProperty(cx, ret, "x", x);
                ok &= JS_SetProperty(cx, ret, "y", y);
                JSB_PRECONDITION2(ok, cx, false, "js_sprite_pos : Error processing arguments");
                args.rval().set(OBJECT_TO_JSVAL(ret.get()));
            }
        }
    } else if (argc == 3) {
        // 设置
        float arg1, arg2;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        ok &= jsval_to_float(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_sprite_pos : Error processing arguments");
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->setPosition(arg1, arg2);
            }
        }
    }
    return true;
}

bool js_sprite_anchor(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_anchor : Error processing arguments");
    
    if (argc == 1) {
        // 返回
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                auto const pos = node->getAnchorPoint();
                auto newobj = NewObject(cx);
                JS::RootedObject ret(cx, newobj);
                JS::RootedValue x(cx), y(cx);
                x.set(DOUBLE_TO_JSVAL(pos.x));
                y.set(DOUBLE_TO_JSVAL(pos.y));
                ok &= JS_SetProperty(cx, ret, "x", x);
                ok &= JS_SetProperty(cx, ret, "y", y);
                JSB_PRECONDITION2(ok, cx, false, "js_sprite_anchor : Error processing arguments");
                args.rval().set(OBJECT_TO_JSVAL(ret.get()));
            }
        }
    } else if (argc == 3) {
        // 设置
        float arg1, arg2;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        ok &= jsval_to_float(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_sprite_anchor : Error processing arguments");
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->setAnchorPoint(cocos2d::Vec2(arg1, arg2));
            }
        }
    }
    return true;
}

bool js_sprite_scale(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_scale : Error processing arguments");
    
    if (argc == 1) {
        // 返回
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                auto pos = cocos2d::Vec2(node->getScaleX(), node->getScaleY());
                auto newobj = NewObject(cx);
                JS::RootedObject ret(cx, newobj);
                JS::RootedValue x(cx), y(cx);
                x.set(DOUBLE_TO_JSVAL(pos.x));
                y.set(DOUBLE_TO_JSVAL(pos.y));
                ok &= JS_SetProperty(cx, ret, "x", x);
                ok &= JS_SetProperty(cx, ret, "y", y);
                JSB_PRECONDITION2(ok, cx, false, "js_sprite_scale : Error processing arguments");
                args.rval().set(OBJECT_TO_JSVAL(ret.get()));
            }
        }
    } else if (argc == 3) {
        // 设置
        float arg1, arg2;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        ok &= jsval_to_float(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_sprite_scale : Error processing arguments");
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->setScale(arg1, arg2);
            }
        }
    } else if (argc == 2) {
        // 设置
        float arg1;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_stage_scale : Error processing arguments");
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->setScale(arg1);
            }
        }
    }
    return true;
}

bool js_sprite_rotate(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_scale : Error processing arguments");
    
    if (argc == 1) {
        // 返回
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                auto rotate = node->getRotation();
                args.rval().set(DOUBLE_TO_JSVAL(rotate));
            }
        }
    } else if (argc == 2) {
        // 设置
        float arg1;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_sprite_scale : Error processing arguments");
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->setRotation(arg1);
            }
        }
    }
    return true;
}

bool js_sprite_color(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_color : Error processing arguments");
    
    if (argc == 1) {
        // 返回
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                auto newobj = NewObject(cx);
                JS::RootedObject ret(cx, newobj);
                JS::RootedValue r(cx), g(cx), b(cx);
                auto color = node->getColor();
                r.set(UINT_TO_JSVAL(color.r));
                g.set(UINT_TO_JSVAL(color.g));
                b.set(UINT_TO_JSVAL(color.b));
                ok &= JS_SetProperty(cx, ret, "r", r);
                ok &= JS_SetProperty(cx, ret, "g", g);
                ok &= JS_SetProperty(cx, ret, "b", b);
                JSB_PRECONDITION2(ok, cx, false, "js_sprite_color : Error processing arguments");
                args.rval().set(OBJECT_TO_JSVAL(ret.get()));
            }
        }
    } else if (argc == 4) {
        // 设置
        int arg1, arg2, arg3;
        ok &= jsval_to_int32(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), &arg2);
        ok &= jsval_to_int32(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_sprite_color : Error processing arguments");
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->setColor(cocos2d::Color3B(arg1, arg2, arg3));
            }
        }
    }
    return true;
}

bool js_sprite_alpha(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_alpha : Error processing arguments");
    
    if (argc == 1) {
        // 返回
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                args.rval().set(UINT_TO_JSVAL(node->getOpacity()));
            }
        }
    } else if (argc == 2) {
        // 设置
        int arg1;
        ok &= jsval_to_int32(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_sprite_alpha : Error processing arguments");
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->setOpacity(arg1);
            }
        }
    }
    return true;
}

bool js_sprite_size(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_size : Error processing arguments");
    
    if (argc == 1) {
        // 返回
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                auto size = node->getContentSize();
                auto newobj = NewObject(cx);
                JS::RootedObject ret(cx, newobj);
                JS::RootedValue w(cx), h(cx);
                w.set(DOUBLE_TO_JSVAL(size.width));
                h.set(DOUBLE_TO_JSVAL(size.height));
                ok &= JS_SetProperty(cx, ret, "w", w);
                ok &= JS_SetProperty(cx, ret, "h", h);
                JSB_PRECONDITION2(ok, cx, false, "js_sprite_size : Error processing arguments");
                args.rval().set(OBJECT_TO_JSVAL(ret.get()));
            }
        }
    } else if (argc == 3) {
        // 设置
        float arg1, arg2;
        ok &= jsval_to_float(cx, args.get(1), &arg1);
        ok &= jsval_to_float(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_sprite_size : Error processing arguments");
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->setContentSize(cocos2d::Size(arg1, arg2));
            }
        }
    }
    return true;
}

bool js_sprite_show(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_show : Error processing arguments");
    
    if (argc == 1) {
        // 返回
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                args.rval().set(BOOLEAN_TO_JSVAL(node->isVisible()));
            }
        }
    } else if (argc == 2) {
        // 设置
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        if (mediator && mediator->topSpriteStack()) {
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr && args.get(1).isBoolean()) {
                node->setVisible(args.get(1).toBoolean());
            }
        }
    }
    return true;
}

bool js_sprite_mov(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0, arg1;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    ok &= jsval_to_int32(cx, args.get(1), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_mov : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    if (mediator && mediator->topSpriteStack()) {
        auto storyView = mediator->topSpriteStack();
        storyView->movSprite(arg0, arg1);
    }
    return true;
}

bool js_sprite_cpy(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0, arg1;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    ok &= jsval_to_int32(cx, args.get(1), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_cpy : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    if (mediator && mediator->topSpriteStack()) {
        auto storyView = mediator->topSpriteStack();
        storyView->cpySprite(arg0, arg1);
    }
    return true;
}

bool js_sprite_swap(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0, arg1;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    ok &= jsval_to_int32(cx, args.get(1), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_swap : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    if (mediator && mediator->topSpriteStack()) {
        auto storyView = mediator->topSpriteStack();
        storyView->swapSprite(arg0, arg1);
    }
    return true;
}

bool js_sprite_action(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int32_t arg0;
    ok &= jsval_to_int32(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_sprite_action : Error processing arguments");
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    if (mediator && mediator->topSpriteStack()) {
        if (args.get(1).isObject()) {
            // run action
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                JS::RootedObject obj(cx, args.get(1).toObjectOrNull());
                auto action = ActionHelper::makeAction(cx, obj);
                if (action != nullptr) {
                    node->runAction(action);
                }
            }
        } else {
            // stop action
            auto storyView = mediator->topSpriteStack();
            auto node = storyView->getSpriteAsNode(arg0);
            if (node != nullptr) {
                node->stopAllActions();
            }
        }
    }
    return true;
}

// ======================================================================================

bool js_music_play(JSContext *cx, uint32_t argc, jsval *vp) {
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        bool ok = true;
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_play_music : Error processing arguments");
        auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
        proxy->playMusic(arg0);
    }
    return true;
}

bool js_music_stop(JSContext *cx, uint32_t argc, jsval *vp) {
    auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
    proxy->stopMusic();
    return true;
}

bool js_music_pause(JSContext *cx, uint32_t argc, jsval *vp) {
    auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
    proxy->pauseMusic();
    return true;
}

bool js_music_resume(JSContext *cx, uint32_t argc, jsval *vp) {
    auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
    proxy->resumeMusic();
    return true;
}

bool js_music_set_vol(JSContext *cx, uint32_t argc, jsval *vp) {
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        bool ok = true;
        int32_t arg0;
        ok &= jsval_to_int32(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_set_music_vol : Error processing arguments");
        auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
        proxy->setMusicVol(arg0);
    }
    return true;
}

bool js_music_get_vol(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
    args.rval().set(INT_TO_JSVAL(proxy->getMusicVol()));
    return true;
}

bool js_se_play(JSContext *cx, uint32_t argc, jsval *vp) {
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        bool ok = true;
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_play_se : Error processing arguments");
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        if (script->getMode() == StoryScriptProxy::MODE_SKIP) {
            // 快进中不播放语音
            args.rval().set(INT_TO_JSVAL(0));
            return true;
        }
        auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
        auto soundId = proxy->playEffect(arg0);
        args.rval().set(INT_TO_JSVAL(soundId));
    }
    return true;
}

bool js_se_stop(JSContext *cx, uint32_t argc, jsval *vp) {
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        bool ok = true;
        int32_t arg0;
        ok &= jsval_to_int32(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_stop_se : Error processing arguments");
        auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
        proxy->stopEffect(arg0);
    }
    return true;
}

bool js_se_set_vol(JSContext *cx, uint32_t argc, jsval *vp) {
    if (argc == 1) {
        JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
        bool ok = true;
        int32_t arg0;
        std::string arg1;
        ok &= jsval_to_int32(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_set_se_vol : Error processing arguments");
        auto proxy = App::getProxy<MediaProxy>(MediaProxy::Name);
        proxy->setEffectVol(arg0);
    }
    return true;
}

bool js_video_play(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_video_play : Error processing arguments");
    MovieView::PlayMovie(arg0);
    return true;
}

// ======================================================================================

bool js_load_ui_savedata(JSContext *cx, uint32_t argc, jsval *vp) {
    //JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    auto view = GameDataMgrView::nodeFrom("system/data_manager");
    view->start(GameDataMgrView::TYPE_SAVE);
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
    view->setExitFunc([&]() {
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->startUpdate();
    });
    auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    script->stopUpdate();
    return true;
}

bool js_load_ui_loaddata(JSContext *cx, uint32_t argc, jsval *vp) {
    //JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    auto view = GameDataMgrView::nodeFrom("system/data_manager");
    view->start(GameDataMgrView::TYPE_LOAD);
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
    view->setExitFunc([&]() {
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->startUpdate();
    });
    auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    script->stopUpdate();
    return true;
}

bool js_load_ui_cg_library(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedValue array(cx);
    array.setObjectOrNull(args.get(0).toObjectOrNull());
    if (!JS_IsArrayObject(cx, array)) {
        return false;
    }
    
    uint32_t length = 0;
    JS::RootedObject obj(cx, args.get(0).toObjectOrNull());
    JS_GetArrayLength(cx, obj, &length);
    
    std::string iconStr;
    std::string imgFile;
    int32_t imgSize = 0;
    CGLibraryView::CgInfoArray infoArray;
    infoArray.reserve(length);
    for (int i=0; i < (int)length; i++) {
        CGLibraryView::CgInfo info;
        JS::RootedValue temp(cx);
        JS_GetElement(cx, obj, i, &temp);
        bool ok = true;
        JS::RootedObject tempObj(cx, temp.toObjectOrNull());
        JS::RootedValue icon(cx), size(cx), images(cx);
        ok &= JS_GetProperty(cx, tempObj, "icon", &icon);
        ok &= JS_GetProperty(cx, tempObj, "size", &size);
        ok &= JS_GetProperty(cx, tempObj, "images", &images);
        JSB_PRECONDITION2(ok, cx, false, "js_load_ui_cg_library : Error processing arguments");
        ok &= jsval_to_std_string(cx, icon, &iconStr);
        ok &= jsval_to_int32(cx, size, &imgSize);
        JSB_PRECONDITION2(ok, cx, false, "js_load_ui_cg_library : Error processing arguments");
        
        uint32_t imgLength = 0;
        JS::RootedObject imagesArr(cx, images.toObjectOrNull());
        JS::RootedValue imagePath(cx);
        JS_GetArrayLength(cx, imagesArr, &imgLength);
        for (int j=0; j < imgLength; j++) {
            ok &= JS_GetElement(cx, imagesArr, j, &imagePath);
            JSB_PRECONDITION2(ok, cx, false, "js_load_ui_cg_library : Error processing arguments");
            ok &= jsval_to_std_string(cx, imagePath, &imgFile);
            JSB_PRECONDITION2(ok, cx, false, "js_load_ui_cg_library : Error processing arguments");
            info.ImgArr.push_back(imgFile);
        }
        
        info.ImgArrMaxSize = imgSize;
        info.ImgIcon = iconStr;
        info.Unlock = info.ImgArr.size() > 0;
        infoArray.push_back(info);
    }
    
    auto view = CGLibraryView::nodeFrom("system/cg_library");
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
    view->setSourceData(infoArray);
    return true;
}

bool js_load_ui_music_library(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedValue array(cx);
    array.setObjectOrNull(args.get(0).toObjectOrNull());
    if (!JS_IsArrayObject(cx, array)) {
        return false;
    }
    
    uint32_t length = 0;
    JS::RootedObject obj(cx, args.get(0).toObjectOrNull());
    JS_GetArrayLength(cx, obj, &length);
    
    std::string titleStr, fileStr;
    MusicLibraryView::MusicInfoArray infoArray;
    infoArray.reserve(length);
    for (int i=0; i < (int)length; i++) {
        JS::RootedValue temp(cx);
        JS_GetElement(cx, obj, i, &temp);
        bool ok = true;
        JS::RootedObject tempObj(cx, temp.toObjectOrNull());
        JS::RootedValue title(cx), file(cx), unlock(cx);
        ok &= JS_GetProperty(cx, tempObj, "title", &title);
        ok &= JS_GetProperty(cx, tempObj, "file", &file);
        ok &= JS_GetProperty(cx, tempObj, "unlock", &unlock);
        JSB_PRECONDITION2(ok, cx, false, "js_load_ui_music_library : Error processing arguments");
        ok &= jsval_to_std_string(cx, title, &titleStr);
        ok &= jsval_to_std_string(cx, file, &fileStr);
        JSB_PRECONDITION2(ok, cx, false, "js_load_ui_music_library : Error processing arguments");
        infoArray.push_back({
            unlock.toBoolean(),
            titleStr,
            fileStr,
        });
    }
    
    auto view = MusicLibraryView::nodeFrom("system/music_library");
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
    view->setSourceData(infoArray);
    return true;
}

bool js_load_ui_movie_library(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedValue array(cx);
    array.setObjectOrNull(args.get(0).toObjectOrNull());
    if (!JS_IsArrayObject(cx, array)) {
        return false;
    }
    
    uint32_t length = 0;
    JS::RootedObject obj(cx, args.get(0).toObjectOrNull());
    JS_GetArrayLength(cx, obj, &length);
    
    std::string iconStr, fileStr;
    MovieLibraryView::MovieInfoArray infoArray;
    infoArray.reserve(length);
    for (int i=0; i < (int)length; i++) {
        JS::RootedValue temp(cx);
        JS_GetElement(cx, obj, i, &temp);
        bool ok = true;
        JS::RootedObject tempObj(cx, temp.toObjectOrNull());
        JS::RootedValue icon(cx), file(cx), unlock(cx);
        ok &= JS_GetProperty(cx, tempObj, "icon", &icon);
        ok &= JS_GetProperty(cx, tempObj, "file", &file);
        ok &= JS_GetProperty(cx, tempObj, "unlock", &unlock);
        JSB_PRECONDITION2(ok, cx, false, "js_load_ui_movie_library : Error processing arguments");
        ok &= jsval_to_std_string(cx, icon, &iconStr);
        ok &= jsval_to_std_string(cx, file, &fileStr);
        JSB_PRECONDITION2(ok, cx, false, "js_load_ui_movie_library : Error processing arguments");
        infoArray.push_back({
            unlock.toBoolean(),
            iconStr,
            fileStr,
        });
    }
    
    auto view = MovieLibraryView::nodeFrom("system/movie_library");
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
    view->setSourceData(infoArray);
    return true;
}

bool js_load_ui_game_option(JSContext *cx, uint32_t argc, jsval *vp) {
    //JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    auto view = GameOptionView::nodeFrom("system/game_option");
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
    return true;
}

bool js_load_ui_game_menu(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_load_ui_game_menu : Error processing arguments");
    
    auto view = GameMenuView::nodeFrom("system/game_menu");
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    view->setTag(0xFF00ABCD);
    view->setBackTitleScript(arg0);
    scene->addChild(view);
    
    // 对游戏截图
    auto screen = App::getMediator<StageMediator>(StageMediator::Name);
    screen->getGameScreenShot("screenshot_save.png", [&](std::string const& pngPath) {
        auto scene = cocos2d::Director::getInstance()->getRunningScene();
        auto view = (GameMenuView*)scene->getChildByTag(0xFF00ABCD);
        if (view == nullptr) {
            return;
        }
        auto director = cocos2d::Director::getInstance();
        director->getTextureCache()->removeTextureForKey(pngPath);
        auto texture = director->getTextureCache()->addImage(pngPath);
        view->start(texture);
    });
    
    return true;
}

// ======================================================================================

bool js_backlog_insert_log(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0, arg1, arg2;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    ok &= jsval_to_std_string(cx, args.get(1), &arg1);
    ok &= jsval_to_std_string(cx, args.get(2), &arg2);
    JSB_PRECONDITION2(ok, cx, false, "js_backlog_insert_log : Error processing arguments");
    auto backlog = App::getProxy<BacklogProxy>(BacklogProxy::Name);
    backlog->insertLog(arg0, arg1, arg2);
    return true;
}

// ======================================================================================

template <class Type>
Type stringToNum(std::string const& str) {
    std::istringstream iss(str);
    Type num = 0;
    iss >> num;
    return num;
}

bool js_kv_set(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc != 2) {
        return false;
    }
    if (args.get(1).isObject()) {
        return false;
    }
    bool ok = true;
    std::string arg0, arg1;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    ok &= jsval_to_std_string(cx, args.get(1), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_kv_set : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    kv->setValue(arg0, arg1);
    return true;
}

bool js_kv_add(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc != 2) {
        return false;
    }
    if (args.get(1).isObject()) {
        return false;
    }
    bool ok = true;
    std::string arg0;
    int32_t arg1 = 0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    ok &= jsval_to_int32(cx, args.get(1), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_kv_add : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    auto& value = kv->getValue(arg0);
    if (value.length() == 0) {
        char temp[32] = {0};
        sprintf(temp, "%d", arg1);
        kv->setValue(arg0, temp);
        return true;
    }
    auto num = stringToNum<int32_t>(value);
    char temp[32] = {0};
    sprintf(temp, "%d", num + arg1);
    kv->setValue(arg0, temp);
    return true;
}

bool js_kv_get_string(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_kv_get_string : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    auto& value = kv->getValue(arg0);
    auto jsengine = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name)->getJSEngine();
    auto val = std_string_to_jsval(cx, value, jsengine->getJSContext(), jsengine->getJSGlobal());
    args.rval().set(val);
    return true;
}

bool js_kv_get_double(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_kv_get_double : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    auto& value = kv->getValue(arg0);
    if (value.length() == 0) {
        args.rval().setDouble(0.0f);
        return true;
    }
    auto ret = stringToNum<double>(value);
    args.rval().setDouble(ret);
    return true;
}

bool js_kv_get_int(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_kv_get_int : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    auto& value = kv->getValue(arg0);
    if (value.length() == 0) {
        args.rval().setInt32(0);
        return true;
    }
    auto ret = stringToNum<int32_t>(value);
    args.rval().setInt32(ret);
    return true;
}


bool js_kv_get_bool(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_kv_get_bool : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    auto& value = kv->getValue(arg0);
    args.rval().setBoolean(value == "true");
    return true;
}

bool js_kv_remove(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_kv_remove : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    kv->removeKey(arg0);
    return true;
}

bool js_kv_clear(JSContext *cx, uint32_t argc, jsval *vp) {
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    kv->clear();
    return true;
}

// ======================================================================================

bool js_pub_kv_set(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc != 2) {
        return false;
    }
    if (args.get(1).isObject()) {
        return false;
    }
    bool ok = true;
    std::string arg0, arg1;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    ok &= jsval_to_std_string(cx, args.get(1), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_pub_kv_set : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    kv->setValue(arg0, arg1);
    return true;
}

bool js_pub_kv_add(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc != 2) {
        return false;
    }
    if (args.get(1).isObject()) {
        return false;
    }
    bool ok = true;
    std::string arg0;
    int32_t arg1 = 0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    ok &= jsval_to_int32(cx, args.get(1), &arg1);
    JSB_PRECONDITION2(ok, cx, false, "js_pub_kv_add : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    auto& value = kv->getValue(arg0);
    if (value.length() == 0) {
        char temp[32] = {0};
        sprintf(temp, "%d", arg1);
        kv->setValue(arg0, temp);
        return true;
    }
    auto num = stringToNum<int32_t>(value);
    char temp[32] = {0};
    sprintf(temp, "%d", num + arg1);
    kv->setValue(arg0, temp);
    return true;
}

bool js_pub_kv_get_string(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_pub_kv_get_string : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    auto& value = kv->getValue(arg0);
    auto jsengine = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name)->getJSEngine();
    auto val = std_string_to_jsval(cx, value, jsengine->getJSContext(), jsengine->getJSGlobal());
    args.rval().set(val);
    return true;
}

bool js_pub_kv_get_double(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_pub_kv_get_double : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    auto& value = kv->getValue(arg0);
    if (value.length() == 0) {
        args.rval().setDouble(0.0f);
        return true;
    }
    auto ret = stringToNum<double>(value);
    args.rval().setDouble(ret);
    return true;
}

bool js_pub_kv_get_int(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_pub_kv_get_int : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    auto& value = kv->getValue(arg0);
    if (value.length() == 0) {
        args.rval().setInt32(0);
        return true;
    }
    auto ret = stringToNum<int32_t>(value);
    args.rval().setInt32(ret);
    return true;
}

bool js_pub_kv_get_bool(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_pub_kv_get_bool : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    auto& value = kv->getValue(arg0);
    args.rval().setBoolean(value == "true");
    return true;
}

bool js_pub_kv_remove(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_pub_kv_remove : Error processing arguments");
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    kv->removeKey(arg0);
    return true;
}

bool js_pub_kv_save(JSContext *cx, uint32_t argc, jsval *vp) {
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    kv->writeToFile(KvProxy::PublicKvPath);
    return true;
}

bool js_pub_kv_load(JSContext *cx, uint32_t argc, jsval *vp) {
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    kv->readFile(KvProxy::PublicKvPath);
    return true;
}

NS_BF_END
