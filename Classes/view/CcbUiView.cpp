//
//  CcbUiView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/21.
//

#include "CcbUiView.hpp"

NS_BF_BEGIN

class SelectorInfo {
public:
    SelectorInfo() {}
    virtual ~SelectorInfo() {}
public:
    SelectorInfo(std::string const& name) {
        setSelectorName(name);
    }
public:
    void setSelectorName(std::string const& name) {
        mSelectorName = name;
    }
    std::string const& getSelectorName() const {
        return mSelectorName;
    }
private:
    std::string mSelectorName;
};

class BFMenuItem : public cocos2d::MenuItemImage, public SelectorInfo {
public:
    CREATE_FUNC(BFMenuItem);
};

class BFControl : public cocos2d::extension::ControlButton, public SelectorInfo {
public:
    CREATE_FUNC(BFControl);
};

class ControlButtonLoader : public cocosbuilder::ControlButtonLoader {
public:
    virtual cocosbuilder::BlockControlData * parsePropTypeBlockControl(cocos2d::Node * pNode, cocos2d::Node * pParent, cocosbuilder::CCBReader * ccbReader);
};

class MenuButtonLoader : public cocosbuilder::MenuItemImageLoader {
public:
    virtual cocosbuilder::BlockData * parsePropTypeBlock(cocos2d::Node * pNode, cocos2d::Node * pParent, cocosbuilder::CCBReader * ccbReader);
};

cocosbuilder::BlockData * MenuButtonLoader::parsePropTypeBlock(cocos2d::Node * pNode, cocos2d::Node * /*pParent*/, cocosbuilder::CCBReader * ccbReader) {
    std::string selectorName = ccbReader->readCachedString();
    cocosbuilder::CCBReader::TargetType selectorTarget = static_cast<cocosbuilder::CCBReader::TargetType>(ccbReader->readInt(false));
    
    if(selectorTarget != cocosbuilder::CCBReader::TargetType::NONE) {
        Ref*  target = nullptr;
        if(!ccbReader->isJSControlled()) {
            if(selectorTarget == cocosbuilder::CCBReader::TargetType::DOCUMENT_ROOT) {
                target = ccbReader->getAnimationManager()->getRootNode();
            } else if(selectorTarget == cocosbuilder::CCBReader::TargetType::OWNER) {
                target = ccbReader->getOwner();
            }
            
            if(target != nullptr) {
                if(!selectorName.empty()) {
                    cocos2d::SEL_MenuHandler selMenuHandler = 0;
                    
                    cocosbuilder::CCBSelectorResolver * targetAsCCBSelectorResolver = dynamic_cast<cocosbuilder::CCBSelectorResolver *>(target);
                    
                    if(targetAsCCBSelectorResolver != nullptr) {
                        selMenuHandler = targetAsCCBSelectorResolver->onResolveCCBCCMenuItemSelector(target, selectorName.c_str());
                    }
                    
                    if(selMenuHandler == 0) {
                        cocosbuilder::CCBSelectorResolver * ccbSelectorResolver = ccbReader->getCCBSelectorResolver();
                        if(ccbSelectorResolver != nullptr) {
                            selMenuHandler = ccbSelectorResolver->onResolveCCBCCMenuItemSelector(target, selectorName.c_str());
                        }
                    }
                    
                    if(selMenuHandler == 0) {
                        CCLOG("Skipping selector '%s' since no CCBSelectorResolver is present.", selectorName.c_str());
                    } else {
                        cocosbuilder::BlockData * blockData = new (std::nothrow) cocosbuilder::BlockData();
                        blockData->mSELMenuHandler = selMenuHandler;
                        blockData->_target = target;
                        
                        auto button = dynamic_cast<BFMenuItem*>(pNode);
                        if (button != nullptr) {
                            button->setSelectorName(selectorName);
                        }
                        
                        return blockData;
                    }
                } else {
                    CCLOG("Unexpected empty selector.");
                }
            } else {
                CCLOG("Unexpected nullptr target for selector.");
            }
        } else {
            if (selectorTarget == cocosbuilder::CCBReader::TargetType::DOCUMENT_ROOT) {
                ccbReader->addDocumentCallbackNode(pNode);
                ccbReader->addDocumentCallbackName(selectorName);
                // Since there isn't a Control::EventType::NONE, add a TOUCH_DOWN type as a placeholder.
                ccbReader->addDocumentCallbackControlEvents(cocos2d::extension::Control::EventType::TOUCH_DOWN);
            } else if (selectorTarget == cocosbuilder::CCBReader::TargetType::OWNER) {
                ccbReader->addOwnerCallbackNode(pNode);
                ccbReader->addOwnerCallbackName(selectorName);
                // Since there isn't a Control::EventType::NONE, add a TOUCH_DOWN type as a placeholder.
                ccbReader->addOwnerCallbackControlEvents(cocos2d::extension::Control::EventType::TOUCH_DOWN);
            }
        }
    }
    
    return nullptr;
}

