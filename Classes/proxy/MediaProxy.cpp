//
//  MediaProxy.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/11.
//

#include "MediaProxy.hpp"
#include "audio/include/SimpleAudioEngine.h"
#include "cocos2d.h"

using namespace CocosDenshion;

NS_BF_BEGIN

std::string const MediaProxy::Name = "MediaProxy";

MediaProxy::MediaProxy():PureMVC::Proxy(Name), mMusicVolScale(1.0f), mMusicVolume(255), mSEVolScale(1.0f), mSEVolume(255) {
    
}

void MediaProxy::onRegister() {
    this->loadConfig();
}

void MediaProxy::onRemove() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

void MediaProxy::setMusicVolScale(float v) {
    this->mMusicVolScale = v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
    this->setMusicVol(this->mMusicVolume);
}

float MediaProxy::getMusicVolScale() const {
    return this->mMusicVolScale;
}

void MediaProxy::setSEVolScale(float v) {
    this->mSEVolScale = v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
    this->setEffectVol(this->mSEVolume);
}

float MediaProxy::getSEVolScale() const {
    return this->mSEVolScale;
}

bool MediaProxy::saveConfig(bool flush) {
    auto userdata = cocos2d::UserDefault::getInstance();
    userdata->setIntegerForKey("MediaProxy:mMusicVolume", mMusicVolume);
    userdata->setIntegerForKey("MediaProxy:mSEVolume", mSEVolume);
    userdata->setFloatForKey("MediaProxy:mMusicVolScale", mMusicVolScale);
    userdata->setFloatForKey("MediaProxy:mSEVolScale", mSEVolScale);
    userdata->setBoolForKey("MediaProxy:hasConfig", true);
    if (flush) {
        userdata->flush();
    }
    return true;
}

bool MediaProxy::loadConfig() {
    auto userdata = cocos2d::UserDefault::getInstance();
    if (!userdata->getBoolForKey("MediaProxy:hasConfig")) {
        return false;
    }
    mMusicVolume = userdata->getIntegerForKey("MediaProxy:mMusicVolume");
    mSEVolume = userdata->getIntegerForKey("MediaProxy:mSEVolume");
    mMusicVolScale = userdata->getFloatForKey("MediaProxy:mMusicVolScale");
    mSEVolScale = userdata->getFloatForKey("MediaProxy:mSEVolScale");
    return true;
}

std::string const& MediaProxy::getMusicName() const {
    return this->mMusicName;
}

void MediaProxy::playMusic(std::string const& filename) {
    this->setMusicVol(mMusicVolume);
    SimpleAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), true);
    this->mMusicName = filename;
}

void MediaProxy::stopMusic() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    this->mMusicName = "";
}

void MediaProxy::setMusicVol(int value) {
    float _value = value * mMusicVolScale;
    float v = _value / 255.0f;
    this->mMusicVolume = value;
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(v > 1.0f ? 1.0f : (v < 0.0f ? 0.0f : v));
}

int MediaProxy::getMusicVol() const {
    return this->mMusicVolume;
}

bool MediaProxy::isMusicPlaying() const {
    return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

void MediaProxy::pauseMusic() const {
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void MediaProxy::resumeMusic() const {
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

int MediaProxy::playEffect(std::string const& filename) {
    this->setEffectVol(mSEVolume);
    return SimpleAudioEngine::getInstance()->playEffect(filename.c_str());
}

void MediaProxy::stopAllEffects() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

void MediaProxy::stopEffect(int soundId) {
    SimpleAudioEngine::getInstance()->stopEffect(soundId);
}

void MediaProxy::setEffectVol(int value) {
    float _value = value * mSEVolScale;
    float v = _value / 255.0f;
    this->mSEVolume = value;
    SimpleAudioEngine::getInstance()->setEffectsVolume(v > 1.0f ? 1.0f : (v < 0.0f ? 0.0f : v));
}

NS_BF_END
