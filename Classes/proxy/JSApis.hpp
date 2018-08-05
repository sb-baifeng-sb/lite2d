//
//  JSApis.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/13.
//

#ifndef JSApis_hpp
#define JSApis_hpp

#include <stdio.h>
#include "app/macros.h"
#include "jsapi.h"

NS_BF_BEGIN

bool js_print(JSContext *cx, uint32_t argc, jsval *vp);
bool js_test_obj(JSContext *cx, uint32_t argc, jsval *vp);

// 系统接口
bool js_window_size(JSContext *cx, uint32_t argc, jsval *vp);
bool js_app_exit(JSContext *cx, uint32_t argc, jsval *vp);

// 剧本脚本接口
bool js_do_script(JSContext *cx, uint32_t argc, jsval *vp);
bool js_run_story_script(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jump_story_script(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pause_story_script(JSContext *cx, uint32_t argc, jsval *vp);
bool js_back_story_script(JSContext *cx, uint32_t argc, jsval *vp);
bool js_exit_story_script(JSContext *cx, uint32_t argc, jsval *vp);
bool js_goto_story_tag(JSContext *cx, uint32_t argc, jsval *vp);
bool js_goto_story_tag_if(JSContext *cx, uint32_t argc, jsval *vp);
bool js_do_code_if(JSContext *cx, uint32_t argc, jsval *vp);
bool js_script_mode(JSContext *cx, uint32_t argc, jsval *vp);
bool js_script_start(JSContext *cx, uint32_t argc, jsval *vp);
bool js_script_chapter(JSContext *cx, uint32_t argc, jsval *vp);

// 场景接口
bool js_stage_push(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_pop(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_all_clear(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_pos(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_anchor(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_scale(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_rotate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_show(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_action(JSContext *cx, uint32_t argc, jsval *vp);
bool js_stage_num(JSContext *cx, uint32_t argc, jsval *vp);

// 精灵接口
bool js_sprite_load(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_null(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_play(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_pos(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_anchor(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_scale(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_rotate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_color(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_alpha(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_size(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_show(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_mov(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_cpy(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_swap(JSContext *cx, uint32_t argc, jsval *vp);
bool js_sprite_action(JSContext *cx, uint32_t argc, jsval *vp);

// 对话接口

// 音频接口
bool js_music_play(JSContext *cx, uint32_t argc, jsval *vp);
bool js_music_stop(JSContext *cx, uint32_t argc, jsval *vp);
bool js_music_pause(JSContext *cx, uint32_t argc, jsval *vp);
bool js_music_resume(JSContext *cx, uint32_t argc, jsval *vp);
bool js_music_set_vol(JSContext *cx, uint32_t argc, jsval *vp);
bool js_music_get_vol(JSContext *cx, uint32_t argc, jsval *vp);
bool js_se_play(JSContext *cx, uint32_t argc, jsval *vp);
bool js_se_stop(JSContext *cx, uint32_t argc, jsval *vp);
bool js_se_set_vol(JSContext *cx, uint32_t argc, jsval *vp);

// 视频接口
bool js_video_play(JSContext *cx, uint32_t argc, jsval *vp);

// 系统界面
bool js_load_ui_savedata(JSContext *cx, uint32_t argc, jsval *vp);
bool js_load_ui_loaddata(JSContext *cx, uint32_t argc, jsval *vp);
bool js_load_ui_cg_library(JSContext *cx, uint32_t argc, jsval *vp);
bool js_load_ui_music_library(JSContext *cx, uint32_t argc, jsval *vp);
bool js_load_ui_movie_library(JSContext *cx, uint32_t argc, jsval *vp);
bool js_load_ui_game_option(JSContext *cx, uint32_t argc, jsval *vp);
bool js_load_ui_game_menu(JSContext *cx, uint32_t argc, jsval *vp);

// 对话记录
bool js_backlog_insert_log(JSContext *cx, uint32_t argc, jsval *vp);

// 键值对
bool js_kv_set(JSContext *cx, uint32_t argc, jsval *vp);
bool js_kv_add(JSContext *cx, uint32_t argc, jsval *vp);
bool js_kv_get_string(JSContext *cx, uint32_t argc, jsval *vp);
bool js_kv_get_double(JSContext *cx, uint32_t argc, jsval *vp);
bool js_kv_get_int(JSContext *cx, uint32_t argc, jsval *vp);
bool js_kv_get_bool(JSContext *cx, uint32_t argc, jsval *vp);
bool js_kv_remove(JSContext *cx, uint32_t argc, jsval *vp);
bool js_kv_clear(JSContext *cx, uint32_t argc, jsval *vp);

// 公共键值对
bool js_pub_kv_set(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pub_kv_add(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pub_kv_get_string(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pub_kv_get_double(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pub_kv_get_int(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pub_kv_get_bool(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pub_kv_remove(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pub_kv_save(JSContext *cx, uint32_t argc, jsval *vp);
bool js_pub_kv_load(JSContext *cx, uint32_t argc, jsval *vp);

NS_BF_END

#endif /* JSApis_hpp */
