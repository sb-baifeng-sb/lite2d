//
//  SpeakView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/21.
//

#include "SpeakView.hpp"
#include "proxy/ResProxy.hpp"
#include "app/App.hpp"
#include "GifView.hpp"

USING_NS_CC;

NS_BF_BEGIN

// ============================================================================

HalfSpeakViewState::HalfSpeakViewState() {
    
}

void HalfSpeakViewState::Set(AvgSprite* sprite) {
    auto view = sprite->type<HalfSpeakView>();
    if (view != nullptr) {
        view->setSpeakMode(this->mSpeakMode);
        view->setPadding(this->mTop, this->mLeft, this->mBottom, this->mRight);
        view->setAlignment(this->mAlign);
        view->setSpeed(this->mSpeed);
        view->loadIconFromResProxy(this->mIconFile);
        view->setIconSize(this->mIconSize);
        view->setString(this->mString);
        view->mSpeakFinish = this->mFinish;
    }
    SimpleSpriteState::Set(sprite);
}

void HalfSpeakViewState::Get(AvgSprite* sprite) {
    SimpleSpriteState::Get(sprite);
    auto view = sprite->type<HalfSpeakView>();
    if (view != nullptr) {
        this->mTop = view->mPadding.top;
        this->mLeft = view->mPadding.left;
        this->mBottom = view->mPadding.bottom;
        this->mRight = view->mPadding.right;
        this->mAlign = view->mTextView->getTextAlignment();
        this->mSpeakMode = view->mSpeakMode;
        this->mSpeed = view->mSpeakPage->getSpeed();
        this->mIconSize = view->mGifView->getContentSize();
        this->mIconFile = view->mGifView->getFileName();
        this->mString = view->mTextView->getString();
        this->mFinish = view->isSpeakFinish();
    }
}

// ============================================================================

HalfSpeakView::HalfSpeakView():
    mTextView(NULL),
    mGifView(NULL),
    mSpeakFinish(false),
    BF::AvgSprite(TYPE_HALF_SPEAK, this) {
    memset(&mPadding, 0, sizeof(mPadding));
}

bool HalfSpeakView::init() {
    if (Node::init()) {
        initView();
        return true;
    }
    return false;
}

void HalfSpeakView::initView() {
    Size const& winSize = Director::getInstance()->getWinSize();
    this->setContentSize(Size(winSize.width, winSize.height/2));
    
    this->mTextView = Label::create();
    this->mTextView->setAnchorPoint(Point(0.5, 1));
    this->mTextView->setBMFontFilePath("font/sets.fnt");
    this->mTextView->setAlignment(TextHAlignment::LEFT);
    this->addChild(mTextView);
    
    this->mGifView = GifView::create();
    //this->mGifView->loadFromResProxy("speak/icon_01.gif");
    this->mGifView->setVisible(false);
    this->addChild(mGifView);
    
    this->mTextView->setPosition(winSize.width/2, 190);
    this->mTextView->setAnchorPoint(Vec2(0.5f, 1.0f));
    this->setSpeakMode(SpeakMode::MODE_NORMAL);
}

void HalfSpeakView::setSpeed(float speed) {
    this->mSpeakPage->setSpeed(speed);
}

void HalfSpeakView::setAlignment(cocos2d::TextHAlignment align) {
    this->mTextView->setAlignment(align);
}

void HalfSpeakView::setPadding(int top, int left, int bottom, int right) {
    Size const& size = this->getContentSize();
    Rect rect((size.width-(left+right))*0.5f+left, size.height-top, size.width-(left+right), size.height-(top+bottom));
    this->mTextView->setPosition(rect.origin.x, rect.origin.y);
    this->mTextView->setWidth(rect.size.width);
    this->mTextView->setHeight(rect.size.height);
    this->mSpeakPage->setPageSize(rect.size);
    
    this->mPadding.top = top;
    this->mPadding.left = left;
    this->mPadding.bottom = bottom;
    this->mPadding.right = right;
}

void HalfSpeakView::loadIconFromResProxy(std::string const& filename) {
    this->mGifView->loadFromResProxy(filename);
}

void HalfSpeakView::setIconSize(cocos2d::Size const& size) {
    this->mGifView->setContentSize(size);
}

void HalfSpeakView::setString(std::string const& text) {
    this->mSpeakFinish = false;
    this->mSpeakPage->setString(text);
}

void HalfSpeakView::onRegister() {
    auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    proxy->getSpeakHolder()->addDelegate(this);
}

void HalfSpeakView::onRemove() {
    auto proxy = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    proxy->getSpeakHolder()->removeDelegate(this);
}

