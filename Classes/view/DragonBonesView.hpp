//
//  DragonBonesView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/17.
//

#ifndef DragonBonesView_hpp
#define DragonBonesView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "dragonBones/cocos2dx/CCArmatureDisplay.h"
#include "Sprite.hpp"

NS_BF_BEGIN

class DBSpriteState : public SimpleSpriteState {
public:
    DBSpriteState();
public:
    virtual void Set(AvgSprite* sprite);
    virtual void Get(AvgSprite* sprite);
public:
    std::string const& getDragonBonesName() const;
    std::string const& getArmatureName() const;
private:
    int mTimes;
    std::string mDragonBonesName;
    std::string mArmatureName;
    std::string mAnimationName;
public:
    template<class Archive>
    void save(Archive & archive) const {
        SimpleSpriteState const& super = *this;
        archive(super);
        archive(mTimes, mDragonBonesName, mArmatureName, mAnimationName);
    }
    template<class Archive>
    void load(Archive & archive) {
        SimpleSpriteState& super = *this;
        archive(super);
        archive(mTimes, mDragonBonesName, mArmatureName, mAnimationName);
    }
};

class DragonBonesView : public dragonBones::CCArmatureDisplay, public BF::AvgSprite {
    friend class DBSpriteState;
public:
    CREATE_FUNC(DragonBonesView);
    DragonBonesView();
public:
    void saveResName(std::string const& dragonBonesName, std::string const& armatureName);
    void play(std::string const& aniName, int times = 0);
public:
    template<class Archive>
    void serialize(Archive & archive) {
        //需要进行存储的变量
    }
    virtual SpriteStatePtr getSpriteState();
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    int mTimes;
    std::string mDragonBonesName;
    std::string mArmatureName;
    std::string mAnimationName;
};

NS_BF_END

#endif /* DragonBonesView_hpp */
