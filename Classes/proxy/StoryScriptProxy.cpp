//
//  StoryScriptProxy.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/2.
//

#include "StoryScriptProxy.hpp"
#include "ResProxy.hpp"
#include "JSApiRegisterFunc.hpp"
#include "app/App.hpp"
#include "tools/script.hpp"
#include "tools/jsengine.hpp"
#include "common/CharsetConvert.hpp"

NS_BF_BEGIN

std::string const StoryScriptProxy::Name = "StoryScriptProxy";

StoryScriptProxy::StoryScriptProxy():
    PureMVC::Proxy(Name), mMode(MODE_NORMAL), mPlayTime(0.0f), mUpdate(false) {
    mStack.reserve(8);
}

void StoryScriptProxy::update(float dt) {
    this->onUpdate(dt);
    this->mPlayTime += dt;
}

void StoryScriptProxy::setMode(ScriptMode mode) {
    if (this->mMode != mode) {
        this->mMode = mode;
        mSpeakHolder->setMode(mode);
    }
}

StoryScriptProxy::ScriptMode StoryScriptProxy::getMode() const {
    return this->mMode;
}

void StoryScriptProxy::startUpdate() {
    if (this->mUpdate) {
        return;
    }
    this->mUpdate = true;
    auto director = cocos2d::Director::getInstance();
    director->getScheduler()->scheduleUpdate(this, 0, false);
}

void StoryScriptProxy::stopUpdate() {
    if (!this->mUpdate) {
        return;
    }
    this->mUpdate = false;
    auto director = cocos2d::Director::getInstance();
    director->getScheduler()->unscheduleUpdate(this);
}

void StoryScriptProxy::onRegister() {
    mEngine = std::make_shared<JSEngine>();
    mEngine->setErrorLogFunc([](std::string const& msg) {
        cocos2d::log("error: %s", msg.c_str());
    });
    mEngine->setInfoLogFunc([](std::string const& msg) {
        cocos2d::log("info: %s", msg.c_str());
    });
    mEngine->addRegisterFunc(register_story_api);
    mEngine->fire();
    setJSSugarGlobalContext(mEngine->getJSContext());
    
    mSpeakHolder = SpeakDelegateHolderPtr(new SpeakDelegateHolder);
    mSpeakHolder->setMode(this->mMode);
    
    this->startUpdate();
}

void StoryScriptProxy::onRemove() {
    setJSSugarGlobalContext(NULL);
    mStack.clear();
    
    mSpeakHolder = nullptr;
    
    this->stopUpdate();
}

void StoryScriptProxy::setPlayTime(int seconds) {
    this->mPlayTime = seconds;
}

int StoryScriptProxy::getPlayTime() const {
    return (int)this->mPlayTime;
}

void StoryScriptProxy::onDoScript(std::string const& line) {
    // 执行一条剧情脚本
	unsigned char c = line[0];
    if (c == '*') {
        // 标签跳过不处理
        return;
    } else if (c == '\t') {
        // 文本
        std::string code = "speak('" + line.substr(1) + "');";
        mEngine->doCode(code);
    } else {
        // 脚本
        mEngine->doCode(line);
    }
}

void StoryScriptProxy::onUpdate(float dt) {
    this->step(dt);
}

StoryScriptProxy::JSEnginePtr StoryScriptProxy::getJSEngine() const {
    return this->mEngine;
}

StoryScriptProxy::SpeakDelegateHolderPtr StoryScriptProxy::getSpeakHolder() const {
    return this->mSpeakHolder;
}

StoryScriptProxy::StoryScriptNotePtr StoryScriptProxy::makeScriptNote(StoryScriptPtr const& p) const {
    StoryScriptNotePtr ptr(new StoryScriptNote);
    ptr->fileName = p->getName();
    ptr->curTime = p->getCurTime();
    ptr->endTime = p->getEndTime();
    ptr->scriptIndex = p->getIndex();
    return ptr;
}

void StoryScriptProxy::loadScriptNode(StoryScriptNotePtr const& p) {
    this->runScript(p->fileName);
    auto& script = mStack[mStack.size()-1];
    script->setPauseTime(p->curTime, p->endTime);
    script->seek(p->scriptIndex);
}

void StoryScriptProxy::popScript() {
    if (mStack.size() != 0) {
        mStack.pop_back();
    }
}

void StoryScriptProxy::runScript(std::string const& filename) {
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto data = res->getStoryScriptData(filename);
    if (data != nullptr) {
        StoryScriptPtr ptr(new storyScript());
        ptr->load(&(*data));
        mStack.push_back(ptr);
    }
}