cocosbuilder::BlockControlData* ControlButtonLoader::parsePropTypeBlockControl(cocos2d::Node * pNode, cocos2d::Node * /*pParent*/, cocosbuilder::CCBReader * ccbReader) {
    std::string selectorName = ccbReader->readCachedString();
    cocosbuilder::CCBReader::TargetType selectorTarget = static_cast<cocosbuilder::CCBReader::TargetType>(ccbReader->readInt(false));
    int controlEvents = ccbReader->readInt(false);
    
    if(selectorTarget != cocosbuilder::CCBReader::TargetType::NONE) {
        if(!ccbReader->isJSControlled()) {
            Ref*  target = nullptr;
            if(selectorTarget == cocosbuilder::CCBReader::TargetType::DOCUMENT_ROOT) {
                target = ccbReader->getAnimationManager()->getRootNode();
            } else if(selectorTarget == cocosbuilder::CCBReader::TargetType::OWNER) {
                target = ccbReader->getOwner();
            }
            
            if(target != nullptr) {
                if(!selectorName.empty()) {
                    cocos2d::extension::Control::Handler selControlHandler = 0;
                    
                    cocosbuilder::CCBSelectorResolver * targetAsCCBSelectorResolver = dynamic_cast<cocosbuilder::CCBSelectorResolver *>(target);
                    
                    if(targetAsCCBSelectorResolver != nullptr) {
                        selControlHandler = targetAsCCBSelectorResolver->onResolveCCBCCControlSelector(target, selectorName.c_str());
                    }
                    
                    if(selControlHandler == 0) {
                        cocosbuilder::CCBSelectorResolver * ccbSelectorResolver = ccbReader->getCCBSelectorResolver();
                        if(ccbSelectorResolver != nullptr) {
                            selControlHandler = ccbSelectorResolver->onResolveCCBCCControlSelector(target, selectorName.c_str());
                        }
                    }
                    
                    if(selControlHandler == 0) {
                        CCLOG("Skipping selector '%s' since no CCBSelectorResolver is present.", selectorName.c_str());
                    } else {
                        cocosbuilder::BlockControlData * blockControlData = new (std::nothrow) cocosbuilder::BlockControlData();
                        blockControlData->mSELControlHandler = selControlHandler;
                        
                        blockControlData->_target = target;
                        blockControlData->mControlEvents = (cocos2d::extension::Control::EventType)controlEvents;
                        
                        auto button = dynamic_cast<BFControl*>(pNode);
                        if (button != nullptr) {
                            button->setSelectorName(selectorName);
                        }
                        
                        return blockControlData;
                    }
                } else {
                    CCLOG("Unexpected empty selector.");
                }
            } else {
                CCLOG("Unexpected nullptr target for selector.");
            }
        } else {
            if(selectorTarget == cocosbuilder::CCBReader::TargetType::DOCUMENT_ROOT) {
                ccbReader->addDocumentCallbackNode(pNode);
                ccbReader->addDocumentCallbackName(selectorName);
                ccbReader->addDocumentCallbackControlEvents((cocos2d::extension::Control::EventType)controlEvents);
            } else {
                ccbReader->addOwnerCallbackNode(pNode);
                ccbReader->addOwnerCallbackName(selectorName);
                ccbReader->addOwnerCallbackControlEvents((cocos2d::extension::Control::EventType)controlEvents);
            }
        }
    }
    
    return nullptr;
}

CCB_VIEW_LOADER_DEFINE(BFMenuItem, MenuButtonLoader, BFMenuItem);
CCB_VIEW_LOADER_DEFINE(BFControl, ControlButtonLoader, BFControl);

