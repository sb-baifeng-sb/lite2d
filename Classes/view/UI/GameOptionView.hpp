//
//  GameOptionView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#ifndef GameOptionView_hpp
#define GameOptionView_hpp

#include <stdio.h>
#include "../../common/CcbBaseView.hpp"

NS_BF_BEGIN

class GameOptionVolumeBar;
class GameOptionView : public CcbBaseView {
public:
    CREATE_FUNC(GameOptionView);
    GameOptionView();
public:
    static GameOptionView* nodeFrom(std::string const& ccbName);
private:
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    void onBack(cocos2d::Ref* sender);
    void onOutEnd();
private:
    GameOptionVolumeBar* mBar[2];
};

CCB_VIEW_LOADER(GameOptionView);

class GameOptionVolumeBar : public CcbBaseView {
public:
    typedef cocos2d::ClippingNode ClipNode;
    typedef std::function<void(float)> ProgressBarChanedCallback;
public:
    CREATE_FUNC(GameOptionVolumeBar);
    GameOptionVolumeBar();
public:
    void setValueChangedCallback(ProgressBarChanedCallback callback);
    void setValue(float v);
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    virtual bool onClipTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onClipTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onClipTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onClipTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
private:
    ClipNode* mClipNode;
    cocos2d::Label* mValue;
    cocos2d::Size mDefSize;
    ProgressBarChanedCallback mChanged;
};

CCB_VIEW_LOADER(GameOptionVolumeBar);
CCB_VIEW_LOADER_DEFINE(GameOptionClipNode, cocosbuilder::NodeLoader, GameOptionVolumeBar::ClipNode);

NS_BF_END

#endif /* GameOptionView_hpp */
