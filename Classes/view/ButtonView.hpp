//
//  ButtonView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#ifndef ButtonView_hpp
#define ButtonView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "Sprite.hpp"
#include "cocos/ui/CocosGUI.h"

NS_BF_BEGIN

class ButtonSpriteState : public SimpleSpriteState {
public:
    ButtonSpriteState();
public:
    virtual void Set(AvgSprite* sprite);
    virtual void Get(AvgSprite* sprite);
private:
    std::string mNormalName;
    std::string mSelectedName;
    std::string mDisabledName;
    std::string mCode;
public:
    template<class Archive>
    void save(Archive & archive) const {
        SimpleSpriteState const& super = *this;
        archive(super);
        archive(mNormalName, mSelectedName, mDisabledName);
        archive(mCode);
    }
    template<class Archive>
    void load(Archive & archive) {
        SimpleSpriteState& super = *this;
        archive(super);
        archive(mNormalName, mSelectedName, mDisabledName);
        archive(mCode);
    }
};

class ResProxy;
class ButtonView : public cocos2d::ui::Button, public BF::AvgSprite {
    friend class ButtonSpriteState;
public:
    CREATE_FUNC(ButtonView);
    ButtonView();
public:
    void loadImageFromResProxy(std::string const& normal, std::string const& selected, std::string const& disabled, bool Scale9 = false, ResProxy* res = nullptr);
    void setJSCode(std::string const& code);
public:
    std::string const& getJSCode() const;
public:
    template<class Archive>
    void serialize(Archive & archive) {
        //需要进行存储的变量
    }
    virtual SpriteStatePtr getSpriteState();
private:
    bool onTouchBegan(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    void onTouchMoved(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    void onTouchEnded(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    void onTouchCancel(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    virtual void onEnter();
    virtual void onExit();
private:
    cocos2d::Label* mTitle;
    std::string mNormalName;
    std::string mSelectedName;
    std::string mDisabledName;
    std::string mCode;
};

NS_BF_END

#endif /* ButtonView_hpp */
