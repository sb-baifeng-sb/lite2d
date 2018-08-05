//
//  AlertDialogView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/27.
//

#ifndef AlertDialogView_hpp
#define AlertDialogView_hpp

#include <stdio.h>
#include "../../common/CcbBaseView.hpp"

NS_BF_BEGIN

class AlertDialogView : public CcbBaseView {
public:
    typedef std::function<void(bool)> ResultFunc;
public:
    static AlertDialogView* New(std::string const& title, ResultFunc func);
    static AlertDialogView* NewErr(std::string const& title);
public:
    CREATE_FUNC(AlertDialogView);
    AlertDialogView();
public:
    void setTitle(std::string const& title);
    void setResultFunc(ResultFunc func);
private:
    void onSure(cocos2d::Ref* sender);
    void onCancel(cocos2d::Ref* sender);
    bool onNodeBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onNodeEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
private:
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    cocos2d::Node* mNode;
    cocos2d::Label* mTitle;
    ResultFunc mCallBack;
};

CCB_VIEW_LOADER(AlertDialogView);

NS_BF_END

#endif /* AlertDialogView_hpp */
