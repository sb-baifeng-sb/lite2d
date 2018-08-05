//
//  CcbView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/20.
//

#ifndef CcbView_hpp
#define CcbView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "editor-support/cocosbuilder/CocosBuilder.h"
#include "cocos2d.h"

NS_BF_BEGIN

class CCB {
public:
    typedef std::shared_ptr<CCB> CCBPtr;
public:
    static CCBPtr New();
public:
    CCB();
    ~CCB();
public:
    void registerClass(std::string const& className, cocosbuilder::NodeLoader* loader);
    void removeClass(std::string const& className);
    cocos2d::Node* nodeFromCCB(std::string const& fileName);
private:
    void buildLoader();
private:
    cocosbuilder::NodeLoaderLibrary* mLoader;
};

class CcbAnimationCompletedDelegate;
class CcbBaseView : public cocos2d::Layer, public cocosbuilder::CCBSelectorResolver, public cocosbuilder::CCBMemberVariableAssigner, public cocosbuilder::NodeLoaderListener {
public:
    typedef std::function<void(std::string const&)> AnimationCompletedCallback;
public:
    CREATE_FUNC(CcbBaseView);
    CcbBaseView();
    ~CcbBaseView();
public:
    void setAnimationCompletedCallBack(std::string const& aniName, AnimationCompletedCallback callback);
    void playAnimation(std::string const& aniName, float fTweenDuration = 0.0f);
    void removeAllAnimationCompletedCallBack();
public:
    void setCCBReader(cocosbuilder::CCBReader* reader);
    cocosbuilder::CCBReader* getCCBReader() const;
public:
    void registerTouchListener(cocos2d::EventListener* listener);
    void removeTouchListener();
protected:
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        //CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(TARGET, SELECTORNAME, METHOD)
        return nullptr;
    }
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        //CCB_SELECTORRESOLVER_CALLFUNC_GLUE(TARGET, SELECTORNAME, METHOD)
        return nullptr;
    };
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(TARGET, SELECTORNAME, METHOD)
        return nullptr;
    }
protected:
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
        //CCB_MEMBERVARIABLEASSIGNER_GLUE(TARGET, MEMBERVARIABLENAME, MEMBERVARIABLETYPE, MEMBERVARIABLE)
        return false;
    }
    virtual bool onAssignCCBCustomProperty(cocos2d::Ref* pTarget, const char* pMemberVariableName, const cocos2d::Value& value) {
        return false;
    }
protected:
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
        // nothing to do
    }
protected:
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
protected:
    cocosbuilder::CCBReader* mReader;
    CcbAnimationCompletedDelegate* mAniCompleted;
};

#ifndef CCB_VIRTUAL_CREATE_NODE
#define CCB_VIRTUAL_CREATE_NODE(T) virtual T * createNode(cocos2d::Node * pParent, cocosbuilder::CCBReader * ccbReader) { \
    auto node = T::create(); \
    auto p = dynamic_cast<CcbBaseView*>(node); \
    if (p != nullptr) { \
        p->setCCBReader(ccbReader); \
    } \
    return node; \
}
#endif

#ifndef CCB_VIEW_LOADER_IMP
#define CCB_VIEW_LOADER_IMP(loaderName, parentClass, nodeName) \
class loaderName : public parentClass { \
public: \
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(loaderName, New); \
protected: \
    CCB_VIRTUAL_CREATE_NODE(nodeName); \
}
#endif

#ifndef CCB_VIEW_LOADER_DEFINE
#define CCB_VIEW_LOADER_DEFINE(loaderName, parentClass, nodeName) CCB_VIEW_LOADER_IMP(loaderName##Loader, parentClass, nodeName)
#endif

#ifndef CCB_VIEW_LOADER_FROM
#define CCB_VIEW_LOADER_FROM(className, parentName) CCB_VIEW_LOADER_IMP(className##Loader, parentName, className)
#endif

#ifndef CCB_VIEW_LOADER
#define CCB_VIEW_LOADER(className) CCB_VIEW_LOADER_FROM(className, cocosbuilder::NodeLoader)
#endif

CCB_VIEW_LOADER(CcbBaseView);

NS_BF_END

#endif /* CcbView_hpp */
