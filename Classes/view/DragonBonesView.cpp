//
//  DragonBonesView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/17.
//

#include "DragonBonesView.hpp"

NS_BF_BEGIN

DBSpriteState::DBSpriteState():mTimes(0) {
    
}

void DBSpriteState::Set(BF::AvgSprite *sprite) {
    auto view = sprite->type<DragonBonesView>();
    if (view != nullptr) {
        view->saveResName(this->mDragonBonesName, this->mArmatureName);
        view->play(this->mAnimationName, this->mTimes);
    }
    SimpleSpriteState::Set(sprite);
}

void DBSpriteState::Get(BF::AvgSprite *sprite) {
    SimpleSpriteState::Get(sprite);
    auto view = sprite->type<DragonBonesView>();
    if (view == nullptr) {
        return;
    }
    this->mDragonBonesName = view->mDragonBonesName;
    this->mArmatureName = view->mArmatureName;
    this->mAnimationName = view->mAnimationName;
    this->mTimes = view->mTimes;
}

std::string const& DBSpriteState::getDragonBonesName() const {
    return this->mDragonBonesName;
}

std::string const& DBSpriteState::getArmatureName() const {
    return this->mArmatureName;
}

DragonBonesView::DragonBonesView():AvgSprite(TYPE_DB, this), mTimes(0) {
}

void DragonBonesView::saveResName(std::string const& dragonBonesName, std::string const& armatureName) {
    this->mDragonBonesName = dragonBonesName;
    this->mArmatureName = armatureName;
}

void DragonBonesView::play(std::string const& aniName, int times) {
    this->getAnimation()->play(aniName, times);
    this->mAnimationName = aniName;
    this->mTimes = times;
}

DragonBonesView::SpriteStatePtr DragonBonesView::getSpriteState() {
    SpriteStatePtr state(new DBSpriteState);
    state->Get(this);
    return state;
}

void DragonBonesView::onRegister() {
    
}

void DragonBonesView::onRemove() {
    this->dispose();
}

NS_BF_END
