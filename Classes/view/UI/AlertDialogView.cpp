//
//  AlertDialogView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/27.
//

#include "AlertDialogView.hpp"

NS_BF_BEGIN

AlertDialogView* AlertDialogView::New(std::string const& title, ResultFunc func) {
    auto ccb = CCB::New();
    ccb->registerClass("AlertDialogView", AlertDialogViewLoader::New());
    auto view = (AlertDialogView*)ccb->nodeFromCCB("system/alert_dialog");
    view->setTitle(title);
    view->setResultFunc(func);
    return view;
}

AlertDialogView* AlertDialogView::NewErr(std::string const& title) {
    auto ccb = CCB::New();
    ccb->registerClass("AlertDialogView", AlertDialogViewLoader::New());
    auto view = (AlertDialogView*)ccb->nodeFromCCB("system/alert_dialog_error");
    view->setTitle(title);
    return view;
}

AlertDialogView::AlertDialogView():mNode(nullptr), mTitle(nullptr) {
    
}

void AlertDialogView::setTitle(std::string const& title) {
    this->mTitle->setString(title);
}

void AlertDialogView::setResultFunc(ResultFunc func) {
    this->mCallBack = func;
}

void AlertDialogView::onSure(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", [&](std::string const& aniName) {
        if (this->mCallBack != nullptr) {
            this->mCallBack(true);
        }
        this->removeFromParent();
    });
    this->playAnimation("outAni");
}

void AlertDialogView::onCancel(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", [&](std::string const& aniName) {
        if (this->mCallBack != nullptr) {
            this->mCallBack(false);
        }
        this->removeFromParent();
    });
    this->playAnimation("outAni");
}

bool AlertDialogView::onNodeBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    return true;
}

void AlertDialogView::onNodeEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    this->onCancel(nullptr);
}

cocos2d::SEL_MenuHandler AlertDialogView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSure", AlertDialogView::onSure);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCancel", AlertDialogView::onCancel);
    return nullptr;
}

bool AlertDialogView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeMask", cocos2d::Node*, mNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfTitle", cocos2d::Label*, mTitle);
    return false;
}

void AlertDialogView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(AlertDialogView::onNodeBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(AlertDialogView::onNodeEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->mNode);
}

NS_BF_END
