//
//  archives.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#include "archives.hpp"

NS_BF_BEGIN

Archives::Archives() {
    
}

void Archives::setStream(std::string const& str) {
    this->mStream.str(str);
}

Archives::Stream const& Archives::getStream() const {
    return this->mStream;
}

Archives::Stream& Archives::getStream() {
    return this->mStream;
}

void Archives::Get(InputFunc func) {
    InputArchive archive(this->mStream);
    if (func != nullptr) {
        func(archive);
    }
}

void Archives::Set(OutputFunc func) {
    OutputArchive archive(this->mStream);
    if (func != nullptr) {
        func(archive);
    }
}

NS_BF_END