void StoryScriptProxy::jumpScript(std::string const& filename) {
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto data = res->getStoryScriptData(filename);
    if (data != nullptr) {
        StoryScriptPtr ptr(new storyScript());
        ptr->load(&(*data));
        if (mStack.size() != 0) {
            mStack.pop_back();
        }
        mStack.push_back(ptr);
    }
}

void StoryScriptProxy::doScript(std::string const& filename) {
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto data = res->getData(filename);
    if (data->isNull()) {
        return;
    }
    std::string code((char*)data->getBytes(), data->getSize());
    mEngine->doCode(code, filename);
}

void StoryScriptProxy::doCode(std::string const& code) {
    mEngine->doCode(code);
}

bool StoryScriptProxy::isEmpty() const {
    return mStack.size() == 0;
}

int StoryScriptProxy::getStackSize() const {
    return (int)mStack.size();
}

void StoryScriptProxy::setScriptPopCallBack(StoryScriptPopCallBack func) {
    this->mScriptPopCallBack = func;
}

void StoryScriptProxy::step(float dt) {
    if (isEmpty()) {
        return;
    }
    auto script = mStack.back();
    script->update(dt);
    
    if (script->isPause()) {
        // 暂停不执行脚本
        return;
    }
    auto speak = mSpeakHolder;
    if (!speak->isSpeakFinish()) {
        // 对话没结束不执行脚本
        return;
    }
    // 执行一行脚本
    script->line(std::bind(&StoryScriptProxy::onDoScript, this, std::placeholders::_1));
    if (script->isEnd() && (&(*script) == &(*mStack.back()))) {
        // 弹出已经执行完毕的脚本
        mStack.pop_back();
        if (this->mScriptPopCallBack != nullptr) {
            this->mScriptPopCallBack(script->getName());
        }
    }
}

void StoryScriptProxy::seek(std::string const& tag) {
    if (isEmpty()) {
        return;
    }
    mStack.back()->seek(tag);
}

void StoryScriptProxy::pause(float seconds) {
    if (isEmpty()) {
        return;
    }
    mStack.back()->pause(seconds);
}

void StoryScriptProxy::back_step(int step) {
    if (isEmpty()) {
        return;
    }
    mStack.back()->back(step);
}

void StoryScriptProxy::clear() {
    mStack.clear();
    mSpeakHolder->removeAllDelegate();
}

// ==========================================================================

SpeakDelegateHolder::SpeakDelegateHolder():mMode(Mode::MODE_NORMAL) {
    
}

SpeakDelegateHolder::~SpeakDelegateHolder() {
    this->removeAllDelegate();
}

void SpeakDelegateHolder::setMode(Mode mode) {
    if (this->mMode != mode) {
        this->mMode = mode;
        this->travel([mode](RealDelegate* delegate) {
            delegate->setSpeakMode(mode);
        });
    }
}

bool SpeakDelegateHolder::isSpeakFinish() const {
    for (int i=0; i < this->mList.size(); i++) {
        auto delegate = dynamic_cast<RealDelegate*>(this->mList[i]);
        if (!delegate->isSpeakFinish()) {
            return false;
        }
    }
    return true;
}

void SpeakDelegateHolder::ClickSpeak() {
    this->travel([](RealDelegate* delegate) {
        delegate->onSpeakClick();
    });
}

bool SpeakDelegateHolder::addDelegate(Delegate* delegate) {
    for (int i=0; i < this->mList.size(); i++) {
        if (this->mList[i] == delegate) {
            return false;
        }
    }
    auto real = dynamic_cast<RealDelegate*>(delegate);
    if (real != nullptr) {
        real->setSpeakMode(this->mMode);
        this->mList.push_back(delegate);
        delegate->retain();
        return true;
    }
    return false;
}

bool SpeakDelegateHolder::removeDelegate(Delegate* delegate) {
    for (int i=0; i < this->mList.size(); i++) {
        if (this->mList[i] == delegate) {
            auto delegate = this->mList[i];
            delegate->release();
            this->mList.erase(this->mList.begin()+i);
            return true;
        }
    }
    return false;
}

void SpeakDelegateHolder::removeAllDelegate() {
    for (int i=0; i < this->mList.size(); i++) {
        auto delegate = this->mList[i];
        delegate->release();
    }
    this->mList.clear();
}

void SpeakDelegateHolder::travel(std::function<void(RealDelegate* delegate)> func) {
    if (func == nullptr) {
        return;
    }
    auto& list = this->mList;
    for (int i=0; i < list.size(); i++) {
        auto delegate = dynamic_cast<RealDelegate*>(list[i]);
        func(delegate);
    }
}

NS_BF_END
