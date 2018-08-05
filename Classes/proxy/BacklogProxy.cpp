//
//  BacklogProxy.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/9.
//

#include "BacklogProxy.hpp"

NS_BF_BEGIN

std::string const BacklogProxy::Name = "BacklogProxy";

BacklogProxy::BacklogProxy():PureMVC::Proxy(Name) {
    
}

void BacklogProxy::setName(std::string const& name) {
    this->mName = name;
}

void BacklogProxy::setText(std::string const& text) {
    this->mText = text;
}

void BacklogProxy::setSound(std::string const& sound) {
    this->mSound = sound;
}

void BacklogProxy::insertLog() {
    this->insertLog(mName, mText, mSound);
}

int BacklogProxy::getLogsSize() const {
    return (int)mLogs.size();
}

BacklogProxy::Log const& BacklogProxy::getLog(int index) const {
    return mLogs[index];
}

void BacklogProxy::insertLog(std::string const& name, std::string const& text, std::string const& sound) {
    mLogs.push_back({
        0, name, text, sound,
    });
}

void BacklogProxy::updateLogHeight(int index, int height) {
    auto& log = mLogs[index];
    log.height = height;
}

void BacklogProxy::clear() {
    mLogs.clear();
}

void BacklogProxy::onRegister() {
    mLogs.reserve(1024);
}

void BacklogProxy::onRemove() {
    mLogs.clear();
}

NS_BF_END
