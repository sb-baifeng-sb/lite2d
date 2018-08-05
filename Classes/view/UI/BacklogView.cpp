//
//  BacklogView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#include "BacklogView.hpp"
#include "../../common/BFTableView.hpp"
#include "../../proxy/BacklogProxy.hpp"
#include "../../proxy/MediaProxy.hpp"
#include "../../app/App.hpp"

NS_BF_BEGIN

CCB_VIEW_LOADER(BFTableView);

BacklogView* BacklogView::nodeFrom(std::string const& ccbName) {
    auto ccb = CCB::New();
    ccb->registerClass("GameBackLog", BacklogViewLoader::New());
    ccb->registerClass("TableView", BFTableViewLoader::New());
    return (BacklogView*)ccb->nodeFromCCB(ccbName);
}

BacklogView::BacklogView():mBacklog(nullptr), mListView(nullptr), mLabel(nullptr) {
    mLabel = cocos2d::Label::createWithBMFont("font/sets.fnt", "");
    CC_SAFE_RETAIN(mLabel);
}

BacklogView::~BacklogView() {
    CC_SAFE_RELEASE(mLabel);
}

void BacklogView::setExitFunc(ExitCallBack func) {
    this->mExitCall = func;
}

void BacklogView::onClose(cocos2d::Ref* sender) {
    this->setAnimationCompletedCallBack("outAni", [&](std::string const& aniName) {
        this->removeFromParent();
    });
    this->playAnimation("outAni");
    
    if (mExitCall != nullptr) {
        mExitCall();
    }
}

cocos2d::SEL_MenuHandler BacklogView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onClose", BacklogView::onClose);
    return nullptr;
}

bool BacklogView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeListBox", BFTableView*, mListView);
    return false;
}

void BacklogView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    this->mBacklog = App::getProxy<BacklogProxy>(BacklogProxy::Name);
    
    /*auto backlog = this->mBacklog;
    backlog->clear();
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "sound_bi.wav");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受打击了看封禁扫来得及覅偶吴居蓝受", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶", "");
    backlog->insertLog("AABB", "封禁扫来得及覅偶吴居蓝受打击了看", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");
    backlog->insertLog("BBCC", "腹肌大佬看撒娇覅偶为案", "");*/
    
    this->initListView();
}

bool BacklogView::isPrevDataSameName(int index) const {
    auto data = this->mBacklog->getLog(index);
    if (index - 1 < 0) {
        return false;
    }
    auto prev = this->mBacklog->getLog(index-1);
    if (prev.name == data.name) {
        return true;
    }
    return false;
}

