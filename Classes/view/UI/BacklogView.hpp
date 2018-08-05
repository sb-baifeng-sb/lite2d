//
//  BacklogView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/26.
//

#ifndef BacklogView_hpp
#define BacklogView_hpp

#include <stdio.h>
#include "../../common/CcbBaseView.hpp"

NS_BF_BEGIN

class BacklogProxy;
class BFTableView;
class BacklogView : public CcbBaseView {
public:
    typedef std::function<void()> ExitCallBack;
public:
    CREATE_FUNC(BacklogView);
    BacklogView();
    ~BacklogView();
public:
    static BacklogView* nodeFrom(std::string const& ccbName);
public:
    void setExitFunc(ExitCallBack func);
private:
    void initListView();
    bool isPrevDataSameName(int index) const;
private:
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    void onClose(cocos2d::Ref* sender);
private:
    BacklogProxy* mBacklog;
    BFTableView* mListView;
    cocos2d::Label* mLabel;
    ExitCallBack mExitCall;
    std::vector<int> mHeights;
};

CCB_VIEW_LOADER(BacklogView);

class BacklogItemView : public CcbBaseView {
public:
    CREATE_FUNC(BacklogItemView);
    BacklogItemView();
public:
    static BacklogItemView* nodeFrom(std::string const& ccbName);
public:
    void setName(std::string const& name);
    void setText(std::string const& text);
    void setSound(std::string const& sound);
    bool playSound() const;
public:
    void updateContent(float width);
    void setNewHeight(float height);
public:
    cocos2d::LayerColor* getMaskLayer() const;
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
private:
    std::string mSound;
    cocos2d::Label* mName;
    cocos2d::Label* mText;
    cocos2d::LayerColor* mMask;
    cocos2d::Node* mBox;
    cocos2d::Vec2 mNamePos;
    cocos2d::Vec2 mTextPos;
};

CCB_VIEW_LOADER(BacklogItemView);

NS_BF_END

#endif /* BacklogView_hpp */
