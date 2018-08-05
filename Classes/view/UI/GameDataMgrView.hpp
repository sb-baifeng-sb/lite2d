//
//  GameDataMgrView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#ifndef GameDataMgrView_hpp
#define GameDataMgrView_hpp

#include <stdio.h>
#include "../../common/CcbBaseView.hpp"
#include "cocos2d.h"

NS_BF_BEGIN

class GameDataItemView;
class GameDataMgrView : public CcbBaseView {
public:
    typedef enum {
        TYPE_NONE = 0,
        TYPE_SAVE = 1,
        TYPE_LOAD = 2
    } MgrType;
    typedef std::function<void(MgrType type)> CompletedFunc;
    typedef std::function<void()> ExitFunc;
public:
    CREATE_FUNC(GameDataMgrView);
    GameDataMgrView();
public:
    static GameDataMgrView* nodeFrom(std::string const& ccbName);
public:
    void setCompletedFunc(CompletedFunc func);
    void setExitFunc(ExitFunc func);
    void setMgrType(MgrType type);
    void setPageIndex(int index);
    void loadPageData(int index);
    void start(MgrType type);
private:
    bool loadGameData(int number);
    void saveGameData(int number);
private:
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    void onBack(cocos2d::Ref* sender);
    void onPrev(cocos2d::Ref* sender);
    void onNext(cocos2d::Ref* sender);
    void onItemClicked(cocos2d::Ref* sender);
    void onOutEnd();
private:
    static int mLoadIndex;
    static int mSaveIndex;
private:
    int mIndex;
    int mCurNumber;
    MgrType mType;
    cocos2d::Label* mTitle;
    cocos2d::MenuItemImage* mBtnPrev;
    cocos2d::MenuItemImage* mBtnNext;
    GameDataItemView* mItemView[8];
    std::string mTitleStr;
    CompletedFunc mCompletedFunc;
    ExitFunc mRemoveFunc;
};

CCB_VIEW_LOADER(GameDataMgrView);

class GameDataItemView : public CcbBaseView {
public:
    typedef std::function<void(cocos2d::Ref*)> ClickFunc;
public:
    CREATE_FUNC(GameDataItemView);
    GameDataItemView();
public:
    void setStyle(GameDataMgrView::MgrType type);
    void setClickCallBack(ClickFunc func);
    void loadData(int number);
public:
    int getNumber() const;
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
private:
    bool canClick() const;
private:
    int mNumber;
    cocos2d::Sprite* mBG;
    cocos2d::LayerColor* mColorMask;
    cocos2d::Sprite* mIconLoad;
    cocos2d::Sprite* mIcon;
    cocos2d::Label* mNO;
    cocos2d::Label* mTitle;
    cocos2d::Label* mPlayTime;
    cocos2d::Label* mSaveTime;
    std::string mGamePlayTimeText;
    std::string mGameSaveTimeText;
    ClickFunc mClickCall;
};

CCB_VIEW_LOADER(GameDataItemView);

NS_BF_END

#endif /* GameDataMgrView_hpp */
