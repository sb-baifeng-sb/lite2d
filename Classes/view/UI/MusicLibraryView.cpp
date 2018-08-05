//
//  MusicLibraryView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#include "MusicLibraryView.hpp"
#include "../../proxy/MediaProxy.hpp"
#include "../../app/App.hpp"
#include "math.h"

NS_BF_BEGIN

MusicLibraryView* MusicLibraryView::nodeFrom(std::string const& ccbName) {
    auto ccb = CCB::New();
    ccb->registerClass("MusicLibrary", MusicLibraryViewLoader::New());
    ccb->registerClass("MusicLibraryItem", MusicLibraryItemViewLoader::New());
    return (MusicLibraryView*)ccb->nodeFromCCB(ccbName);
}

MusicLibraryView::MusicLibraryView():mIndex(0), mTitle(nullptr), mBtnPrev(nullptr), mBtnNext(nullptr) {
    memset(mItemView, 0, sizeof(mItemView));
}

void MusicLibraryView::setSourceData(MusicInfoArray const& data) {
    mInfoArray = data;
    this->setPageIndex(0);
}

void  MusicLibraryView::setPageIndex(int index) {
    int const pageMax = (int)ceil(mInfoArray.size() / 10.0f);
    if (index < 0) {
        return;
    } else if (index >= pageMax) {
        return;
    }
    this->mIndex = index;
    this->mBtnPrev->setEnabled(true);
    this->mBtnNext->setEnabled(true);
    if (index == 0) {
        this->mBtnPrev->setEnabled(false);
    }
    if (index >= pageMax-1) {
        this->mBtnNext->setEnabled(false);
    }
    char temp[64] = {0};
    sprintf(temp, "(%d/%d)", index+1, pageMax);
    this->mTitle->setString(mTitleStr+temp);
    this->loadPageData(index);
}

void  MusicLibraryView::loadPageData(int index) {
    for (int i=0; i < 10; i++) {
        int _index = index*10+i;
        if (_index >= mInfoArray.size()) {
            this->mItemView[i]->setVisible(false);
            continue;
        }
        MusicLibraryItemView::MusicInfo const& info = mInfoArray[_index];
        this->mItemView[i]->setNumber(_index);
        this->mItemView[i]->setItemData(info);
        this->mItemView[i]->setVisible(true);
    }
}

cocos2d::SEL_MenuHandler MusicLibraryView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", MusicLibraryView::onBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPrevPage", MusicLibraryView::onPrev);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNextPage", MusicLibraryView::onNext);
    return nullptr;
}

bool MusicLibraryView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfTitle", cocos2d::Label*, mTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnPrev", cocos2d::MenuItemImage*, mBtnPrev);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnNext", cocos2d::MenuItemImage*, mBtnNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem0", MusicLibraryItemView*, mItemView[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem1", MusicLibraryItemView*, mItemView[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem2", MusicLibraryItemView*, mItemView[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem3", MusicLibraryItemView*, mItemView[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem4", MusicLibraryItemView*, mItemView[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem5", MusicLibraryItemView*, mItemView[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem6", MusicLibraryItemView*, mItemView[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem7", MusicLibraryItemView*, mItemView[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem8", MusicLibraryItemView*, mItemView[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem9", MusicLibraryItemView*, mItemView[9]);
    return false;
}

void MusicLibraryView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    mTitleStr = this->mTitle->getString();
    //this->setPageIndex(0);
    for (int i=0; i<10; i++) {
        mItemView[i]->setClickCallBack(std::bind(&MusicLibraryView::onItemClicked, this, std::placeholders::_1));
    }
}

void MusicLibraryView::onBack(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", std::bind(&MusicLibraryView::onOutEnd, this));
    this->playAnimation("outAni");
}

void MusicLibraryView::onPrev(cocos2d::Ref* sender) {
    this->setPageIndex(mIndex-1);
}

void MusicLibraryView::onNext(cocos2d::Ref* sender) {
    this->setPageIndex(mIndex+1);
}

void MusicLibraryView::onItemClicked(cocos2d::Ref* sender) {
    auto view = (MusicLibraryItemView*)sender;
    auto info = view->getItemData();
    auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
    media->playMusic(info.Path);
}

void MusicLibraryView::onOutEnd() {
    this->removeFromParent();
}

// =================================================================================================================

MusicLibraryItemView::MusicLibraryItemView():mNumber(0), mMask(nullptr), mTitle(nullptr) {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MusicLibraryItemView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MusicLibraryItemView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MusicLibraryItemView::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MusicLibraryItemView::onTouchCancel, this);
    this->registerTouchListener(listener);
}

void MusicLibraryItemView::setClickCallBack(ClickFunc func) {
    this->mClickCall = func;
}

int MusicLibraryItemView::getNumber() const {
    return this->mNumber;
}

void MusicLibraryItemView::setNumber(int number) {
    this->mNumber = number;
}

void MusicLibraryItemView::setItemData(MusicLibraryItemView::MusicInfo const& data) {
    this->mData = data;
    
    char temp[64] = {0};
    if (!data.Unlock) {
        sprintf(temp, "NO.%d ????????", mNumber+1);
    } else {
        sprintf(temp, "NO.%d %s", mNumber+1, data.Name.c_str());
    }
    this->mTitle->setString(temp);
}

MusicLibraryItemView::MusicInfo const& MusicLibraryItemView::getItemData() const {
    return this->mData;
}

bool MusicLibraryItemView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "colorMask", cocos2d::LayerColor*, mMask);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfTitle", cocos2d::Label*, mTitle);
    return false;
}

void MusicLibraryItemView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    this->mMask->setVisible(false);
}

bool MusicLibraryItemView::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        pEvent->stopPropagation();
        if (!this->mData.Unlock) {
            return true;
        }
        this->mMask->setVisible(true);
        return true;
    }
    return CcbBaseView::onTouchBegan(pTouch, pEvent);
}

void MusicLibraryItemView::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    if (!this->mData.Unlock) {
        return;
    }
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        this->mMask->setVisible(true);
    } else {
        this->mMask->setVisible(false);
    }
}

void MusicLibraryItemView::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        if (this->mClickCall != nullptr  && mData.Unlock) {
            this->mClickCall(this);
        }
    }
    this->mMask->setVisible(false);
}

void MusicLibraryItemView::onTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    this->mMask->setVisible(false);
}


NS_BF_END
