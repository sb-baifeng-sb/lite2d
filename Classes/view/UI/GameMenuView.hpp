//
//  GameMenuView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/7.
//

#ifndef GameMenuView_hpp
#define GameMenuView_hpp

#include <stdio.h>
#include "common/CcbBaseView.hpp"

NS_BF_BEGIN

class GameMenuView : public CcbBaseView {
public:
    CREATE_FUNC(GameMenuView);
    GameMenuView();
public:
    static GameMenuView* nodeFrom(std::string const& ccbName);
public:
    void start(cocos2d::Texture2D* screenshot);
    void setBackTitleScript(std::string const& code);
private:
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    void onBack(cocos2d::Ref* sender);
    void onAutoMode(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type);
    void onSkipMode(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type);
    void onBacklog(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type);
    void onSave(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type);
    void onLoad(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type);
    void onOption(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type);
    void onBackTitle(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type);
private:
    cocos2d::Node* mBox;
    cocos2d::Sprite* mScreenShot;
    cocos2d::LayerColor* mShotMask;
    std::string mTitleCode;
};

CCB_VIEW_LOADER(GameMenuView);

NS_BF_END

#endif /* GameMenuView_hpp */
