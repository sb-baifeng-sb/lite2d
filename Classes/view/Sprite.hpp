//
//  Sprite.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#ifndef Sprite_hpp
#define Sprite_hpp

#include <stdio.h>
#include "app/macros.h"
#include "cocos2d.h"

NS_BF_BEGIN

class AvgSpriteState;
class AvgSprite {
public:
    typedef enum {
        TYPE_NONE = 0,
        TYPE_IMG = 1,       // image
        TYPE_BTN = 2,       // button
        TYPE_GIF = 3,       // gif
        TYPE_DB = 4,        // DragonBones
        TYPE_LABEL = 5,     // label
        TYPE_HALF_SPEAK = 6,    // half speak
        TYPE_CCB_UI = 7,    // ccb
    } Type;
    typedef std::shared_ptr<AvgSpriteState> SpriteStatePtr;
public:
    AvgSprite(Type type, cocos2d::Node* target);
    virtual ~AvgSprite() {}
public:
    void setSpriteType(Type type);
    Type getSpriteType() const;
public:
    cocos2d::Node* getTargetNode() const;
    void setTargetNode(cocos2d::Node* target);
public:
    virtual SpriteStatePtr getSpriteState() = 0;
    virtual void onRegister() {}
    virtual void onRemove() {}
public:
    template<class T>
    T* type() {
        return dynamic_cast<T*>(this);
    }
private:
    cocos2d::Node* mTarget;
    Type mType;
};

class AvgSpriteState {
public:
    ~AvgSpriteState() {}
public:
    virtual void Set(AvgSprite* sprite) = 0;
    virtual void Get(AvgSprite* sprite) = 0;
public:
    template<class T>
    T* type() {
        return dynamic_cast<T*>(this);
    }
};

class SimpleSpriteState : public AvgSpriteState {
public:
    SimpleSpriteState();
public:
    virtual void Set(AvgSprite* sprite);
    virtual void Get(AvgSprite* sprite);
protected:
    bool mShow;
    int mAlpha;
    float mRotate;
    AvgSprite::Type mType;
    cocos2d::Size mSize;
    cocos2d::Vec2 mPos;
    cocos2d::Vec2 mScale;
    cocos2d::Vec2 mAnchor;
    cocos2d::Color3B mColor;
public:
    template<class Archive>
    void save(Archive & archive) const {
        archive(mShow, mAlpha, mRotate, mType);
        archive(mSize.width, mSize.height);
        archive(mPos.x, mPos.y);
        archive(mScale.x, mScale.y);
        archive(mAnchor.x, mAnchor.y);
        archive(mColor.r, mColor.g, mColor.b);
    }
    template<class Archive>
    void load(Archive & archive) {
        archive(mShow, mAlpha, mRotate, mType);
        archive(mSize.width, mSize.height);
        archive(mPos.x, mPos.y);
        archive(mScale.x, mScale.y);
        archive(mAnchor.x, mAnchor.y);
        archive(mColor.r, mColor.g, mColor.b);
    }
};

NS_BF_END

#endif /* Sprite_hpp */
