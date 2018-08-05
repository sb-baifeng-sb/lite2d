//
//  TextView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/30.
//

#ifndef TextView_hpp
#define TextView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "Sprite.hpp"
#include "cocos2d.h"

NS_BF_BEGIN

class TextSpriteState : public SimpleSpriteState {
public:
    TextSpriteState();
public:
    virtual void Set(AvgSprite* sprite);
    virtual void Get(AvgSprite* sprite);
private:
    int mWidth;
    cocos2d::TextHAlignment mAlign;
    std::string mTitle;
public:
    template<class Archive>
    void save(Archive & archive) const {
        SimpleSpriteState const& super = *this;
        archive(super);
        archive(mAlign, mWidth, mTitle);
    }
    template<class Archive>
    void load(Archive & archive) {
        SimpleSpriteState& super = *this;
        archive(super);
        archive(mAlign, mWidth, mTitle);
    }
};

class TextView : public cocos2d::Label, public BF::AvgSprite {
    friend class TextSpriteState;
public:
    CREATE_FUNC(TextView);
    TextView();
public:
    virtual SpriteStatePtr getSpriteState();
};

NS_BF_END

#endif /* TextView_hpp */
