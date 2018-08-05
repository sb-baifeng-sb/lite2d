//
//  SpriteStackView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/14.
//

#include "SpriteStackView.hpp"
#include "BFImageView.hpp"
#include "ButtonView.hpp"
#include "GifView.hpp"
#include "DragonBonesView.hpp"
#include "TextView.hpp"
#include "SpeakView.hpp"
#include "CcbUiView.hpp"
#include "proxy/DragonBonesFactory.hpp"
#include "proxy/ResProxy.hpp"
#include "proxy/StoryScriptProxy.hpp"
#include "app/App.hpp"
#include "tools/jsengine.hpp"

NS_BF_BEGIN

SpriteStackView::SpriteStackView() {
    mSprites.reserve(20);
    setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

SpriteStackView::~SpriteStackView() {
    this->clear();
}

void SpriteStackView::clear() {
    for (int i=0; i < (int)mSprites.size(); i++) {
        if (mSprites[i] != nullptr) {
            mSprites[i]->onRemove();
            mSprites[i]->getTargetNode()->removeFromParent();
        }
    }
    mSprites.clear();
}

bool SpriteStackView::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    auto touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        // 点击屏幕对话
        auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        if (proxy->getMode() != StoryScriptProxy::MODE_NORMAL) {
            proxy->setMode(StoryScriptProxy::MODE_NORMAL);
            return true;
        }
        proxy->getSpeakHolder()->ClickSpeak();
        return true;
    }
    return false;
}

BFImageView* SpriteStackView::makeImageView(std::string const& imgName) {
    auto view = BFImageView::create();
    view->loadImageFromResProxy(imgName);
    return view;
}

ButtonView* SpriteStackView::makeButtonView(std::string const& normal, std::string const& selected, std::string const& disabled, bool scale9) {
    auto view = ButtonView::create();
    view->loadImageFromResProxy(normal, selected, disabled, scale9);
    return view;
}

GifView* SpriteStackView::makeGifView(std::string const& gifName) {
    auto view = GifView::create();
    view->loadFromResProxy(gifName);
    return view;
}

DragonBonesView* SpriteStackView::makeDBView(std::string const& dragonBonesName, std::string const& armatureName) {
    auto factory = DragonBonesFactory::getInstance();
    factory->loadDragonBones(dragonBonesName);
    auto view = factory->makeView(dragonBonesName, armatureName);
    view->saveResName(dragonBonesName, armatureName);
    return view;
}

TextView* SpriteStackView::makeTextView(std::string const& title, float width) {
    auto view = TextView::create();
    view->setWidth(width);
    view->setString(title);
    return view;
}

HalfSpeakView* SpriteStackView::makeHalfSpeakView() {
    auto view = HalfSpeakView::create();
    return view;
}

CcbUiView* SpriteStackView::makeCcbUiView(std::string const& ccbi, std::string const& className) {
    auto view = CcbUiView::New(ccbi, className);
    view->setCodeExcute([&](std::string const& code) {
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->doCode(code);
    });
    return view;
}

void SpriteStackView::setStackSize(int size) {
    clear();
    for (int i=0; i < size; i++) {
        mSprites.push_back(nullptr);
    }
}

int SpriteStackView::getStackSize() const {
    return (int)this->mSprites.size();
}

void SpriteStackView::removeAllSprite() {
    setStackSize((int)this->mSprites.size());
}

void SpriteStackView::setSprite(int index, SpriteStackView::Sprite* sprite) {
    if (index < 0 || index >= (int)this->mSprites.size()) {
        return;
    }
    
    // 移除原有节点
    auto old_sprite = this->mSprites[index];
    if (old_sprite != nullptr) {
        old_sprite->onRemove();
        old_sprite->getTargetNode()->removeFromParent();
    }
    
    // 增加新节点
    this->mSprites[index] = sprite;
    if (sprite != nullptr) {
        this->addChild(sprite->getTargetNode(), index);
        sprite->onRegister();
        sprite->getTargetNode()->setVisible(false);
    }
}

bool SpriteStackView::isSpriteType(int index, SpriteStackView::Sprite::Type type) const {
    Sprite* sprite = getSprite(index);
    if (sprite == nullptr || (sprite && sprite->getSpriteType() != type)) {
        return false;
    }
    return true;
}

SpriteStackView::Sprite* SpriteStackView::getSprite(int index) const {
    if (index >= 0 && index < this->mSprites.size()) {
        return this->mSprites[index];
    }
    return nullptr;
}

