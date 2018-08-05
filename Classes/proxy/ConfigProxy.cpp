//
//  ConfigProxy.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/7/1.
//

#include "ConfigProxy.hpp"
#include "bftools/parseini.hpp"
#include "cocos2d.h"

NS_BF_BEGIN

template <class Type>
Type stringToNum(std::string const& str) {
    std::istringstream iss(str);
    Type num = 0;
    iss >> num;
    return num;
}

std::string const ConfigProxy::Name = "ConfigProxy";

ConfigProxy::Session::Session(ConfigProxy::Config const& config):mConfig(config) {
    
}

int ConfigProxy::Session::getValueAsInt(std::string const& name) const {
    auto v = this->getValueAsStr(name);
    if (v.length() == 0) {
        return 0;
    }
    return stringToNum<int>(v);
}

std::string const& ConfigProxy::Session::getValueAsStr(std::string const& name) const {
    auto iter = this->mConfig.find(name);
    if (iter != this->mConfig.end()) {
        return iter->second;
    }
    return mValue;
}

double ConfigProxy::Session::getValueAsDouble(std::string const& name) const {
    auto v = this->getValueAsStr(name);
    if (v.length() == 0) {
        return 0.0f;
    }
    return stringToNum<double>(v);
}

ConfigProxy::ConfigProxy():PureMVC::Proxy(Name) {
    
}

ConfigProxy::SessionPtr ConfigProxy::getSession(std::string const& session_name) const {
    auto iter = this->mConfigs.find(session_name);
    if (iter != this->mConfigs.end()) {
        auto& config = iter->second;
        auto ptr = std::make_shared<Session>(config);
        return ptr;
    }
    return nullptr;
}

void ConfigProxy::onRegister() {
    auto data = cocos2d::FileUtils::getInstance()->getDataFromFile("config.ini");
    if (data.isNull()) {
        return;
    }
    std::string source((char*)data.getBytes(), data.getSize());
    CParseIniFile ini;
    auto& engine = this->mConfigs["engine"];
    auto& ui = this->mConfigs["ui"];
    ini.ReadConfig(source, engine, "engine");
    ini.ReadConfig(source, ui, "ui");
}

void ConfigProxy::onRemove() {
    
}

NS_BF_END