cocos2d::Label* HalfSpeakView::getLabel() const {
    return this->mTextView;
}

HalfSpeakView::SpriteStatePtr HalfSpeakView::getSpriteState() {
    SpriteStatePtr state(new HalfSpeakViewState());
    state->Get(this);
    return state;
}

bool HalfSpeakView::isSpeakFinish() {
    if (this->mSpeakFinish) {
        return true;
    }
    return this->mSpeakPage->isFinish();
}

void HalfSpeakView::onSpeakClick() {
    this->mSpeakPage->click();
}

void HalfSpeakView::setSpeakMode(SpeakMode mode) {
    if (mode == SpeakMode::MODE_NORMAL) {
        mSpeakPage = SpeakPagePtr(new SpeakPageNormal(this->mTextView, this->mGifView));
    } else if (mode == SpeakMode::MODE_SKIP) {
        mSpeakPage = SpeakPagePtr(new SpeakPageSkip(this->mTextView, this->mGifView));
    } else if (mode == SpeakMode::MODE_AUTO) {
        mSpeakPage = SpeakPagePtr(new SpeakPageAuto(this->mTextView, this->mGifView));
    }
    this->mSpeakMode = mode;
}

// ============================================================================

SpeakPageNormal::SpeakPageNormal(cocos2d::Label* label, GifView* icon):mLabel(label), mIcon(icon), mfSpeed(1.0f), mFinish(true), mStrLength(0) {
    auto director = cocos2d::Director::getInstance();
    director->getScheduler()->setTimeScale(1.0f);
}

SpeakPageNormal::~SpeakPageNormal() {
    
}

void SpeakPageNormal::setSpeed(float speed) {
    this->mfSpeed = speed;
}

float SpeakPageNormal::getSpeed() const {
    return this->mfSpeed;
}

void SpeakPageNormal::setPageSize(cocos2d::Size const& size) {
    this->mSize = size;
}

void SpeakPageNormal::setString(std::string const& text) {
    
    this->mFinish = false;
    
    std::string realText;
    if (text.length() > 0 && text.back() == 'W') {
        realText = text;
    } else {
        realText = text + "W";
    }

    int length = this->mLabel->getStringLength();
    for (int i=0; i < length; i++) {
        auto view = this->mLabel->getLetter(i);
        if (view != nullptr) {
            view->stopAllActions();
            view->setVisible(true);
        }
    }
    this->mIcon->stop();
    
    if (text.length() == 0) {
        this->mFinish = true;
        this->mStrLength = 0;
        this->mLabel->setString("");
        return;
    }
    
    this->mLabel->setString(realText);
    this->mStrLength = length = this->mLabel->getStringLength();
    
    // 计算图标位置
    auto lastLetter = this->mLabel->getLetter(length-1);
    if (lastLetter != nullptr) {
        auto point = mLabel->getParent()->convertToNodeSpace( mLabel->convertToWorldSpace(lastLetter->getPosition()) );
        point.x += 5.0f;
        this->mIcon->setPosition(point);
        lastLetter->setVisible(false);
    }
    
    float beginSecond = 0.0f;
    float fadeSpeed = 0.2f;
    float waitSecond = fadeSpeed * 0.25f;
    for (int i=0; i < length; i++) {
        auto view = this->mLabel->getLetter(i);
        if (view != nullptr) {
            auto delay = DelayTime::create(beginSecond);
            auto fadein = FadeIn::create( fadeSpeed * this->mfSpeed );
            view->setOpacity(0);
            beginSecond += waitSecond * this->mfSpeed;
            if (i+1 == length) {
                // 最后一个动作增加显示图标逻辑
                static GifView* gif;
                gif = this->mIcon;
                auto func = CallFunc::create(std::bind([&]() {
                    gif->play();
                }));
                auto action = Sequence::create(delay, fadein, func, nullptr);
                view->runAction(action);
                view->setVisible(false);
                continue;
            }
            auto action = Sequence::create(delay, fadein, nullptr);
            view->runAction(action);
        }
    }
}

void SpeakPageNormal::click() {
    // 检查文本是否显示完毕
    if (this->mIcon->isPlaying()) {
        this->mFinish = true;
        return;
    }
    if (this->mStrLength != 0) {
        int length = this->mStrLength;
        for (int i=0; i < length; i++) {
            auto view = this->mLabel->getLetter(i);
            if (view != nullptr) {
                view->stopAllActions();
                view->setOpacity(255);
            }
        }
        this->mIcon->play();
        return;
    }
}

bool SpeakPageNormal::isFinish() {
    return this->mFinish;
}

// ============================================================================

