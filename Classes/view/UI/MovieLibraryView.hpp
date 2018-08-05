//
//  MovieLibraryView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#ifndef MovieLibraryView_hpp
#define MovieLibraryView_hpp

#include <stdio.h>
#include "../../common/CcbBaseView.hpp"

NS_BF_BEGIN

class MovieLibraryItemView;
class MovieLibraryView : public CcbBaseView {
public:
    typedef struct {
        bool Unlock;
        std::string ImgIcon;
        std::string MoviePath;
    } MovieInfo;
    typedef std::vector<MovieInfo> MovieInfoArray;
public:
    CREATE_FUNC(MovieLibraryView);
    MovieLibraryView();
public:
    static MovieLibraryView* nodeFrom(std::string const& ccbName);
public:
    void setSourceData(MovieInfoArray const& data);
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
    MovieLibraryItemView* mItemView[12];
    std::string mTitleStr;
    MovieInfoArray mInfoArray;
};

CCB_VIEW_LOADER(MovieLibraryView);

class MovieLibraryItemView : public CcbBaseView {
public:
    typedef MovieLibraryView::MovieInfo ItemData;
    typedef std::function<void(cocos2d::Ref*)> ClickFunc;
public:
    CREATE_FUNC(MovieLibraryItemView);
    MovieLibraryItemView();
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
    cocos2d::Sprite* mMovieIcon;
    cocos2d::Sprite* mPlayIcon;
    cocos2d::LayerColor* mMask;
    ClickFunc mClickCall;
    ItemData mData;
};

CCB_VIEW_LOADER(MovieLibraryItemView);

NS_BF_END

#endif /* MovieLibraryView_hpp */