CcbUiViewState::CcbUiViewState() {
    
}

void CcbUiViewState::Set(AvgSprite* sprite) {
    auto view = sprite->type<CcbUiView>();
    if (view != nullptr) {
        view->mCcbName = this->mCcbName;
        view->mClassName = this->mClassName;
        view->mCodes = this->mCodes;
        view->mAniCodes = this->mAniCodes;
    }
    SimpleSpriteState::Set(sprite);
}

void CcbUiViewState::Get(AvgSprite* sprite) {
    SimpleSpriteState::Get(sprite);
    auto view = sprite->type<CcbUiView>();
    if (view != nullptr) {
        this->mCcbName = view->mCcbName;
        this->mClassName = view->mClassName;
        this->mCodes = view->mCodes;
        this->mAniCodes = view->mAniCodes;
    }
}

std::string const& CcbUiViewState::getCcbName() const {
    return this->mCcbName;
}

std::string const& CcbUiViewState::getClassName() const {
    return this->mClassName;
}

CcbUiView* CcbUiView::New(std::string const& fileName, std::string const& className) {
    auto ccb = CCB::New();
    ccb->removeClass("CCMenuItemImage");
    ccb->removeClass("CCControlButton");
    ccb->registerClass(className, CcbUiViewLoader::New());
    ccb->registerClass("CCMenuItemImage", BFMenuItemLoader::New());
    ccb->registerClass("CCControlButton", BFControlLoader::New());
    auto view = (CcbUiView*)ccb->nodeFromCCB(fileName);
    if (view != nullptr) {
        view->mCcbName = fileName;
        view->mClassName = className;
    }
    return view;
}

CcbUiView::SpriteStatePtr CcbUiView::getSpriteState() {
    SpriteStatePtr state(new CcbUiViewState);
    state->Get(this);
    return state;
}

CcbUiView::CcbUiView():BF::AvgSprite(TYPE_CCB_UI, this) {
    mSelectorVector.reserve(10);
}

cocos2d::SEL_MenuHandler CcbUiView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    mSelectorVector.push_back(pSelectorName);
    return CC_MENU_SELECTOR(CcbUiView::onMenuClick);
}

cocos2d::extension::Control::Handler CcbUiView::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
    mSelectorVector.push_back(pSelectorName);
    return cccontrol_selector(CcbUiView::onControlClick);
}

void CcbUiView::setCodeExcute(CodeExcute exc) {
    this->mExcute = exc;
}

void CcbUiView::setSelectorAndCode(std::string const& selectorName, std::string const& code) {
    this->mCodes[selectorName] = code;
}

void CcbUiView::setAnimationEndCallbackCode(std::string const& aniName, std::string const& code) {
    mAniCodes[aniName] = code;
    this->setAnimationCompletedCallBack(aniName, [&](std::string const& aniName) {
        auto iter = mAniCodes.find(aniName);
        if (iter != mAniCodes.end() && iter->second.length() != 0 && mExcute != nullptr) {
            mExcute(iter->second);
        }
    });
}

CcbUiView::SelectorVector const& CcbUiView::getSelectorVector() const {
    return this->mSelectorVector;
}

void CcbUiView::onMenuClick(cocos2d::Ref* sender) {
    auto button = dynamic_cast<BFMenuItem*>(sender);
    if (button == nullptr) {
        return;
    }
    auto name = button->getSelectorName();
    if (name.length() != 0) {
        CCLOG("onMenuItem clicked: %s", name.c_str());
        auto iter = mCodes.find(name);
        if (iter != mCodes.end() && iter->second.length() != 0 && mExcute != nullptr) {
            mExcute(iter->second);
        }
    }
}

void CcbUiView::onControlClick(cocos2d::Ref* sender, cocos2d::extension::Control::EventType type) {
    if (cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE != type) {
        return;
    }
    auto button = dynamic_cast<BFControl*>(sender);
    if (button == nullptr) {
        return;
    }
    auto name = button->getSelectorName();
    if (name.length() != 0) {
        CCLOG("onControlButton clicked: %s", name.c_str());
        auto iter = mCodes.find(name);
        if (iter != mCodes.end() && iter->second.length() != 0 && mExcute != nullptr) {
            mExcute(iter->second);
        }
    }
}

NS_BF_END
