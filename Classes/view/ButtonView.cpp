//
//  ButtonView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#include "ButtonView.hpp"
#include "proxy/ResProxy.hpp"
#include "proxy/StoryScriptProxy.hpp"
#include "app/App.hpp"

NS_BF_BEGIN

ButtonSpriteState::ButtonSpriteState() {
    
}

void ButtonSpriteState::Set(BF::AvgSprite *sprite) {
    auto view = sprite->type<ButtonView>();
    if (view != nullptr) {
        view->loadImageFromResProxy(this->mNormalName, this->mSelectedName, this->mDisabledName);
        view->setJSCode(this->mCode);
    }
    SimpleSpriteState::Set(sprite);
}

void ButtonSpriteState::Get(BF::AvgSprite *sprite) {
    SimpleSpriteState::Get(sprite);
    auto view = sprite->type<ButtonView>();
    if (view != nullptr) {
        this->mNormalName = view->mNormalName;
        this->mSelectedName = view->mSelectedName;
        this->mDisabledName = view->mDisabledName;
        this->mCode = view->mCode;
    }
}

ButtonView::ButtonView():AvgSprite(TYPE_BTN, this) {
}

void ButtonView::loadImageFromResProxy(std::string const& normal, std::string const& selected, std::string const& disabled, bool Scale9, ResProxy* res) {
    if (res == nullptr) {
        res = App::getProxy<ResProxy>(ResProxy::Name);
    }
    
    if (normal.length() != 0) {
        auto normalTex = res->getTexture(normal);
        getRendererNormal()->initWithTexture(normalTex);
        setupPressedTexture(normalTex != nullptr);
        this->mNormalName = normal;
        
        if (_customSize.equals(cocos2d::Size::ZERO)) {
            _customSize = normalTex->getContentSize();
        }
    }
    
    if (selected.length() != 0) {
        auto selectedTex = res->getTexture(selected);
        getRendererClicked()->initWithTexture(selectedTex);
        setupPressedTexture(selectedTex != nullptr);
        this->mSelectedName = selected;
        
        if (_customSize.equals(cocos2d::Size::ZERO)) {
            _customSize = selectedTex->getContentSize();
        }
    }
    
    if (disabled.length() != 0) {
        auto disabledTex = res->getTexture(disabled);
        getRendererDisabled()->initWithTexture(disabledTex);
        setupDisabledTexture(disabledTex != nullptr);
        this->mDisabledName = disabled;
        
        if (_customSize.equals(cocos2d::Size::ZERO)) {
            _customSize = disabledTex->getContentSize();
        }
    }
    
    this->setScale9Enabled(Scale9);
    this->setUnifySizeEnabled(true);
    this->ignoreContentAdaptWithSize(false);
    this->updateContentSize();
}

void ButtonView::setJSCode(std::string const& code) {
    this->mCode = code;
}

std::string const& ButtonView::getJSCode() const {
    return this->mCode;
}

ButtonView::SpriteStatePtr ButtonView::getSpriteState() {
    SpriteStatePtr state(new ButtonSpriteState);
    state->Get(this);
    return state;
}

bool ButtonView::onTouchBegan(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(tTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        this->onPressStateChangedToPressed();
        eEvent->stopPropagation();
        return true;
    }
    return false;
}

void ButtonView::onTouchMoved(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(tTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        this->onPressStateChangedToPressed();
    } else {
        this->onPressStateChangedToNormal();
    }
}

void ButtonView::onTouchEnded(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    this->onPressStateChangedToNormal();
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(tTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) && this->mCode.size() != 0 ) {
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->doCode(this->mCode);
    }
}

void ButtonView::onTouchCancel(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    this->onPressStateChangedToNormal();
}

void ButtonView::onEnter() {
    Button::onEnter();
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ButtonView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ButtonView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ButtonView::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(ButtonView::onTouchCancel, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void ButtonView::onExit() {
    Button::onExit();
    this->getEventDispatcher()->removeEventListenersForTarget(this);
}

NS_BF_END
