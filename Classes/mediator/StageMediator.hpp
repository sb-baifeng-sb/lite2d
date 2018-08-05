//
//  StageMediator.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/1.
//

#ifndef StageMediator_hpp
#define StageMediator_hpp

#include <stdio.h>
#include "app/macros.h"
#include "../view/SpriteStackView.hpp"
#include "puremvc/pmvcpp.h"
#include "cocos2d.h"

NS_BF_BEGIN

//class SpriteStackView;
class StageMediator : public PureMVC::Mediator {
public:
    typedef SpriteStackView SpriteStack;
    typedef std::vector<SpriteStack*> SpriteStackArray;
    typedef std::function<void(std::string const& pngPath)> ScreenShotCallBack;
public:
    static std::string const Name;
public:
    StageMediator();
public:
    int getSpriteStackSize() const;
    void pushSpriteStack(int stackSize);
    void popSpriteStack();
    void popAllSpriteStack();
public:
    SpriteStack* topSpriteStack() const;
    SpriteStack* getSpriteStack(int index) const;
    cocos2d::Node* getViewHolder() const;
public:
    void getGameScreenShot(std::string const& screenshot_name, ScreenShotCallBack callback);
public:
    template<class Archive>
    void save(Archive & archive) const {
        int size = (int)mSpriteStack.size();
        archive(size);
        
        for (int i=0; i < size; i++) {
            auto view = mSpriteStack[i];
            archive(*view);
        }
    }
    template<class Archive>
    void load(Archive & archive) {
        int size = 0;
        archive(size);
        
        this->popAllSpriteStack();
        for (int i=0; i < size; i++) {
            this->pushSpriteStack(0);
            auto view = this->topSpriteStack();
            archive(*view);
        }
    }
private:
    void makeFirstScene();
private:
    virtual std::vector<int> listNotificationInterests();
    virtual void handleNotification( PureMVC::INotification* notification );
    virtual void onRegister();
    virtual void onRemove();
private:
    cocos2d::Scene* mScene;
    cocos2d::Node* mViewHoder;
    SpriteStackArray mSpriteStack;
    ScreenShotCallBack mShotCall;
};

NS_BF_END

#endif /* StageMediator_hpp */
