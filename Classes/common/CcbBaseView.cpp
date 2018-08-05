//
//  CcbView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/20.
//

#include "CcbBaseView.hpp"

NS_BF_BEGIN

//======================================================================================
class CcbAnimationCompletedDelegate : public cocos2d::Ref, public cocosbuilder::CCBAnimationManagerDelegate {
public:
    typedef std::function<void(std::string const&)> AnimationCompletedCallback;
    typedef std::map<std::string, AnimationCompletedCallback> ListenerMap;
public:
    CREATE_FUNC(CcbAnimationCompletedDelegate);
public:
    void setAnimationCompletedCallback(std::string const& aniName, AnimationCompletedCallback callback) {
        mListeners[aniName] = callback;
    }
    void clearListeners() {
        mListeners.clear();
    }
private:
    bool init() {
        return true;
    }
private:
    virtual void completedAnimationSequenceNamed(const char *name) {
        std::string _name(name);
        auto iter = mListeners.find(_name);
        if (iter != mListeners.end() && iter->second != nullptr) {
            iter->second(_name);
        }
    }
private:
    ListenerMap mListeners;
};

//======================================================================================
CCB::CCBPtr CCB::New() {
    CCBPtr ptr(new CCB);
    return ptr;
}

CCB::CCB():mLoader(nullptr) {
    
}

CCB::~CCB() {
    
}

void CCB::buildLoader() {
    if (mLoader == nullptr) {
        mLoader = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    }
}

void CCB::registerClass(std::string const& className, cocosbuilder::NodeLoader* loader) {
    this->buildLoader();
    mLoader->registerNodeLoader(className.c_str(), loader);
}

void CCB::removeClass(std::string const& className) {
    this->buildLoader();
    mLoader->unregisterNodeLoader(className.c_str());
}

cocos2d::Node* CCB::nodeFromCCB(std::string const& fileName) {
    this->buildLoader();
    cocosbuilder::CCBReader* reader = new cocosbuilder::CCBReader(mLoader);
    reader->autorelease();
    return reader->readNodeGraphFromFile(fileName.c_str());
}

//======================================================================================
CcbBaseView::CcbBaseView():mReader(nullptr), mAniCompleted(CcbAnimationCompletedDelegate::create()) {
    mAniCompleted->retain();
    
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(CcbBaseView::onTouchBegan, this);
    this->registerTouchListener(listener);
}

CcbBaseView::~CcbBaseView() {
    CC_SAFE_RELEASE(this->mAniCompleted);
    CC_SAFE_RELEASE(this->mReader);
}

void CcbBaseView::registerTouchListener(cocos2d::EventListener* listener) {
    auto dispatcher = this->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CcbBaseView::removeTouchListener() {
    auto dispatcher = this->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

void CcbBaseView::setCCBReader(cocosbuilder::CCBReader* reader) {
    CC_SAFE_RELEASE(this->mReader);
    this->mReader = reader;
    CC_SAFE_RETAIN(this->mReader);
    
    if (reader != nullptr) {
        this->mReader->getAnimationManager()->setDelegate(mAniCompleted);
    }
}

cocosbuilder::CCBReader* CcbBaseView::getCCBReader() const {
    return this->mReader;
}

void CcbBaseView::setAnimationCompletedCallBack(std::string const& aniName, AnimationCompletedCallback callback) {
    this->mAniCompleted->setAnimationCompletedCallback(aniName, callback);
}

void CcbBaseView::playAnimation(std::string const& aniName, float fTweenDuration) {
    this->mReader->getAnimationManager()->runAnimationsForSequenceNamedTweenDuration(aniName.c_str(), fTweenDuration);
}

void CcbBaseView::removeAllAnimationCompletedCallBack() {
    this->mAniCompleted->clearListeners();
}

bool CcbBaseView::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        pEvent->stopPropagation();
        return true;
    }
    return false;
}

NS_BF_END
