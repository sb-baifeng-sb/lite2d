//
//  GameDataMgrView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#include "GameDataMgrView.hpp"
#include "AlertDialogView.hpp"
#include "../../proxy/SaveProxy.hpp"
#include "../../proxy/ConfigProxy.hpp"
#include "../../app/App.hpp"
#include "bftools/simpleapi.hpp"

NS_BF_BEGIN

int GameDataMgrView::mLoadIndex = 0;
int GameDataMgrView::mSaveIndex = 0;

GameDataMgrView* GameDataMgrView::nodeFrom(std::string const& ccbName) {
    auto ccb = CCB::New();
    ccb->registerClass("GameDataManager", GameDataMgrViewLoader::New());
    ccb->registerClass("GameDataItem", GameDataItemViewLoader::New());
    return (GameDataMgrView*)ccb->nodeFromCCB(ccbName);
}

GameDataMgrView::GameDataMgrView():mType(TYPE_NONE), mIndex(0), mTitle(nullptr), mBtnPrev(nullptr), mBtnNext(nullptr) {
    memset(mItemView, 0, sizeof(mItemView));
}

void GameDataMgrView::setCompletedFunc(CompletedFunc func) {
    this->mCompletedFunc = func;
}

void GameDataMgrView::setExitFunc(ExitFunc func) {
    this->mRemoveFunc = func;
}

void GameDataMgrView::setMgrType(GameDataMgrView::MgrType type) {
    this->mType = type;
    if (this->mTitle != nullptr) {
        auto config = App::getProxy<ConfigProxy>(ConfigProxy::Name);
        auto ui = config->getSession("ui");
        if (type == TYPE_LOAD) {
            mTitleStr = ui->getValueAsStr("load-game-data");
        } else if (type == TYPE_SAVE) {
            mTitleStr = ui->getValueAsStr("save-game-data");
        }
    }
}

void GameDataMgrView::setPageIndex(int index) {
    int const pageMax = 10;
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
    } else if (index >= pageMax-1) {
        this->mBtnNext->setEnabled(false);
    }
    char temp[64] = {0};
    sprintf(temp, "(%d/%d)", index+1, pageMax);
    this->mTitle->setString(mTitleStr+temp);
    this->loadPageData(index);
}

void GameDataMgrView::loadPageData(int index) {
    // 加载存档简介
    for (int i=0; i < 8; i++) {
        this->mItemView[i]->setStyle(mType);
        this->mItemView[i]->loadData(index*8+i);
    }
}

void GameDataMgrView::start(MgrType type) {
    this->setMgrType(type);
    this->setPageIndex(mSaveIndex);
}

bool GameDataMgrView::loadGameData(int number) {
    auto save = App::getProxy<SaveProxy>(SaveProxy::Name);
    bool success = true;
    save->LoadGame(number, [&](int number, std::string const& error) {
        if (error.length() != 0) {
            std::string text = error;
            auto view = AlertDialogView::NewErr(text);
            cocos2d::Director::getInstance()->getRunningScene()->addChild(view);
            success = false;
            return;
        }
        this->onBack(nullptr);
    });
    return success;
}

void GameDataMgrView::saveGameData(int number) {
    auto save = App::getProxy<SaveProxy>(SaveProxy::Name);
    save->SaveGame(number, [&](int number, std::string const& error) {
        if (error.length() != 0) {
            std::string text = error;
            auto view = AlertDialogView::NewErr(text);
            cocos2d::Director::getInstance()->getRunningScene()->addChild(view);
            return;
        }
        for (int i=0; i < 8; i++) {
            if (this->mItemView[i]->getNumber() == number) {
                this->mItemView[i]->loadData(number);
            }
        }
    });
}

void GameDataMgrView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    for (int i=0; i < 8; i++) {
        this->mItemView[i]->setClickCallBack(std::bind(&GameDataMgrView::onItemClicked, this, std::placeholders::_1));
    }
}

cocos2d::SEL_MenuHandler GameDataMgrView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", GameDataMgrView::onBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPrevPage", GameDataMgrView::onPrev);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNextPage", GameDataMgrView::onNext);
    return nullptr;
}