SpriteStackView::Node* SpriteStackView::getSpriteAsNode(int index) const {
    Sprite* sprite = getSprite(index);
    if (sprite != nullptr) {
        return sprite->getTargetNode();
    }
    return nullptr;
}

void SpriteStackView::setSpriteXY(int index, float x, float y) {
    auto sprite = getSpriteAsNode(index);
    if (sprite != nullptr) {
        sprite->setPosition(x, y);
    }
}

void SpriteStackView::setSpriteAnchor(int index, float x, float y) {
    auto sprite = getSpriteAsNode(index);
    if (sprite != nullptr) {
        sprite->setAnchorPoint(cocos2d::Vec2(x, y));
    }
}

void SpriteStackView::setSpriteScale(int index, float x, float y) {
    auto sprite = getSpriteAsNode(index);
    if (sprite != nullptr) {
        sprite->setScale(x, y);
    }
}

void SpriteStackView::setSpriteSize(int index, int w, int h) {
    auto sprite = getSpriteAsNode(index);
    if (sprite != nullptr) {
        sprite->setContentSize(cocos2d::Size(w, h));
    }
}

void SpriteStackView::showSprite(int index, bool show) {
    auto sprite = getSpriteAsNode(index);
    if (sprite != nullptr) {
        sprite->setVisible(show);
    }
}

void SpriteStackView::movSprite(int index0, int index1) {
    this->cpySprite(index0, index1);
    this->setSprite(index1, nullptr);
}

void SpriteStackView::cpySprite(int index0, int index1) {
    auto sprite1 = this->getSprite(index1);
    if (sprite1 == nullptr) {
        this->setSprite(index0, nullptr);
        return;
    }
    BF::AvgSprite* sprite = this->copySprite(sprite1);
    this->setSprite(index0, sprite);
    if (sprite != nullptr) {
        sprite->getTargetNode()->setVisible( sprite1->getTargetNode()->isVisible() );
    }
}

void SpriteStackView::swapSprite(int index0, int index1) {
    auto sprite0 = this->getSprite(index0);
    auto sprite1 = this->getSprite(index1);
    if (sprite0 == nullptr && sprite1 != nullptr) {
        this->movSprite(index0, index1);
        return;
    }
    if (sprite0 != nullptr && sprite1 == nullptr) {
        this->movSprite(index1, index0);
        return;
    }
    if (sprite0 == nullptr && sprite1 == nullptr) {
        return;
    }
    auto newSprite0 = this->copySprite(sprite0);
    auto newSprite1 = this->copySprite(sprite1);
    bool visible0 = sprite0->getTargetNode()->isVisible();
    bool visible1 = sprite1->getTargetNode()->isVisible();
    this->setSprite(index0, newSprite1);
    this->setSprite(index1, newSprite0);
    newSprite0->getTargetNode()->setVisible(visible0);
    newSprite1->getTargetNode()->setVisible(visible1);
}

BF::AvgSprite* SpriteStackView::copySprite(BF::AvgSprite* source) const {
    auto state = source->getSpriteState();
    auto type = source->getSpriteType();
    BF::AvgSprite* sprite = nullptr;
    if (type == BF::AvgSprite::TYPE_IMG) {
        auto view = BF::BFImageView::create();
        sprite = view;
    } else if (type == BF::AvgSprite::TYPE_BTN) {
        auto view = BF::ButtonView::create();
        sprite = view;
    } else if (type == BF::AvgSprite::TYPE_GIF) {
        auto view = BF::GifView::create();
        sprite = view;
    } else if (type == BF::AvgSprite::TYPE_DB) {
        auto _state = static_cast<DBSpriteState*>(&(*state));
        auto dbView = this->makeDBView(_state->getDragonBonesName(), _state->getArmatureName());
        sprite = dbView;
    } else if (type == BF::AvgSprite::TYPE_LABEL) {
        auto view = BF::TextView::create();
        sprite = view;
    } else if (type == BF::AvgSprite::TYPE_HALF_SPEAK) {
        auto view = BF::HalfSpeakView::create();
        sprite = view;
    } else if (type == BF::AvgSprite::TYPE_CCB_UI) {
        auto _state = static_cast<CcbUiViewState*>(&(*state));
        auto view = this->makeCcbUiView(_state->getCcbName(), _state->getClassName());
        sprite = view;
    }
    if (sprite != nullptr) {
        state->Set(sprite);
    }
    return sprite;
}

typedef std::function<bool(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index)> LoadFunc;
static std::map<std::string, LoadFunc> LoadFuncMap;

