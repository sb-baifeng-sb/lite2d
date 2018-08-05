//
//  SpeakView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/21.
//

#ifndef SpeakView_hpp
#define SpeakView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "proxy/StoryScriptProxy.hpp"
#include "cocos2d.h"
#include "Sprite.hpp"

NS_BF_BEGIN

class HalfSpeakViewState : public SimpleSpriteState {
public:
    HalfSpeakViewState();
public:
    virtual void Set(AvgSprite* sprite);
    virtual void Get(AvgSprite* sprite);
private:
    bool mFinish;
    SpeakDelegate::SpeakMode mSpeakMode;
    int mTop, mLeft, mBottom, mRight;
    cocos2d::TextHAlignment mAlign;
    float mSpeed;
    std::string mString;
    std::string mIconFile;
    cocos2d::Size mIconSize;
public:
    template<class Archive>
    void save(Archive & archive) const {
        SimpleSpriteState const& super = *this;
        archive(super);
        archive(mFinish);
        archive(mTop, mLeft, mBottom, mRight);
        archive(mSpeakMode, mAlign, mSpeed, mString);
        archive(mIconFile, mIconSize.width, mIconSize.height);
    }
    template<class Archive>
    void load(Archive & archive) {
        SimpleSpriteState& super = *this;
        archive(super);
        archive(mFinish);
        archive(mTop, mLeft, mBottom, mRight);
        archive(mSpeakMode, mAlign, mSpeed, mString);
        archive(mIconFile, mIconSize.width, mIconSize.height);
    }
};

class GifView;
class SpeakPage;
class HalfSpeakView : public cocos2d::Node, public BF::AvgSprite, public SpeakDelegate {
    friend class HalfSpeakViewState;
public:
    typedef std::shared_ptr<SpeakPage> SpeakPagePtr;
    typedef struct {
        int top, left, bottom, right;
    } Padding;
public:
    CREATE_FUNC(HalfSpeakView);
    HalfSpeakView();
public:
    void setPadding(int top, int left, int bottom, int right);
    void setString(std::string const& text);
    void setSpeed(float speed); // 值越大速度越慢:0~10
    void setAlignment(cocos2d::TextHAlignment align);
public:
    void loadIconFromResProxy(std::string const& filename);
    void setIconSize(cocos2d::Size const& size);
public:
    cocos2d::Label* getLabel() const;
    virtual SpriteStatePtr getSpriteState();
private:
    void initView();
    virtual bool init();
private:
    virtual bool isSpeakFinish();
    virtual void onSpeakClick();
    virtual void setSpeakMode(SpeakMode mode);
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    bool mSpeakFinish;
    SpeakMode mSpeakMode;
    cocos2d::Label* mTextView;
    GifView* mGifView;
    SpeakPagePtr mSpeakPage;
    Padding mPadding;
};

class SpeakPage {
public:
    virtual ~SpeakPage(){}
public:
    virtual void setSpeed(float speed) = 0;
    virtual void setPageSize(cocos2d::Size const& size) = 0;
    virtual void setString(std::string const& text) = 0;
    virtual void click() = 0;
    virtual bool isFinish() = 0;
public:
    virtual float getSpeed() const {
        return 0.0f;
    }
};

class SpeakPageNormal : public SpeakPage {
public:
    SpeakPageNormal(cocos2d::Label* label, GifView* icon);
    ~SpeakPageNormal();
public:
    virtual void setSpeed(float speed);
    virtual void setPageSize(cocos2d::Size const& size);
    virtual void setString(std::string const& text);
    virtual void click();
    virtual bool isFinish();
public:
    virtual float getSpeed() const;
protected:
    bool mFinish;
    int mStrLength;
    GifView* mIcon;
    cocos2d::Label* mLabel;
    cocos2d::Size mSize;
    float mfSpeed;
};

class SpeakPageAuto : public SpeakPageNormal {
public:
    SpeakPageAuto(cocos2d::Label* label, GifView* icon);
    ~SpeakPageAuto();
private:
    virtual void setString(std::string const& text);
};

class SpeakPageSkip : public SpeakPageNormal {
public:
    SpeakPageSkip(cocos2d::Label* label, GifView* icon);
    ~SpeakPageSkip();
private:
    virtual void setString(std::string const& text);
    virtual void click();
};

NS_BF_END

#endif /* SpeakView_hpp */
