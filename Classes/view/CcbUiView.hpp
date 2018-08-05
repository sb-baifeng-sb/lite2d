//
//  CcbUiView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/21.
//

#ifndef CcbUiView_hpp
#define CcbUiView_hpp

#include <stdio.h>
#include "common/CcbBaseView.hpp"
#include "extensions/cocos-ext.h"
#include "Sprite.hpp"

NS_BF_BEGIN

class CcbUiViewState : public SimpleSpriteState {
public:
    typedef std::map<std::string, std::string> CodeMap;
    typedef std::map<std::string, std::string> AniCodeMap;
public:
    CcbUiViewState();
public:
    virtual void Set(AvgSprite* sprite);
    virtual void Get(AvgSprite* sprite);
public:
    template<class Archive>
    void save(Archive & archive) const {
        SimpleSpriteState const& super = *this;
        archive(super);
        archive(mCcbName, mClassName);
        archive(mCodes, mAniCodes);
    }
    template<class Archive>
    void load(Archive & archive) {
        SimpleSpriteState& super = *this;
        archive(super);
        archive(mCcbName, mClassName);
        archive(mCodes, mAniCodes);
    }
public:
    std::string const& getCcbName() const;
    std::string const& getClassName() const;
private:
    std::string mCcbName;
    std::string mClassName;
    CodeMap mCodes;
    CodeMap mAniCodes;
};

class CcbUiView : public CcbBaseView, public BF::AvgSprite {
    friend class CcbUiViewState;
public:
    typedef std::map<std::string, std::string> CodeMap;
    typedef std::function<void(std::string const& code)> CodeExcute;
    typedef std::vector<std::string> SelectorVector;
    typedef std::map<std::string, std::string> AnimationCodeMap;
public:
    static CcbUiView* New(std::string const& fileName, std::string const& className);
public:
    CREATE_FUNC(CcbUiView);
    CcbUiView();
public:
    void setCodeExcute(CodeExcute exc);
    void setSelectorAndCode(std::string const& selectorName, std::string const& code);
    void setAnimationEndCallbackCode(std::string const& aniName, std::string const& code);
public:
    SelectorVector const& getSelectorVector() const;
    virtual SpriteStatePtr getSpriteState();
private:
    cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    void onMenuClick(cocos2d::Ref* sender);
    void onControlClick(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type);
private:
    std::string mCcbName;
    std::string mClassName;
    CodeMap mCodes;
    CodeExcute mExcute;
    SelectorVector mSelectorVector;
    AnimationCodeMap mAniCodes;
};

CCB_VIEW_LOADER(CcbUiView);

NS_BF_END

#endif /* CcbUiView_hpp */
