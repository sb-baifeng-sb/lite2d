//
//  MusicLibraryView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#ifndef MusicLibraryView_hpp
#define MusicLibraryView_hpp

#include <stdio.h>
#include "../../common/CcbBaseView.hpp"

NS_BF_BEGIN

class MusicLibraryItemView;
class MusicLibraryView : public CcbBaseView {
public:
    typedef struct {
        bool Unlock;
        std::string Name;
        std::string Path;
    } MusicInfo;
    typedef std::vector<MusicInfo> MusicInfoArray;
public:
    CREATE_FUNC(MusicLibraryView);
    MusicLibraryView();
public:
    static MusicLibraryView* nodeFrom(std::string const& ccbName);
public:
    void setSourceData(MusicInfoArray const& data);
    void setPageIndex(int index);
    void loadPageData(int index);
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
    int mIndex;
    cocos2d::Label* mTitle;
    cocos2d::MenuItemImage* mBtnPrev;
    cocos2d::MenuItemImage* mBtnNext;
    MusicLibraryItemView* mItemView[12];
    std::string mTitleStr;
    MusicInfoArray mInfoArray;
};

CCB_VIEW_LOADER(MusicLibraryView);

class MusicLibraryItemView : public CcbBaseView {
public:
    typedef MusicLibraryView::MusicInfo MusicInfo;
    typedef std::function<void(cocos2d::Ref*)> ClickFunc;
public:
    CREATE_FUNC(MusicLibraryItemView);
    MusicLibraryItemView();
public:
    void setClickCallBack(ClickFunc func);
public:
    int getNumber() const;
    void setNumber(int number);
public:
    void setItemData(MusicInfo const& data);
    MusicInfo const& getItemData() const;
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
private:
    int mNumber;
    cocos2d::Label* mTitle;
    cocos2d::LayerColor* mMask;
    ClickFunc mClickCall;
    MusicInfo mData;
};

CCB_VIEW_LOADER(MusicLibraryItemView);

NS_BF_END

#endif /* MusicLibraryView_hpp */
