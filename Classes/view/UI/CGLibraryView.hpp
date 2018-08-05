//
//  CGLibraryView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#ifndef CGLibraryView_hpp
#define CGLibraryView_hpp

#include <stdio.h>
#include "../../common/CcbBaseView.hpp"

NS_BF_BEGIN

class CGLibraryItemView;
class CGLibraryView : public CcbBaseView {
public:
    typedef struct {
        bool Unlock;
        int ImgArrMaxSize;
        std::string ImgIcon;
        std::vector<std::string> ImgArr;
    } CgInfo;
    typedef std::vector<CgInfo> CgInfoArray;
public:
    CREATE_FUNC(CGLibraryView);
    CGLibraryView();
public:
    static CGLibraryView* nodeFrom(std::string const& ccbName);
public:
    void setSourceData(CgInfoArray const& data);
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
    CGLibraryItemView* mItemView[12];
    std::string mTitleStr;
    CgInfoArray mInfoArray;
};

CCB_VIEW_LOADER(CGLibraryView);

class CGLibraryItemView : public CcbBaseView {
public:
    typedef CGLibraryView::CgInfo ItemData;
    typedef std::function<void(cocos2d::Ref*)> ClickFunc;
public:
    CREATE_FUNC(CGLibraryItemView);
    CGLibraryItemView();
public:
    void setClickCallBack(ClickFunc func);
public:
    int getNumber() const;
    void setNumber(int number);
public:
    void setItemData(ItemData const& data);
    ItemData const& getItemData() const;
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
    cocos2d::Sprite* mCG;
    cocos2d::Label* mInfo;
    cocos2d::LayerColor* mMask;
    ClickFunc mClickCall;
    ItemData mData;
};

CCB_VIEW_LOADER(CGLibraryItemView);

NS_BF_END

#endif /* CGLibraryView_hpp */
