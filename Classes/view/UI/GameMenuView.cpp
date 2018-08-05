//
//  GameMenuView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/7.
//

#include "GameMenuView.hpp"
#include "GameDataMgrView.hpp"
#include "GameOptionView.hpp"
#include "BacklogView.hpp"
#include "AlertDialogView.hpp"
#include "common/TouchEventRef.hpp"
#include "proxy/StoryScriptProxy.hpp"
#include "proxy/MediaProxy.hpp"
#include "proxy/BacklogProxy.hpp"
#include "proxy/KvProxy.hpp"
#include "proxy/ConfigProxy.hpp"
#include "mediator/StageMediator.hpp"
#include "app/App.hpp"

NS_BF_BEGIN

GameMenuView* GameMenuView::nodeFrom(std::string const& ccbName) {
    auto ccb = CCB::New();
    ccb->registerClass("GameMenu", GameMenuViewLoader::New());
    return (GameMenuView*)ccb->nodeFromCCB(ccbName);
}

GameMenuView::GameMenuView():mScreenShot(nullptr), mShotMask(nullptr), mBox(nullptr) {
    
}

void GameMenuView::start(cocos2d::Texture2D* screenshot) {
    if (mScreenShot != nullptr) {
        mScreenShot->initWithTexture(screenshot);
        this->playAnimation("firstAni");
        this->mBox->setVisible(true);
    }
}

void GameMenuView::setBackTitleScript(std::string const& fileName) {
    this->mTitleCode = fileName;
}

cocos2d::SEL_MenuHandler GameMenuView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    return nullptr;
}

cocos2d::extension::Control::Handler GameMenuView::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMenuAuto", GameMenuView::onAutoMode);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMenuSkip", GameMenuView::onSkipMode);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMenuBacklog", GameMenuView::onBacklog);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMenuDataSave", GameMenuView::onSave);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMenuDataLoad", GameMenuView::onLoad);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMenuGameOption", GameMenuView::onOption);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMenuBackToTitle", GameMenuView::onBackTitle);
    return nullptr;
}

bool GameMenuView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeBox", cocos2d::Node*, mBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "imgScreen", cocos2d::Sprite*, mScreenShot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "colorMask", cocos2d::LayerColor*, mShotMask);
    return false;
}

void GameMenuView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    auto listener = TouchEventRef::NewTouchListener(mScreenShot, [&](TouchEventRef const* ref, cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
        cocos2d::Vec2 touchPointInView = mScreenShot->convertToNodeSpace(pTouch->getLocation());
        if( getBoundingBox().containsPoint(touchPointInView) ) {
            pEvent->stopPropagation();
            this->mShotMask->setContentSize(mScreenShot->getContentSize());
            this->mShotMask->setScale(mScreenShot->getScale());
            this->mShotMask->setAnchorPoint(mScreenShot->getAnchorPoint());
            this->mShotMask->setPosition(mScreenShot->getPosition());
            this->mShotMask->setVisible(true);
            return true;
        }
        return false;
    }, [&](TouchEventRef const* ref, cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
        cocos2d::Vec2 touchPointInView = mScreenShot->convertToNodeSpace(pTouch->getLocation());
        if( getBoundingBox().containsPoint(touchPointInView) ) {
            this->mShotMask->setVisible(true);
        } else {
            this->mShotMask->setVisible(false);
        }
    }, [&](TouchEventRef const* ref, cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
        if (this->mShotMask->isVisible()) {
            this->onBack(nullptr);
        }
        this->mShotMask->setVisible(false);
    });
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, mScreenShot);
    this->mShotMask->setVisible(false);
    this->mBox->setVisible(false);
}

void GameMenuView::onBack(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", [&](std::string const& aniName) {
        this->removeFromParent();
    });
    this->playAnimation("outAni");
}

void GameMenuView::onAutoMode(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type) {
    /*this->setAnimationCompletedCallBack("outAni", [&](std::string const& aniName) {
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->setMode(StoryScriptProxy::MODE_AUTO);
        this->removeFromParent();
    });
    this->playAnimation("outAni");*/
    
    auto config = App::getProxy<ConfigProxy>(ConfigProxy::Name);
    auto ui = config->getSession("ui");
    auto view = AlertDialogView::NewErr(ui->getValueAsStr("unfinished-msg"));
    cocos2d::Director::getInstance()->getRunningScene()->addChild(view);
}

void GameMenuView::onSkipMode(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type) {
    this->setAnimationCompletedCallBack("outAni", [&](std::string const& aniName) {
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->setMode(StoryScriptProxy::MODE_SKIP);
        this->removeFromParent();
    });
    this->playAnimation("outAni");
}

void GameMenuView::onBacklog(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type) {
    auto view = BacklogView::nodeFrom("system/back_log");
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
    
    this->scheduleOnce([&](float dt) {
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        mediator->getViewHolder()->setVisible(false);
        this->setVisible(false);
    }, 0.5, "GameMenuView::onBacklog");
    
    view->setExitFunc([&]() {
        auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
        mediator->getViewHolder()->setVisible(true);
        this->setVisible(true);
    });
}

void GameMenuView::onSave(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type) {
    auto view = GameDataMgrView::nodeFrom("system/data_manager");
    view->start(GameDataMgrView::TYPE_SAVE);
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
}

void GameMenuView::onLoad(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type) {
    auto view = GameDataMgrView::nodeFrom("system/data_manager");
    view->start(GameDataMgrView::TYPE_LOAD);
    view->setCompletedFunc([&](GameDataMgrView::MgrType type) {
        this->removeFromParent();
        
        auto backlog = App::getProxy<BacklogProxy>(BacklogProxy::Name);
        backlog->clear();
    });
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
}

void GameMenuView::onOption(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type) {
    auto view = GameOptionView::nodeFrom("system/game_option");
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(view);
}

void GameMenuView::onBackTitle(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type) {
    if (this->mTitleCode.length() == 0) {
        return;
    }
    this->setAnimationCompletedCallBack("outAni", [&](std::string const& aniName) {
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->doCode(this->mTitleCode);
        this->removeFromParent();
        
        auto backlog = App::getProxy<BacklogProxy>(BacklogProxy::Name);
        backlog->clear();
        
        auto kv = App::getProxy<KvProxy>(KvProxy::Name);
        kv->clear();
    });
    this->playAnimation("outAni");
}

NS_BF_END