SpeakPageAuto::SpeakPageAuto(cocos2d::Label* label, GifView* icon):SpeakPageNormal(label, icon) {
    auto director = cocos2d::Director::getInstance();
    director->getScheduler()->setTimeScale(1.0f);
}

SpeakPageAuto::~SpeakPageAuto() {
    this->mIcon->stopAllActions();
}

void SpeakPageAuto::setString(std::string const& text) {
    
    this->mFinish = false;
    
    std::string realText;
    if (text.length() > 0 && text.back() == 'W') {
        realText = text;
    } else {
        realText = text + "W";
    }
    
    int length = this->mLabel->getStringLength();
    for (int i=0; i < length; i++) {
        auto view = this->mLabel->getLetter(i);
        if (view != nullptr) {
            view->stopAllActions();
            view->setVisible(true);
        }
    }
    this->mIcon->stop();
    
    if (text.length() == 0) {
        this->mFinish = true;
        this->mStrLength = 0;
        this->mLabel->setString("");
        return;
    }
    
    this->mLabel->setString(realText);
    this->mStrLength = length = this->mLabel->getStringLength();
    
    // 计算图标位置
    auto lastLetter = this->mLabel->getLetter(length-1);
    if (lastLetter != nullptr) {
        auto point = mLabel->getParent()->convertToNodeSpace( mLabel->convertToWorldSpace(lastLetter->getPosition()) );
        point.x += 5.0f;
        this->mIcon->setPosition(point);
        lastLetter->setVisible(false);
    }
    
    float beginSecond = 0.0f;
    float fadeSpeed = 0.2f;
    float waitSecond = fadeSpeed * 0.25f;
    for (int i=0; i < length; i++) {
        auto view = this->mLabel->getLetter(i);
        if (view != nullptr) {
            auto delay = DelayTime::create(beginSecond);
            auto fadein = FadeIn::create( fadeSpeed * this->mfSpeed );
            view->setOpacity(0);
            beginSecond += waitSecond * this->mfSpeed;
            if (i+1 == length) {
                // 最后一个动作增加显示图标逻辑
                static GifView* gif;
                gif = this->mIcon;
                auto func = CallFunc::create(std::bind([&]() {
                    gif->play();
                }));
                auto action = Sequence::create(delay, fadein, func, nullptr);
                view->runAction(action);
                view->setVisible(false);
                continue;
            }
            auto action = Sequence::create(delay, fadein, nullptr);
            view->runAction(action);
        }
    }
}

// ============================================================================

SpeakPageSkip::SpeakPageSkip(cocos2d::Label* label, GifView* icon):SpeakPageNormal(label, icon) {
    this->setString(label->getString());
    auto director = cocos2d::Director::getInstance();
    director->getScheduler()->setTimeScale(2.0f);
}

SpeakPageSkip::~SpeakPageSkip() {
    this->mLabel->stopAllActions();
}

void SpeakPageSkip::setString(std::string const& text) {
    
    this->mFinish = false;
    
    std::string realText;
    if (text.length() > 0 && text.back() == 'W') {
        realText = text;
    } else {
        realText = text + "W";
    }
    
    int length = this->mLabel->getStringLength();
    for (int i=0; i < length; i++) {
        auto view = this->mLabel->getLetter(i);
        if (view != nullptr) {
            view->stopAllActions();
            view->setVisible(true);
        }
    }
    this->mIcon->stop();
    this->mLabel->stopAllActions();
    
    if (text.length() == 0) {
        this->mFinish = true;
        this->mStrLength = 0;
        this->mLabel->setString("");
        return;
    }
    
    this->mLabel->setString(realText);
    this->mStrLength = length = this->mLabel->getStringLength();
    
    // 计算图标位置
    auto lastLetter = this->mLabel->getLetter(length-1);
    if (lastLetter != nullptr) {
        auto point = mLabel->getParent()->convertToNodeSpace( mLabel->convertToWorldSpace(lastLetter->getPosition()) );
        point.x += 5.0f;
        this->mIcon->setPosition(point);
        lastLetter->setVisible(false);
    }
    
    for (int i=0; i < length; i++) {
        auto view = this->mLabel->getLetter(i);
        if (view != nullptr) {
            view->setVisible(true);
            view->stopAllActions();
            view->setOpacity(255);
        }
    }
    this->mIcon->play();
    
    auto action = cocos2d::Sequence::create(cocos2d::DelayTime::create(0.05f), cocos2d::CallFunc::create([&]() {
        this->mFinish = true;
    }), NULL);
    this->mLabel->runAction(action);
}

void SpeakPageSkip::click() {
    
}

NS_BF_END