void BacklogView::initListView() {
    if (!this->mListView) {
        return;
    }
    mLabel->setWidth( mListView->getContentSize().width-50 );
    
    mHeights.clear();
    for (int i=0; i < this->mBacklog->getLogsSize(); i++) {
        mHeights.push_back(0);
    }
    
    mListView->TableCellSizeForIndex = [&](BFTableView::TableView* table, ssize_t idx) {
        int index = this->mBacklog->getLogsSize() - (int)idx;
        auto data = this->mBacklog->getLog(index-1);
        
        int height = data.height;
        if (data.height == 0) {
            mLabel->setString(data.text);
            mLabel->updateContent();
            height =  mLabel->getContentSize().height - 32;
            this->mBacklog->updateLogHeight(index-1, height);
        }
        
        if (isPrevDataSameName(index-1)) {
            mHeights[index-1] = 60 + height;
            return cocos2d::Size(0, 60+height);
        }
        
        mHeights[index-1] = 120 + height;
        return cocos2d::Size(0, 120+height);
    };
    mListView->TableCellAtIndex = [&](BFTableView::TableView* table, ssize_t idx) {
        
        cocos2d::Color4B _color[6] = {
            cocos2d::Color4B(255, 0, 0, 100),
            cocos2d::Color4B(0, 255, 0, 100),
            cocos2d::Color4B(0, 0, 255, 100),
            cocos2d::Color4B(255, 255, 0, 100),
            cocos2d::Color4B(255, 0, 255, 100),
            cocos2d::Color4B(0, 255, 255, 100),
        };
        
        int index = this->mBacklog->getLogsSize() - (int)idx;
        auto data = this->mBacklog->getLog(index-1);

        auto cell = table->dequeueCell();
        if (cell == nullptr) {
            cell = BFTableView::TableViewCell::create();
            
            auto view = BacklogItemView::nodeFrom("system/back_log_item");
            view->setTag(1101);
            cell->addChild(view);

            if (!isPrevDataSameName(index-1)) {
                view->setName(data.name);
                if (mHeights[index-1] > 120) {
                    view->setPositionY(mHeights[index-1]-120);
                } else {
                    view->setPositionY(0);
                }
            } else {
                view->setPositionY(mHeights[index-1]-60);
            }
            
            view->setText(data.text);
            view->setSound(data.sound);
            view->updateContent( mLabel->getWidth() );
            
        } else {
            
            auto view = (BacklogItemView*)cell->getChildByTag(1101);
            if (view == nullptr) {
                return cell;
            }
            view->setName("");
            if (!isPrevDataSameName(index-1)) {
                view->setName(data.name);
                if (mHeights[index-1] > 120) {
                    view->setPositionY(mHeights[index-1]-120);
                } else {
                    view->setPositionY(0);
                }
            } else {
                view->setPositionY(mHeights[index-1]-60);
            }
            view->setText(data.text);
            view->setSound(data.sound);
            view->updateContent( mLabel->getWidth() );
        }
        return cell;
    };
    mListView->NumberOfCellsInTableView = [&](BFTableView::TableView* table) {
        return this->mBacklog->getLogsSize();
    };
    mListView->TableCellTouched = [&](BFTableView::TableView* table, BFTableView::TableViewCell* cell) {

        auto view = (BacklogItemView*)cell->getChildByTag(1101);
        if (view == nullptr) {
            return;
        }
        if (view->playSound()) {
            view->getMaskLayer()->setVisible(true);
        }
    };
    
    mListView->setViewSize(mListView->getContentSize());
    mListView->reloadData();
    
    //mListView->setContentOffset(cocos2d::Vec2(0, mListView->maxContainerOffset().y));
}

// ===================================================================================================

BacklogItemView* BacklogItemView::nodeFrom(std::string const& ccbName) {
    auto ccb = CCB::New();
    ccb->registerClass("BackLogItem", BacklogItemViewLoader::New());
    return (BacklogItemView*)ccb->nodeFromCCB(ccbName);
}

BacklogItemView::BacklogItemView():mName(nullptr), mText(nullptr), mMask(nullptr), mBox(nullptr) {
    
}

void BacklogItemView::setName(std::string const& name) {
    this->mName->setString(name);
}

void BacklogItemView::setText(std::string const& text) {
    this->mText->setString(text);
}

void BacklogItemView::setSound(std::string const& sound) {
    this->mSound = sound;
}

bool BacklogItemView::playSound() const {
    if (this->mSound.length() != 0) {
        auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
        media->playEffect(this->mSound);
        return true;
    }
    return false;
}

void BacklogItemView::updateContent(float width) {
    this->mName->setWidth(width);
    this->mName->updateContent();
    this->mText->setWidth(width);
    this->mText->updateContent();
    
    if (this->mName->getString().length() == 0) {
        // 和上一个数据项同名
        this->mName->setVisible(false);
    } else {
        this->mName->setVisible(true);
    }
    this->mMask->setVisible(false);
}

void BacklogItemView::setNewHeight(float height) {
    if (height == 0) {
        return;
    }
    auto& size = this->getContentSize();
    this->setContentSize(cocos2d::Size(size.width, height));
    if (height <= 120) {
        this->setPositionY(0);
    } else {
        this->setPositionY(height-120);
    }
}

cocos2d::LayerColor* BacklogItemView::getMaskLayer() const {
    return this->mMask;
}

bool BacklogItemView::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "labelName", cocos2d::Label*, mName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "labelText", cocos2d::Label*, mText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "colorMask", cocos2d::LayerColor*, mMask);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeBox", cocos2d::Node*, mBox);
    return false;
}

void BacklogItemView::onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
    this->mNamePos = this->mName->getPosition();
    this->mTextPos = this->mText->getPosition();
    this->mMask->setVisible(false);
    this->removeTouchListener();
}

NS_BF_END
