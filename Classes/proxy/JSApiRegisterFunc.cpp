//
//  JSApiRegisterFunc.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/13.
//

#include "JSApiRegisterFunc.hpp"
#include "JSApis.hpp"
#include "tools/jsengine.hpp"
#include "cocos2d.h"

NS_BF_BEGIN

void register_funcs(JSContext* cx, JS::HandleObject global, JSFunctionSpec funcs[], std::string const& name) {
    std::string error = "register_story_api ";
    bool ok = true;
    auto newobj = NewObject(cx);
    JS::RootedObject obj(cx, newobj);
    ok &= JS_DefineFunctions(cx, obj, funcs);
    CCASSERT(ok, (error + name + " fail.").c_str());
    JS::RootedValue val(cx);
    val.setObject(*obj.get());
    ok &= JS_SetProperty(cx, global, name.c_str(), val);
    CCASSERT(ok, (error + name + " fail.").c_str());
}

void register_story_api(JSContext *cx, JS::HandleObject global) {
    
    static JSFunctionSpec app[] = {
        JS_FS("print", js_print, 1, 0),
        //JS_FS("testobj", js_test_obj, 1, 0),
        JS_FS("winSize", js_window_size, 0, 1),
        JS_FS("exit", js_app_exit, 0, 0),
        JS_FS_END
    };
    register_funcs(cx, global, app, "app");
    
    static JSFunctionSpec ui[] = {
        JS_FS("load_savedata", js_load_ui_savedata, 0, 0),
        JS_FS("load_loaddata", js_load_ui_loaddata, 0, 0),
        JS_FS("load_cglibrary", js_load_ui_cg_library, 0, 0),
        JS_FS("load_musiclibrary", js_load_ui_music_library, 0, 0),
        JS_FS("load_movielibrary", js_load_ui_movie_library, 0, 0),
        JS_FS("load_gameoption", js_load_ui_game_option, 0, 0),
        JS_FS("load_gamemenu", js_load_ui_game_menu, 0, 0),
        JS_FS_END
    };
    register_funcs(cx, global, ui, "ui");
    
    static JSFunctionSpec script[] = {
        JS_FS("dofile", js_do_script, 1, 0),
        JS_FS("run", js_run_story_script, 1, 0),
        JS_FS("jump", js_jump_story_script, 1, 0),
        JS_FS("pause", js_pause_story_script, 1, 0),
        JS_FS("back", js_back_story_script, 1, 0),
        JS_FS("exit", js_exit_story_script, 0, 0),
        JS_FS("goto", js_goto_story_tag, 1, 0),
        //JS_FS("gotoif", js_goto_story_tag_if, 2, 0),
        //JS_FS("doif", js_do_code_if, 2, 0),
        JS_FS("mode", js_script_mode, 1, 1),
        JS_FS("start", js_script_start, 0, 0),
        JS_FS("chapter", js_script_chapter, 1, 1),
        JS_FS_END
    };
    register_funcs(cx, global, script, "script");
    
    static JSFunctionSpec stage[] = {
        JS_FS("push", js_stage_push, 1, 0),
        JS_FS("pop", js_stage_pop, 0, 1),
        JS_FS("clear", js_stage_all_clear, 1, 0),
        JS_FS("xy", js_stage_pos, 3, 1),
        JS_FS("anchor", js_stage_anchor, 3, 1),
        JS_FS("scale", js_stage_scale, 3, 1),
        JS_FS("rotate", js_stage_rotate, 2, 1),
        JS_FS("show", js_stage_show, 2, 1),
        JS_FS("action", js_stage_action, 2, 0),
        JS_FS("num", js_stage_num, 0, 1),
        JS_FS_END
    };
    register_funcs(cx, global, stage, "stage");
    
    static JSFunctionSpec sprite[] = {
        JS_FS("load", js_sprite_load, 6, 0),
        JS_FS("kill", js_sprite_null, 1, 0),
        JS_FS("play", js_sprite_play, 3, 0),
        JS_FS("xy", js_sprite_pos, 3, 1),
        JS_FS("anchor", js_sprite_anchor, 3, 1),
        JS_FS("scale", js_sprite_scale, 3, 1),
        JS_FS("rotate", js_sprite_scale, 2, 1),
        JS_FS("color", js_sprite_color, 4, 1),
        JS_FS("alpha", js_sprite_alpha, 2, 1),
        JS_FS("size", js_sprite_size, 3, 1),
        JS_FS("show", js_sprite_show, 2, 1),
        JS_FS("mov", js_sprite_mov, 2, 0),
        JS_FS("cpy", js_sprite_cpy, 2, 0),
        JS_FS("swap", js_sprite_swap, 2, 0),
        JS_FS("action", js_sprite_action, 2, 0),
        JS_FS_END
    };
    register_funcs(cx, global, sprite, "sprite");
    
    static JSFunctionSpec media[] = {
        JS_FS("playmusic", js_music_play, 1, 0),
        JS_FS("stopmusic", js_music_stop, 0, 0),
        JS_FS("pausemusic", js_music_pause, 0, 0),
        JS_FS("resumemusic", js_music_resume, 0, 0),
        JS_FS("setmusicvol", js_music_set_vol, 1, 0),
        JS_FS("getmusicvol", js_music_get_vol, 0, 1),
        JS_FS("playse", js_se_play, 1, 1),
        JS_FS("stopse", js_se_stop, 1, 0),
        JS_FS("setsevol", js_se_set_vol, 1, 0),
        JS_FS("playvideo", js_video_play, 1, 0),
        JS_FS_END
    };
    register_funcs(cx, global, media, "media");
    
    static JSFunctionSpec backlog[] = {
        JS_FS("insert", js_backlog_insert_log, 3, 0),
        JS_FS_END
    };
    register_funcs(cx, global, backlog, "backlog");
    
    static JSFunctionSpec kv[] = {
        JS_FS("set", js_kv_set, 2, 0),
        JS_FS("add", js_kv_add, 2, 0),
        JS_FS("getString", js_kv_get_string, 1, 1),
        JS_FS("getDouble", js_kv_get_double, 1, 1),
        JS_FS("getInt", js_kv_get_int, 1, 1),
        JS_FS("getBool", js_kv_get_bool, 1, 1),
        JS_FS("remove", js_kv_remove, 1, 0),
        JS_FS("clear", js_kv_clear, 0, 0),
        JS_FS_END
    };
    register_funcs(cx, global, kv, "kv");
    
    static JSFunctionSpec pub_kv[] = {
        JS_FS("set", js_pub_kv_set, 2, 0),
        JS_FS("add", js_pub_kv_add, 2, 0),
        JS_FS("getString", js_pub_kv_get_string, 1, 1),
        JS_FS("getDouble", js_pub_kv_get_double, 1, 1),
        JS_FS("getInt", js_pub_kv_get_int, 1, 1),
        JS_FS("getBool", js_pub_kv_get_bool, 1, 1),
        JS_FS("remove", js_pub_kv_remove, 1, 0),
        JS_FS("save", js_pub_kv_save, 0, 0),
        JS_FS("load", js_pub_kv_load, 0, 0),
        JS_FS_END
    };
    register_funcs(cx, global, pub_kv, "pubkv");
}

NS_BF_END
