//
//  ImageView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#ifndef ImageView_hpp
#define ImageView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "Sprite.hpp"

NS_BF_BEGIN

class ImageSpriteState : public SimpleSpriteState {
public:
    ImageSpriteState();
public:
    virtual void Set(AvgSprite* sprite);
    virtual void Get(AvgSprite* sprite);
public:
    std::string const& getFileName() const;
private:
    std::string mFilename;
public:
    template<class Archive>
    void save(Archive & archive) const {
        SimpleSpriteState const& super = *this;
        archive(super);
        archive(mFilename);
    }
    template<class Archive>
    void load(Archive & archive) {
        SimpleSpriteState& super = *this;
        archive(super);
        archive(mFilename);
    }
};

class ResProxy;
class BFImageView : public cocos2d::Sprite, public BF::AvgSprite {
    friend class ImageSpriteState;
public:
	CREATE_FUNC(BFImageView);
	BFImageView();
public:
    void loadImageFromResProxy(std::string const& filename, ResProxy* res = nullptr);
public:
    template<class Archive>
    void save(Archive & archive) const {
        
    }
    template<class Archive>
    void load(Archive & archive) {
        
    }
public:
    virtual SpriteStatePtr getSpriteState();
private:
    std::string mFilename;
};

NS_BF_END

#endif /* ImageView_hpp */
