//
//  KvProxy.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#include "KvProxy.hpp"
#include "ResProxy.hpp"
#include "app/App.hpp"
#include "bftools/archives.hpp"
#include "cocos2d.h"

NS_BF_BEGIN

std::string const KvProxy::Name = "KvProxy";
std::string const KvProxy::Common = "KvProxyCommon";
std::string const KvProxy::PublicKvPath = "pub_kv.bin";

KvProxy::KvProxy(std::string const& proxyName):PureMVC::Proxy(proxyName) {
    
}

std::string const& KvProxy::getValue(std::string const& key) {
    auto iter = this->mValues.find(key);
    if (iter == this->mValues.end()) {
        return mEmptyStr;
    }
    return iter->second;
}

void KvProxy::setValue(std::string const& key, std::string const& value) {
    this->mValues[key] = value;
}

void KvProxy::removeKey(std::string const& key) {
    this->mValues.erase(key);
}

void KvProxy::clear() {
    this->mValues.clear();
}

void KvProxy::onRegister() {
    
}

void KvProxy::onRemove() {
    
}

bool KvProxy::writeToFile(std::string const& filename) {
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto fullName = res->getSavePath() + filename;
    Archives ar;
    ar.Set([&](Archives::OutputArchive& archive) {
        archive(mValues);
    });
    
    auto str = ar.getStream().str();
    cocos2d::Data data;
    data.fastSet((unsigned char*)malloc(str.length()), str.length());
    memcpy(data.getBytes(), str.data(), str.length());
    
    res->Encrypt(data.getBytes(), (int)data.getSize());
    return cocos2d::FileUtils::getInstance()->writeDataToFile(data, fullName);
}

bool KvProxy::readFile(std::string const& filename) {
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto fullName = res->getSavePath() + filename;
    auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(fullName);
    
    if (data.isNull()) {
        return false;
    }
    
    mValues.clear();
    res->Decrypt(data.getBytes(), (int)data.getSize());
    
    Archives ar;
    std::string str((char*)data.getBytes(), data.getSize());
    ar.setStream(str);
    ar.Get([&](Archives::InputArchive& archive) {
        archive(mValues);
    });
    
    return true;
}

NS_BF_END
