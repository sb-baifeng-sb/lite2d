//
//  Sprite.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#include "Sprite.hpp"

NS_BF_BEGIN

SimpleSpriteState::SimpleSpriteState():mShow(false), mRotate(0.0f), mType(AvgSprite::TYPE_NONE) {
    
}

void SimpleSpriteState::Set(AvgSprite* sprite) {
    sprite->getTargetNode()->setVisible(this->mShow);
    sprite->getTargetNode()->setRotation(this->mRotate);
    sprite->getTargetNode()->setContentSize(this->mSize);
    sprite->getTargetNode()->setPosition(this->mPos);
    sprite->getTargetNode()->setScale(this->mScale.x, this->mScale.y);
    sprite->getTargetNode()->setAnchorPoint(this->mAnchor);
    sprite->getTargetNode()->setColor(this->mColor);
    sprite->getTargetNode()->setOpacity(this->mAlpha);
}

void SimpleSpriteState::Get(AvgSprite* sprite) {
    this->mType = sprite->getSpriteType();
    this->mShow = sprite->getTargetNode()->isVisible();
    this->mRotate = sprite->getTargetNode()->getRotation();
    this->mSize = sprite->getTargetNode()->getContentSize();
    this->mPos = sprite->getTargetNode()->getPosition();
    this->mScale = cocos2d::Point(sprite->getTargetNode()->getScaleX(), sprite->getTargetNode()->getScaleY());
    this->mAnchor = sprite->getTargetNode()->getAnchorPoint();
    this->mColor = sprite->getTargetNode()->getColor();
    this->mAlpha = sprite->getTargetNode()->getOpacity();
}

AvgSprite::AvgSprite(Type type, cocos2d::Node* target):mType(type), mTarget(target) {
    
}

void AvgSprite::setSpriteType(Type type) {
    this->mType = type;
}

AvgSprite::Type AvgSprite::getSpriteType() const {
    return this->mType;
}

cocos2d::Node* AvgSprite::getTargetNode() const {
    return this->mTarget;
};
void AvgSprite::setTargetNode(cocos2d::Node* target) {
    this->mTarget = target;
}

NS_BF_END
