//
//  GifView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/17.
//

#ifndef GifView_hpp
#define GifView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "Gif/GifBase.h"
#include "Sprite.hpp"

NS_BF_BEGIN

class GifSpriteState : public SimpleSpriteState {
public:
    GifSpriteState();
public:
    virtual void Set(AvgSprite* sprite);
    virtual void Get(AvgSprite* sprite);
private:
    std::string mFilename;
    int mTimes;
public:
    template<class Archive>
    void save(Archive & archive) const {
        SimpleSpriteState const& super = *this;
        archive(super);
        archive(mFilename, mTimes);
    }
    template<class Archive>
    void load(Archive & archive) {
        SimpleSpriteState& super = *this;
        archive(super);
        archive(mFilename, mTimes);
    }
};

class ResProxy;
class GIFMovieData;
class GifView : public GifBase, public BF::AvgSprite {
    friend class GifSpriteState;
public:
    CREATE_FUNC(GifView);
    GifView();
    ~GifView();
public:
    void load(cocos2d::Data const& data);
    void play(int times = 0);
    void stop();
    bool isPlaying() const;
public:
    void loadFromResProxy(std::string const& filename, ResProxy* res = nullptr);
    std::string const& getFileName() const;
public:
    template<class Archive>
    void serialize(Archive & archive) {
        //需要进行存储的变量
    }
    virtual SpriteStatePtr getSpriteState();
private:
    void clear();
    virtual void updateGif(uint32_t delta);
    virtual std::string getGifFrameName(int index);
private:
    bool mUpdate;
    uint32_t mTimes, mMaxTimes;
    uint32_t m_instantGifId;
    GIFMovieData* mMovie;
    std::string mFilename;
};

NS_BF_END

#endif /* GifView_hpp */