void registerLoadFuncMap();
bool SpriteStackView::loadSprite(int index, std::string const& type, JSContext* cx, uint32_t argc, JS::CallArgs& args) {
    registerLoadFuncMap();
    auto const iter = LoadFuncMap.find(type);
    if (iter != LoadFuncMap.end()) {
        return iter->second(this, cx, args, argc, index);
    }
    return false;
}

// ======================================================================================

bool _loadImg(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index);
bool _loadBtn(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index);
bool _loadDragonBones(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index);
bool _loadGif(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index);
bool _loadLabel(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index);
bool _loadHalfSpeak(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index);
bool _loadCcbUi(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index);

void registerLoadFuncMap() {
    if (LoadFuncMap.size() != 0) {
        return;
    }
    LoadFuncMap["img"] = _loadImg;
    LoadFuncMap["btn"] = _loadBtn;
    LoadFuncMap["db"] = _loadDragonBones;
    LoadFuncMap["gif"] = _loadGif;
    LoadFuncMap["label"] = _loadLabel;
    LoadFuncMap["halfspeak"] = _loadHalfSpeak;
    LoadFuncMap["ccb-ui"] = _loadCcbUi;
}

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

bool _loadImg(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index) {
    bool ok = true;
    auto storyView = target;
    std::string arg2;
    ok &= jsval_to_std_string(cx, args.get(2), &arg2);
    JSB_PRECONDITION2(ok, cx, false, "_loadImg : Error processing arguments");
    if (!storyView->isSpriteType(index, AvgSprite::TYPE_IMG)) {
        auto view = storyView->makeImageView(arg2);
        storyView->setSprite(index, view);
    } else {
        auto view = dynamic_cast<BFImageView*>(storyView->getSpriteAsNode(index));
        view->loadImageFromResProxy(arg2);
    }
    return true;
}

bool _loadBtn(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index) {
    bool ok = true;
    auto storyView = target;
    std::string arg2, arg3, arg4, arg5;
    ok &= jsval_to_std_string(cx, args.get(2), &arg2);
    ok &= jsval_to_std_string(cx, args.get(3), &arg3);
    ok &= jsval_to_std_string(cx, args.get(4), &arg4);
    JSB_PRECONDITION2(ok, cx, false, "_loadBtn : Error processing arguments");
    if (argc >= 6) {
        ok &= jsval_to_std_string(cx, args.get(5), &arg5);
        JSB_PRECONDITION2(ok, cx, false, "_loadBtn : Error processing arguments");
    }
    if (!storyView->isSpriteType(index, AvgSprite::TYPE_BTN)) {
        auto view = storyView->makeButtonView(arg2, arg3, arg4, args.length() >= 7 ? args.get(6).toBoolean() : false);
        view->setJSCode(arg5);
        storyView->setSprite(index, view);
    } else {
        auto view = dynamic_cast<ButtonView*>(storyView->getSpriteAsNode(index));
        view->loadImageFromResProxy(arg2, arg3, arg4, args.length() >= 7 ? args.get(6).toBoolean() : false);
        view->setJSCode(arg5);
    }
    return true;
}

bool _loadDragonBones(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index) {
    bool ok = true;
    auto storyView = target;
    std::string arg2, arg3;
    ok &= jsval_to_std_string(cx, args.get(2), &arg2);
    ok &= jsval_to_std_string(cx, args.get(3), &arg3);
    JSB_PRECONDITION2(ok, cx, false, "_loadDragonBones : Error processing arguments");
    auto view = storyView->makeDBView(arg2, arg3);
    storyView->setSprite(index, view);
    return true;
}

bool _loadGif(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index) {
    bool ok = true;
    auto storyView = target;
    std::string arg2;
    ok &= jsval_to_std_string(cx, args.get(2), &arg2);
    JSB_PRECONDITION2(ok, cx, false, "_loadGif : Error processing arguments");
    if (!storyView->isSpriteType(index, AvgSprite::TYPE_GIF)) {
        auto view = storyView->makeGifView(arg2);
        storyView->setSprite(index, view);
    } else {
        auto view = dynamic_cast<GifView*>(storyView->getSpriteAsNode(index));
        view->loadFromResProxy(arg2);
    }
    return true;
}

bool _loadLabel(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index) {
    bool ok = true;
    auto storyView = target;
    std::string arg2;
    float arg3;
    ok &= jsval_to_std_string(cx, args.get(2), &arg2);
    ok &= jsval_to_float(cx, args.get(3), &arg3);
    JSB_PRECONDITION2(ok, cx, false, "_loadGif : Error processing arguments");
    if (!storyView->isSpriteType(index, AvgSprite::TYPE_LABEL)) {
        auto view = storyView->makeTextView(arg2, arg3);
        storyView->setSprite(index, view);
    } else {
        auto view = dynamic_cast<TextView*>(storyView->getSpriteAsNode(index));
        view->setWidth(arg3);
        view->setString(arg2);
    }
    return true;
}

