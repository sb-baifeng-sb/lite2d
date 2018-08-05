//
//  StageMediator.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/1.
//

#include "StageMediator.hpp"
#include "app/notify.hpp"
#include "view/SpriteStackView.hpp"
#include "proxy/StoryScriptProxy.hpp"
#include "proxy/ResProxy.hpp"
#include "proxy/MediaProxy.hpp"
#include "app/App.hpp"

USING_NS_CC;

NS_BF_BEGIN

std::string const StageMediator::Name = "StageMediator";

StageMediator::StageMediator():
    PureMVC::Mediator(Name), mScene(nullptr), mViewHoder(nullptr) {
}

std::vector<int> StageMediator::listNotificationInterests() {
    std::vector<int> array;
    array.push_back(Event::SystemFire);
    array.push_back(Event::PushStoryView);
    array.push_back(Event::PopStoryView);
    array.push_back(Event::PauseStoryScript);
    array.push_back(Event::ResumeStoryScript);
    array.push_back(Event::PauseBackGroundMusic);
    array.push_back(Event::ResumeBackGroundMusic);
    return array;
}

void StageMediator::handleNotification( PureMVC::INotification* notification ) {
    if (notification->getName() == Event::SystemFire) {
        makeFirstScene();
    } else if (notification->getName() == Event::PushStoryView) {
        int value = *(int*)(notification->getBody());
        pushSpriteStack(value);
    } else if (notification->getName() == Event::PopStoryView) {
        popSpriteStack();
    } else if (notification->getName() == Event::PauseStoryScript) {
        // 暂停脚本执行
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->pause(60*60*24);
    } else if (notification->getName() == Event::ResumeStoryScript) {
        // 恢复脚本执行
        auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
        script->pause(0);
    } else if (notification->getName() == Event::PauseBackGroundMusic) {
        auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
        media->pauseMusic();
    } else if (notification->getName() == Event::ResumeBackGroundMusic) {
        auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
        media->resumeMusic();
    }
}

int StageMediator::getSpriteStackSize() const {
    return (int)this->mSpriteStack.size();
}

void StageMediator::pushSpriteStack(int stackSize) {
    if (this->mScene == NULL) {
        return;
    }
    // 压入一个精灵栈
    auto view = SpriteStack::create();
    view->setStackSize(stackSize);
    this->mViewHoder->addChild(view);
    this->mSpriteStack.push_back(view);
}

void StageMediator::popSpriteStack() {
    if (this->mScene == NULL || this->mSpriteStack.size() == 0) {
        return;
    }
    // 弹出一个精灵栈
    this->mSpriteStack.back()->removeFromParent();
    this->mSpriteStack.pop_back();
}

void StageMediator::popAllSpriteStack() {
    while (mSpriteStack.size() != 0) {
        this->popSpriteStack();
    }
}

StageMediator::SpriteStack* StageMediator::topSpriteStack() const {
    if (this->mSpriteStack.size() != 0) {
        return this->mSpriteStack.back();
    }
    return NULL;
}

StageMediator::SpriteStack* StageMediator::getSpriteStack(int index) const {
    if (index >= 0 && index < (int)this->mSpriteStack.size()) {
        return this->mSpriteStack[index];
    }
    return nullptr;
}

cocos2d::Node* StageMediator::getViewHolder() const {
    return this->mViewHoder;
}

void StageMediator::getGameScreenShot(std::string const& screenshot_name, ScreenShotCallBack callback) {
    auto director = cocos2d::Director::getInstance();
    auto winSize = director->getWinSize();
    auto screen = cocos2d::RenderTexture::create(winSize.width, winSize.height);
    auto view = this->getViewHolder();
    screen->begin();
    view->visit();
    screen->end();
    
    this->mShotCall = callback;
    screen->saveToFile(screenshot_name, cocos2d::Image::Format::PNG, true, [&](cocos2d::RenderTexture* render, std::string const& fileName) {
        if (this->mShotCall != nullptr) {
            this->mShotCall(fileName);
        }
    });
}

void StageMediator::makeFirstScene() {
    if (mScene != NULL) {
        CCLOG("StageMediator MakeFirstScene fail.");
        return;
    }
    // 创建第一个场景
    this->mViewHoder = cocos2d::Layer::create();
    auto director = Director::getInstance();
    auto scene = Scene::create();
    scene->addChild(this->mViewHoder);
    director->runWithScene(scene);
    this->mScene = scene;
    
    // ==============================================================================
    // 加载一个ROM
    auto res = BF::App::getProxy<BF::ResProxy>(BF::ResProxy::Name);
    res->loadResPack();
    
    auto script = BF::App::getProxy<BF::StoryScriptProxy>(BF::StoryScriptProxy::Name);
    script->doScript("main.js");
    
    // ==============================================================================
    // 添加键盘事件
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = [](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){
        cocos2d::log ("KeyPress:%d" , keyCode );
    };
    listener->onKeyReleased = [](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){
        cocos2d::log ("KeyRelease:%d" , keyCode );
    };
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority ( listener, scene );
}

void StageMediator::onRegister() {
    mSpriteStack.reserve(10);
}

void StageMediator::onRemove() {
    mSpriteStack.clear();
}

NS_BF_END
