//
//  GameOptionView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#include "GameOptionView.hpp"
#include "../../proxy/MediaProxy.hpp"
#include "../../app/App.hpp"

NS_BF_BEGIN

GameOptionView* GameOptionView::nodeFrom(std::string const& ccbName) {
    auto ccb = CCB::New();
    ccb->registerClass("GameOption", GameOptionViewLoader::New());
    ccb->registerClass("GameOptionVolumeBar", GameOptionVolumeBarLoader::New());
    ccb->registerClass("ProgressBar", GameOptionClipNodeLoader::New());
    return (GameOptionView*)ccb->nodeFromCCB(ccbName);
}

GameOptionView::GameOptionView() {
    memset(mBar, 0, sizeof(mBar));
}

cocos2d::SEL_MenuHandler GameOptionView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", GameOptionView::onBack);
    return nullptr;
}

bool GameOptionView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbBar0", GameOptionVolumeBar*, mBar[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbBar1", GameOptionVolumeBar*, mBar[1]);
    return false;
}

void GameOptionView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
    for (int i=0; i < 2; i++) {
        auto bar = this->mBar[i];
        if (i == 0) {
            bar->setValue(media->getMusicVolScale());
            bar->setValueChangedCallback([&](float value) {
                auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
                media->setMusicVolScale(value);
            });
        } else if (i == 1) {
            bar->setValue(media->getSEVolScale());
            bar->setValueChangedCallback([&](float value) {
                auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
                media->setSEVolScale(value);
            });
        }
    }
}

void GameOptionView::onBack(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", std::bind(&GameOptionView::onOutEnd, this));
    this->playAnimation("outAni");
}

void GameOptionView::onOutEnd() {
    auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
    media->saveConfig(true);
    this->removeFromParent();
}

// =================================================================================================================

GameOptionVolumeBar::GameOptionVolumeBar():mValue(nullptr), mClipNode(nullptr) {
    
}

bool GameOptionVolumeBar::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfValue", cocos2d::Label*, mValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeProgressBar", ClipNode*, mClipNode);
    return false;
}

void GameOptionVolumeBar::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    this->mDefSize = this->mClipNode->getContentSize();
    auto mask = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 255), mDefSize.width, mDefSize.height);
    this->mClipNode->setStencil(mask);
    this->mClipNode->setInverted(false);
    this->mClipNode->setAlphaThreshold(0);
    
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameOptionVolumeBar::onClipTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameOptionVolumeBar::onClipTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameOptionVolumeBar::onClipTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameOptionVolumeBar::onClipTouchCancel, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->mClipNode);
}

void GameOptionVolumeBar::setValue(float v) {
    v = v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
    this->mClipNode->getStencil()->setContentSize(cocos2d::Size(this->mDefSize.width*v, this->mDefSize.height));
    char temp[64] = {0};
    sprintf(temp, "%d%%", (int)(v*100));
    this->mValue->setString(temp);
    
    if (mChanged != nullptr) {
        mChanged(v);
    }
}

void GameOptionVolumeBar::setValueChangedCallback(GameOptionVolumeBar::ProgressBarChanedCallback callback) {
    this->mChanged = callback;
}

bool GameOptionVolumeBar::onClipTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = this->mClipNode->getParent()->convertToNodeSpace(pTouch->getLocation());
    if( this->mClipNode->getBoundingBox().containsPoint(touchPointInView) ) {
        pEvent->stopPropagation();
        this->setValue(touchPointInView.x / this->mDefSize.width);
        return true;
    }
    return false;
}

void GameOptionVolumeBar::onClipTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = this->mClipNode->getParent()->convertToNodeSpace(pTouch->getLocation());
    this->setValue(touchPointInView.x / this->mDefSize.width);
}

void GameOptionVolumeBar::onClipTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = this->mClipNode->getParent()->convertToNodeSpace(pTouch->getLocation());
    this->setValue(touchPointInView.x / this->mDefSize.width);
}

void GameOptionVolumeBar::onClipTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    
}

NS_BF_END