bool _loadHalfSpeak(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index) {
    bool ok = true;
    auto storyView = target;
    
    JS::RootedObject text(cx, args.get(2).toObjectOrNull());
    JS::RootedValue top(cx), left(cx), bottom(cx), right(cx), align(cx);
    
    ok &= JS_GetProperty(cx, text, "top", &top);
    ok &= JS_GetProperty(cx, text, "left", &left);
    ok &= JS_GetProperty(cx, text, "bottom", &bottom);
    ok &= JS_GetProperty(cx, text, "right", &right);
    ok &= JS_GetProperty(cx, text, "align", &align);
    JSB_PRECONDITION2(ok, cx, false, "_loadHalfSpeak : Error processing arguments");
    
    std::string alignStr;
    ok &= jsval_to_std_string(cx, align, &alignStr);
    JSB_PRECONDITION2(ok, cx, false, "_loadHalfSpeak : Error processing arguments");
    
    JS::RootedObject iconObj(cx, args.get(3).toObjectOrNull());
    JS::RootedValue icon(cx);
    
    ok &= JS_GetProperty(cx, iconObj, "icon", &icon);
    //ok &= JS_GetProperty(cx, iconObj, "width", &width);
    //ok &= JS_GetProperty(cx, iconObj, "height", &height);
    JSB_PRECONDITION2(ok, cx, false, "_loadHalfSpeak : Error processing arguments");
    
    std::string iconStr;
    ok &= jsval_to_std_string(cx, icon, &iconStr);
    JSB_PRECONDITION2(ok, cx, false, "_loadHalfSpeak : Error processing arguments");
    
    if (!storyView->isSpriteType(index, AvgSprite::TYPE_HALF_SPEAK)) {
        auto view = storyView->makeHalfSpeakView();
        view->setPadding(top.toInt32(), left.toInt32(), bottom.toInt32(), right.toInt32());
        view->setAlignment(alignStr == "left" ? cocos2d::TextHAlignment::LEFT : (alignStr == "center" ? cocos2d::TextHAlignment::CENTER : cocos2d::TextHAlignment::LEFT));
        view->loadIconFromResProxy(iconStr);
        storyView->setSprite(index, view);
    } else {
        auto view = dynamic_cast<HalfSpeakView*>(storyView->getSpriteAsNode(index));
        view->setPadding(top.toInt32(), left.toInt32(), bottom.toInt32(), right.toInt32());
        view->setAlignment(alignStr == "left" ? cocos2d::TextHAlignment::LEFT : (alignStr == "center" ? cocos2d::TextHAlignment::CENTER : cocos2d::TextHAlignment::LEFT));
        view->loadIconFromResProxy(iconStr);
    }
    
    return true;
}

bool _loadCcbUi(SpriteStackView* target, JSContext* cx, JS::CallArgs& args, uint32_t argc, uint32_t index) {
    bool ok = true;
    auto storyView = target;
    
    JS::RootedObject param(cx, args.get(2).toObjectOrNull());
    JS::RootedValue ccbName(cx), className(cx);
    
    ok &= JS_GetProperty(cx, param, "ccbName", &ccbName);
    ok &= JS_GetProperty(cx, param, "className", &className);
    JSB_PRECONDITION2(ok, cx, false, "_loadCcbUi : Error processing arguments");
    
    std::string ccbNameStr, classNameStr;
    ok &= jsval_to_std_string(cx, ccbName, &ccbNameStr);
    ok &= jsval_to_std_string(cx, className, &classNameStr);
    JSB_PRECONDITION2(ok, cx, false, "_loadCcbUi : Error processing arguments");
    
    auto view = storyView->makeCcbUiView(ccbNameStr, classNameStr);
    
    std::string codeStr;
    JS::RootedValue code(cx);
    auto array = view->getSelectorVector();
    for (int i=0; i < (int)array.size(); i++) {
        auto const& name = array[i];
        ok &= JS_GetProperty(cx, param, name.c_str(), &code);
        if (!ok) {
            continue;
        }
        ok &= jsval_to_std_string(cx, code, &codeStr);
        if (!ok || codeStr.length() == 0) {
            continue;
        }
        view->setSelectorAndCode(name, codeStr);
    }
    storyView->setSprite(index, view);
    
    return true;
}

NS_BF_END
