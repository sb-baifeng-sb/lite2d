LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/bftools/archives.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/bftools/pack.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/bftools/parseini.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/bftools/simpleapi.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/animation/Animation.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/animation/AnimationState.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/animation/BaseTimelineState.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/animation/TimelineState.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/animation/WorldClock.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/armature/Armature.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/armature/Bone.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/armature/Constraint.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/armature/DeformVertices.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/armature/Slot.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/armature/TransformObject.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/cocos2dx/CCArmatureDisplay.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/cocos2dx/CCFactory.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/cocos2dx/CCSlot.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/cocos2dx/CCTextureAtlasData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/core/BaseObject.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/core/DragonBones.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/event/EventObject.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/factory/BaseFactory.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/geom/Point.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/geom/Transform.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/AnimationConfig.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/AnimationData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/ArmatureData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/BoundingBoxData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/CanvasData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/ConstraintData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/DisplayData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/DragonBonesData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/SkinData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/TextureAtlasData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/model/UserData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/parser/BinaryDataParser.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/parser/DataParser.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/dragonBones/parser/JSONDataParser.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SHA1.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SqlCommon.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SqlDatabase.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SqlField.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SqlFieldSet.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/sqlite3.c \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SqlRecord.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SqlRecordSet.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SqlTable.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/easySQLite/SqlValue.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/entity/component.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/entity/entity.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/entity/event.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/entity/system.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/entity/world.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/gif_lib/dgif_lib.c \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/gif_lib/gif_err.c \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/gif_lib/gif_font.c \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/gif_lib/gif_hash.c \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/gif_lib/gifalloc.c \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/gif_lib/quantize.c \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/Bitmap.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/CacheGif.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/CacheGifData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/FileData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/GifBase.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/GifMovie.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/GifMovieData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/InstantGif.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/InstantGifData.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/Gif/Movie.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/micropather/micropather.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/obs/source/connection.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/puremvc/pmvcpp.cpp \
                   $(LOCAL_PATH)/../../../Classes/third/tea/TEA_Encrypt.c \
                   $(LOCAL_PATH)/../../../Classes/third/android/iconv/lib/iconv.c \
                   $(LOCAL_PATH)/../../../Classes/third/android/iconv/lib/relocatable.c \
                   $(LOCAL_PATH)/../../../Classes/third/android/iconv/libcharset/lib/localcharset.c \
                   $(LOCAL_PATH)/../../../Classes/app/App.cpp \
                   $(LOCAL_PATH)/../../../Classes/common/actions.cpp \
                   $(LOCAL_PATH)/../../../Classes/common/BFTableView.cpp \
                   $(LOCAL_PATH)/../../../Classes/common/CcbBaseView.cpp \
                   $(LOCAL_PATH)/../../../Classes/common/CharsetConvert.cpp \
                   $(LOCAL_PATH)/../../../Classes/common/TouchEventRef.cpp \
                   $(LOCAL_PATH)/../../../Classes/mediator/StageMediator.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/BacklogProxy.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/ConfigProxy.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/DragonBonesFactory.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/JSApiRegisterFunc.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/JSApis.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/KvProxy.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/MediaProxy.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/ResProxy.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/SaveProxy.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/SqlProxy.cpp \
                   $(LOCAL_PATH)/../../../Classes/proxy/StoryScriptProxy.cpp \
                   $(LOCAL_PATH)/../../../Classes/tools/jsengine.cpp \
                   $(LOCAL_PATH)/../../../Classes/tools/jssugar.cpp \
                   $(LOCAL_PATH)/../../../Classes/tools/script.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/BFImageView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/ButtonView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/CcbUiView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/DragonBonesView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/GifView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/MovieView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/SpeakView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/Sprite.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/SpriteStackView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/TextView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/UI/AlertDialogView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/UI/BacklogView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/UI/CGLibraryView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/UI/GameDataMgrView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/UI/GameMenuView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/UI/GameOptionView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/UI/MovieLibraryView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/UI/MusicLibraryView.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../Classes/third \
                    $(LOCAL_PATH)/../../../Classes/third/android/iconv/include \
                    $(LOCAL_PATH)/../../../Classes/third/android/iconv/lib \
                    $(LOCAL_PATH)/../../../Classes/third/android/iconv/libcharset/include \
                    $(LOCAL_PATH)/../../../cocos2d/external/spidermonkey/include/android

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += spidermonkey_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,spidermonkey/prebuilt/android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
