//
//  MediaProxy.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/11.
//

#ifndef MediaProxy_hpp
#define MediaProxy_hpp

#include <stdio.h>
#include "bftools/archives.hpp"
#include "puremvc/pmvcpp.h"

NS_BF_BEGIN

class MediaProxy : public PureMVC::Proxy {
public:
    static std::string const Name;
public:
    MediaProxy();
public:
    void playMusic(std::string const& filename);
    void stopMusic();
    void setMusicVol(int value);
    int getMusicVol() const;
    bool isMusicPlaying() const;
    void pauseMusic() const;
    void resumeMusic() const;
public:
    int playEffect(std::string const& filename);
    void stopAllEffects();
    void stopEffect(int soundId);
    void setEffectVol(int value);
public:
    std::string const& getMusicName() const;
public:
    void setMusicVolScale(float v);
    float getMusicVolScale() const;
public:
    void setSEVolScale(float v);
    float getSEVolScale() const;
public:
    bool saveConfig(bool flush = false);
    bool loadConfig();
public:
    template<class Archive>
    void save(Archive & archive) const {
        archive(mMusicVolume);
        archive(mMusicName);
    }
    template<class Archive>
    void load(Archive & archive) {
        archive(mMusicVolume);
        archive(mMusicName);
    }
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    int mMusicVolume;
    int mSEVolume;
    float mMusicVolScale;
    float mSEVolScale;
    std::string mMusicName;
};

NS_BF_END

#endif /* MediaProxy_hpp */
