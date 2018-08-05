//
//  MovieLibraryView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#include "MovieLibraryView.hpp"
#include "../../proxy/ResProxy.hpp"
#include "../../app/App.hpp"
#include "../MovieView.hpp"

NS_BF_BEGIN

MovieLibraryView* MovieLibraryView::nodeFrom(std::string const& ccbName) {
    auto ccb = CCB::New();
    ccb->registerClass("MovieLibrary", MovieLibraryViewLoader::New());
    ccb->registerClass("MovieLibraryItem", MovieLibraryItemViewLoader::New());
    return (MovieLibraryView*)ccb->nodeFromCCB(ccbName);
}

MovieLibraryView::MovieLibraryView():mIndex(0), mTitle(nullptr), mBtnPrev(nullptr), mBtnNext(nullptr) {
    memset(mItemView, 0, sizeof(mItemView));
}

void MovieLibraryView::setSourceData(MovieInfoArray const& data) {
    mInfoArray = data;
    this->setPageIndex(0);
}

void  MovieLibraryView::setPageIndex(int index) {
    int const pageMax = (int)ceil(mInfoArray.size() / 12.0f);
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

void  MovieLibraryView::loadPageData(int index) {
    for (int i=0; i < 12; i++) {
        int _index = index*12+i;
        if (_index >= mInfoArray.size()) {
            this->mItemView[i]->setVisible(false);
            continue;
        }
        auto& data = mInfoArray[_index];
        this->mItemView[i]->setNumber(_index);
        this->mItemView[i]->setItemData(data);
        this->mItemView[i]->setVisible(true);
    }
}

cocos2d::SEL_MenuHandler MovieLibraryView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", MovieLibraryView::onBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPrevPage", MovieLibraryView::onPrev);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNextPage", MovieLibraryView::onNext);
    return nullptr;
}

bool MovieLibraryView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfTitle", cocos2d::Label*, mTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnPrev", cocos2d::MenuItemImage*, mBtnPrev);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnNext", cocos2d::MenuItemImage*, mBtnNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem0", MovieLibraryItemView*, mItemView[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem1", MovieLibraryItemView*, mItemView[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem2", MovieLibraryItemView*, mItemView[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem3", MovieLibraryItemView*, mItemView[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem4", MovieLibraryItemView*, mItemView[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem5", MovieLibraryItemView*, mItemView[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem6", MovieLibraryItemView*, mItemView[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem7", MovieLibraryItemView*, mItemView[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem8", MovieLibraryItemView*, mItemView[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem9", MovieLibraryItemView*, mItemView[9]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem10", MovieLibraryItemView*, mItemView[10]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem11", MovieLibraryItemView*, mItemView[11]);
    return false;
}

void MovieLibraryView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    mTitleStr = this->mTitle->getString();
    //this->setPageIndex(0);
    
    for (int i=0; i < 12; i++) {
        this->mItemView[i]->setClickCallBack(std::bind(&MovieLibraryView::onItemClicked, this, std::placeholders::_1));
    }
}

void MovieLibraryView::onBack(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", std::bind(&MovieLibraryView::onOutEnd, this));
    this->playAnimation("outAni");
}

void MovieLibraryView::onPrev(cocos2d::Ref* sender) {
    this->setPageIndex(mIndex-1);
}

void MovieLibraryView::onNext(cocos2d::Ref* sender) {
    this->setPageIndex(mIndex+1);
}

void MovieLibraryView::onItemClicked(cocos2d::Ref* sender) {
    auto view = (MovieLibraryItemView*)sender;
    auto data = view->getItemData();
    MovieView::PlayMovie(data.MoviePath);
}

void MovieLibraryView::onOutEnd() {
    this->removeFromParent();
}

// =================================================================================================================

MovieLibraryItemView::MovieLibraryItemView():mNumber(0), mMovieIcon(nullptr), mPlayIcon(nullptr), mMask(nullptr) {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MovieLibraryItemView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MovieLibraryItemView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MovieLibraryItemView::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MovieLibraryItemView::onTouchCancel, this);
    this->registerTouchListener(listener);
}

void MovieLibraryItemView::setClickCallBack(ClickFunc func) {
    this->mClickCall = func;
}

int MovieLibraryItemView::getNumber() const {
    return this->mNumber;
}

void MovieLibraryItemView::setNumber(int number) {
    this->mNumber = number;
}

void MovieLibraryItemView::setItemData(ItemData const& data) {
    mData = data;
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto texture = res->getTexture(data.ImgIcon);
    this->mMovieIcon->initWithTexture(texture);
    this->mMovieIcon->setContentSize(this->mMask->getContentSize());
    if (!data.Unlock) {
        this->mMovieIcon->setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_GRAYSCALE));
        this->mPlayIcon->setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_GRAYSCALE));
    } else {
        this->mMovieIcon->setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
        this->mPlayIcon->setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    }
}

MovieLibraryItemView::ItemData const& MovieLibraryItemView::getItemData() const {
    return this->mData;
}

bool MovieLibraryItemView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "imgMovieIcon", cocos2d::Sprite*, mMovieIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "imgPlayIcon", cocos2d::Sprite*, mPlayIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "colorMask", cocos2d::LayerColor*, mMask);
    return false;
}

void MovieLibraryItemView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    this->mMask->setVisible(false);
}

bool MovieLibraryItemView::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
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

void MovieLibraryItemView::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
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

void MovieLibraryItemView::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        if (this->mClickCall != nullptr && this->mData.Unlock) {
            this->mClickCall(this);
        }
    }
    this->mMask->setVisible(false);
}

void MovieLibraryItemView::onTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    this->mMask->setVisible(false);
}

NS_BF_END
