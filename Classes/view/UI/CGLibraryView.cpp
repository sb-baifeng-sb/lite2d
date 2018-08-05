//
//  CGLibraryView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#include "CGLibraryView.hpp"
#include "../../proxy/ResProxy.hpp"
#include "../../app/App.hpp"

NS_BF_BEGIN

class CgLookLayer : public cocos2d::Node {
public:
    CREATE_FUNC(CgLookLayer);
    CgLookLayer();
public:
    void setCgInfo(CGLibraryView::CgInfo const& info);
private:
    void nextBG();
    void onLookFinish();
    void buildBG();
private:
    bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
private:
    int mIndex;
    cocos2d::Sprite* mBG[2];
    CGLibraryView::CgInfo mInfo;
};

CgLookLayer::CgLookLayer():mIndex(0) {
    memset(mBG, 0, sizeof(mBG));
    this->setContentSize(cocos2d::Director::getInstance()->getWinSize());
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(CgLookLayer::onTouchBegan, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool CgLookLayer::onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) {
    pEvent->stopPropagation();
    this->buildBG();
    if (this->mBG[0]->isVisible()) {
        return true;
    }
    this->nextBG();
    return true;
}

void CgLookLayer::setCgInfo(CGLibraryView::CgInfo const& info) {
    this->mIndex = 0;
    this->mInfo = info;
    this->nextBG();
}

void CgLookLayer::buildBG() {
    for (int i=0; i<2; i++) {
        if (mBG[i] == nullptr) {
            mBG[i] = cocos2d::Sprite::create();
            mBG[i]->setPosition(getContentSize().width*0.5f, getContentSize().height*0.5f);
            this->addChild(mBG[i]);
        }
    }
}

void CgLookLayer::nextBG() {
    this->buildBG();
    if (this->mIndex >= this->mInfo.ImgArr.size()) {
        getEventDispatcher()->removeEventListenersForTarget(this);
        if (this->mBG[1]->getTexture() != nullptr) {
            auto action = cocos2d::Sequence::create(cocos2d::FadeOut::create(0.3f), cocos2d::CallFunc::create(std::bind(&CgLookLayer::onLookFinish, this)), NULL);
            this->mBG[1]->runAction(action);
            return;
        } else {
            this->onLookFinish();
            return;
        }
    }
    auto texture = this->mBG[1]->getTexture();
    if (texture != nullptr) {
        this->mBG[0]->initWithTexture(texture);
    }
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    texture = res->getTexture(this->mInfo.ImgArr[this->mIndex]);
    this->mBG[1]->initWithTexture(texture);
    this->mBG[1]->setOpacity(0);
    this->mBG[0]->setVisible(true);
    auto action = cocos2d::Sequence::create(cocos2d::FadeIn::create(0.33f), cocos2d::CallFunc::create([&](){
        this->mBG[0]->setVisible(false);
    }), NULL);
    this->mBG[1]->runAction(action);
    this->mIndex++;
}

void CgLookLayer::onLookFinish() {
    this->removeFromParent();
}


CGLibraryView* CGLibraryView::nodeFrom(std::string const& ccbName) {
    auto ccb = CCB::New();
    ccb->registerClass("CGLibrary", CGLibraryViewLoader::New());
    ccb->registerClass("CGLibraryItem", CGLibraryItemViewLoader::New());
    return (CGLibraryView*)ccb->nodeFromCCB(ccbName);
}

CGLibraryView::CGLibraryView():mIndex(0), mTitle(nullptr), mBtnPrev(nullptr), mBtnNext(nullptr) {
    memset(mItemView, 0, sizeof(mItemView));
}

void CGLibraryView::setSourceData(CgInfoArray const& data) {
    mInfoArray = data;
    this->setPageIndex(0);
}

void  CGLibraryView::setPageIndex(int index) {
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

void  CGLibraryView::loadPageData(int index) {
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

cocos2d::SEL_MenuHandler CGLibraryView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", CGLibraryView::onBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPrevPage", CGLibraryView::onPrev);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNextPage", CGLibraryView::onNext);
    return nullptr;
}

bool CGLibraryView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfTitle", cocos2d::Label*, mTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnPrev", cocos2d::MenuItemImage*, mBtnPrev);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnNext", cocos2d::MenuItemImage*, mBtnNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem0", CGLibraryItemView*, mItemView[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem1", CGLibraryItemView*, mItemView[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem2", CGLibraryItemView*, mItemView[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem3", CGLibraryItemView*, mItemView[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem4", CGLibraryItemView*, mItemView[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem5", CGLibraryItemView*, mItemView[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem6", CGLibraryItemView*, mItemView[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem7", CGLibraryItemView*, mItemView[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem8", CGLibraryItemView*, mItemView[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem9", CGLibraryItemView*, mItemView[9]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem10", CGLibraryItemView*, mItemView[10]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem11", CGLibraryItemView*, mItemView[11]);
    return false;
}

void CGLibraryView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    mTitleStr = this->mTitle->getString();
    for (int i=0; i < 12; i++) {
        this->mItemView[i]->setClickCallBack(std::bind(&CGLibraryView::onItemClicked, this, std::placeholders::_1));
    }
}

void CGLibraryView::onBack(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", std::bind(&CGLibraryView::onOutEnd, this));
    this->playAnimation("outAni");
}

void CGLibraryView::onPrev(cocos2d::Ref* sender) {
    this->setPageIndex(mIndex-1);
}

void CGLibraryView::onNext(cocos2d::Ref* sender) {
    this->setPageIndex(mIndex+1);
}

void CGLibraryView::onItemClicked(cocos2d::Ref* sender) {
    auto view = (CGLibraryItemView*)sender;
    auto data = view->getItemData();
    auto layer = CgLookLayer::create();
    this->addChild(layer);
    layer->setCgInfo(data);
}

void CGLibraryView::onOutEnd() {
    this->removeFromParent();
}

// =================================================================================================================

CGLibraryItemView::CGLibraryItemView():mNumber(0), mCG(nullptr), mMask(nullptr), mInfo(nullptr) {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(CGLibraryItemView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CGLibraryItemView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CGLibraryItemView::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(CGLibraryItemView::onTouchCancel, this);
    this->registerTouchListener(listener);
}

void CGLibraryItemView::setClickCallBack(ClickFunc func) {
    this->mClickCall = func;
}

int CGLibraryItemView::getNumber() const {
    return this->mNumber;
}

void CGLibraryItemView::setNumber(int number) {
    this->mNumber = number;
}

void CGLibraryItemView::setItemData(CGLibraryItemView::ItemData const& data) {
    mData = data;
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto texture = res->getTexture(data.ImgIcon);
    this->mCG->initWithTexture(texture);
    this->mCG->setContentSize(this->mMask->getContentSize());
    if (!data.Unlock) {
        this->mCG->setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_GRAYSCALE));
    } else {
        this->mCG->setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    }
    char temp[64] = {0};
    sprintf(temp, "%d/%d", (int)data.ImgArr.size(), (int)data.ImgArrMaxSize);
    this->mInfo->setString(temp);
}

CGLibraryItemView::ItemData const& CGLibraryItemView::getItemData() const {
    return this->mData;
}

bool CGLibraryItemView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "imgCG", cocos2d::Sprite*, mCG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "colorMask", cocos2d::LayerColor*, mMask);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfInfo", cocos2d::Label*, mInfo);
    return false;
}

void CGLibraryItemView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    this->mMask->setVisible(false);
}

bool CGLibraryItemView::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
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

void CGLibraryItemView::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
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

void CGLibraryItemView::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        if (this->mClickCall != nullptr && this->mData.Unlock) {
            this->mClickCall(this);
        }
    }
    this->mMask->setVisible(false);
}

void CGLibraryItemView::onTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    this->mMask->setVisible(false);
}

NS_BF_END