bool GameDataMgrView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfTitle", cocos2d::Label*, mTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnPrev", cocos2d::MenuItemImage*, mBtnPrev);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnNext", cocos2d::MenuItemImage*, mBtnNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem0", GameDataItemView*, mItemView[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem1", GameDataItemView*, mItemView[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem2", GameDataItemView*, mItemView[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem3", GameDataItemView*, mItemView[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem4", GameDataItemView*, mItemView[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem5", GameDataItemView*, mItemView[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem6", GameDataItemView*, mItemView[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccbItem7", GameDataItemView*, mItemView[7]);
    return false;
}

void GameDataMgrView::onBack(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", std::bind(&GameDataMgrView::onOutEnd, this));
    this->playAnimation("outAni");
}

void GameDataMgrView::onPrev(cocos2d::Ref* sender) {
    this->setPageIndex(mIndex-1);
    mSaveIndex = mIndex;
}

void GameDataMgrView::onNext(cocos2d::Ref* sender) {
    this->setPageIndex(mIndex+1);
    mSaveIndex = mIndex;
}

void GameDataMgrView::onItemClicked(cocos2d::Ref* sender) {
    auto itemView = (GameDataItemView*)sender;
    this->mCurNumber = itemView->getNumber();
    
    auto config = App::getProxy<ConfigProxy>(ConfigProxy::Name);
    auto ui = config->getSession("ui");
    
    std::string text = mType == TYPE_SAVE ? ui->getValueAsStr("save-game-ask") : ui->getValueAsStr("load-game-ask");
    auto view = AlertDialogView::New(text, [&](bool yes) {
        if (!yes) {
            return;
        }
        bool success = true;
        if (mType == TYPE_SAVE) {
            this->saveGameData(this->mCurNumber);
        } else if (mType == TYPE_LOAD) {
            success = this->loadGameData(this->mCurNumber);
        }
        if (this->mCompletedFunc != nullptr && success) {
            this->mCompletedFunc(mType);
        }
    });
    cocos2d::Director::getInstance()->getRunningScene()->addChild(view);
}

void GameDataMgrView::onOutEnd() {
    this->removeFromParent();
    if (this->mRemoveFunc != nullptr) {
        this->mRemoveFunc();
    }
}

// =================================================================================================================

GameDataItemView::GameDataItemView():mNumber(0), mBG(nullptr), mColorMask(nullptr), mIconLoad(nullptr), mIcon(nullptr), mNO(nullptr), mTitle(nullptr), mPlayTime(nullptr), mSaveTime(nullptr) {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameDataItemView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameDataItemView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameDataItemView::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameDataItemView::onTouchCancel, this);
    this->registerTouchListener(listener);
    
    auto config = App::getProxy<ConfigProxy>(ConfigProxy::Name);
    auto ui = config->getSession("ui");
    mGamePlayTimeText = ui->getValueAsStr("game-play-time");
    mGameSaveTimeText = ui->getValueAsStr("game-save-time");
}

void GameDataItemView::setStyle(GameDataMgrView::MgrType type) {
    if (type == GameDataMgrView::TYPE_SAVE) {
        this->mIconLoad->setVisible(false);
    } else if (type == GameDataMgrView::TYPE_LOAD) {
        this->mIconLoad->setVisible(true);
    }
}

void GameDataItemView::setClickCallBack(ClickFunc func) {
    this->mClickCall = func;
}

void GameDataItemView::loadData(int number) {
    char temp[64] = {0};
    sprintf(temp, "%d", number+1);
    this->mNO->setString(std::string("NO.")+temp);
    this->mNumber = number;
    
    auto save = App::getProxy<SaveProxy>(SaveProxy::Name);
    auto info = save->loadGameSaveInfo(number);
    
    if (info == nullptr) {
        this->mIcon->setVisible(false);
        this->mTitle->setString("NO DATA");
        this->mPlayTime->setString(mGamePlayTimeText+": 00:00:00");
        this->mSaveTime->setString(mGameSaveTimeText+": 0000/00/00 00:00:00");
        return;
    }

    // 删除截图缓存
    auto director = cocos2d::Director::getInstance();
    director->getTextureCache()->removeTextureForKey(info->icon);
    
    // 加载截图
    this->mIcon->initWithFile(info->icon);
    this->mIcon->setContentSize(this->mIconLoad->getContentSize());
    this->mIcon->setVisible(true);
    this->mTitle->setString(info->title);
    
    auto times = stamp_to_standard(info->save_time);
    sprintf(temp, "%s: %04d/%02d/%02d %02d:%02d:%02d", mGameSaveTimeText.c_str(), times.Year, times.Mon, times.Day, times.Hour, times.Min, times.Second);
    this->mSaveTime->setString(temp);
    
    int hour = floor(info->play_time/3600.f);
    int min = (int)floor(info->play_time/60.0f) % 60;
    int sec = info->play_time % 60;
    sprintf(temp, "%s: %02d:%02d:%02d", mGamePlayTimeText.c_str(), hour, min, sec);
    this->mPlayTime->setString(temp);
}

int GameDataItemView::getNumber() const {
    return this->mNumber;
}

bool GameDataItemView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "imgBG", cocos2d::Sprite*, mBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "colorMask", cocos2d::LayerColor*, mColorMask);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "imgIconLoad", cocos2d::Sprite*, mIconLoad);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "imgIcon", cocos2d::Sprite*, mIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfNO", cocos2d::Label*, mNO);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfTitle", cocos2d::Label*, mTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfPlayTime", cocos2d::Label*, mPlayTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttfSaveTime", cocos2d::Label*, mSaveTime);
    return false;
}

void GameDataItemView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    this->mColorMask->setVisible(false);
}

bool GameDataItemView::canClick() const {
    if (!this->mIconLoad->isVisible()) {
        return true;
    }
    return this->mIcon->isVisible();
}

bool GameDataItemView::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        pEvent->stopPropagation();
        if (!this->canClick()) {
            return true;
        }
        this->mColorMask->setVisible(true);
        return true;
    }
    return CcbBaseView::onTouchBegan(pTouch, pEvent);
}

void GameDataItemView::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    if (!this->canClick()) {
        return;
    }
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        this->mColorMask->setVisible(true);
    } else {
        this->mColorMask->setVisible(false);
    }
}

void GameDataItemView::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        if (this->mClickCall != nullptr && this->canClick()) {
            this->mClickCall(this);
        }
    }
    this->mColorMask->setVisible(false);
}

void GameDataItemView::onTouchCancel(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    this->mColorMask->setVisible(false);
}

NS_BF_END
