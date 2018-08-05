//
//  ResProxy.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/9.
//

#include "ResProxy.hpp"
#include "tools/script.hpp"
#include "cocos2d.h"

USING_NS_CC;

NS_BF_BEGIN

bool SimplePack::open(std::string const& filename, std::string const& password) {
    auto paths = cocos2d::FileUtils::getInstance()->getSearchPaths();
    paths.push_back("game");
    cocos2d::FileUtils::getInstance()->setSearchPaths(paths);
    return true;
}

bool SimplePack::isEmpty() const {
    return false;
}
Data SimplePack::getData(std::string const& filename) const {
    auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(filename);
    BF::Data _data;
    _data.copy(data.getBytes(), data.getSize());
    return _data;
}


std::string const ResProxy::Name = "ResProxy";

ResProxy::ResProxy():
    PureMVC::Proxy(Name),
    mPack(NULL) {
    
}

void ResProxy::loadResPack() {
    auto utils = FileUtils::getInstance();
    std::string dataName = utils->fullPathForFilename("data.bin");
    this->mPack->open(dataName, "394811161");
}

std::string ResProxy::getSavePath() const {
    auto utils = FileUtils::getInstance();
    return utils->getWritablePath() + this->getSaveFolderName();
}

std::string ResProxy::getSaveFolderName() const {
    return "saved/";
}

ResProxy::storyScriptDataPtr ResProxy::getStoryScriptData(std::string const& filename) {
    if (this->mPack->isEmpty()) {
        return nullptr;
    }
    BF::Data temp = this->mPack->getData(filename);
    if (!temp.isNull()) {
        storyScriptData* data = new storyScriptData();
        data->load(temp.getData(), temp.getDataSize(), filename);
        temp.free();
        return storyScriptDataPtr(data);
	} else {
		CCLOG("script read fail: %s\n", filename.c_str());
	}
    return nullptr;
}

ResProxy::DataPtr ResProxy::getData(std::string const& filename) {
    if (this->mPack->isEmpty()) {
        return std::make_shared<cocos2d::Data>();
    }
    auto data = std::make_shared<cocos2d::Data>();
    BF::Data temp = this->mPack->getData(filename);
    if (!temp.isNull()) {
        data->fastSet(temp.getData(), temp.getDataSize());
        //temp.free();
	} else {
		CCLOG("data read fail: %s\n", filename.c_str());
	}
    return data;
}

cocos2d::Texture2D* ResProxy::getTexture(std::string const& filename) {
    auto director = Director::getInstance();
    auto texture = director->getTextureCache()->getTextureForKey(filename);
    if (texture != nullptr) {
        return texture;
    }
    auto data = this->getData(filename);
    if (data->isNull()) {
		CCLOG("image read fail: %s\n", filename.c_str());
        return NULL;
    }
    Image* image = new Image();
    image->autorelease();
    image->initWithImageData(data->getBytes(), data->getSize());
    return director->getTextureCache()->addImage(image, filename);
}

bool ResProxy::Encrypt(unsigned char* buffer, int byteSize) {
    return this->mPack->Encrypt(buffer, byteSize);
}

bool ResProxy::Decrypt(unsigned char* buffer, int byteSize) {
    return this->mPack->Decrypt(buffer, byteSize);
}

void ResProxy::onRegister() {
    this->mPack = PackResPtr(new SimplePack);
}

void ResProxy::onRemove() {
    this->mPack = nullptr;
}

NS_BF_END
