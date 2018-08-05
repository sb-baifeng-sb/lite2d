//
//  pack.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/4.
//

#include "pack.hpp"
#include "stdlib.h"

NS_BF_BEGIN

Data::Data():mData(NULL),mDataSize(0) {
}

void Data::fastSet(unsigned char* buffer, long size) {
    this->free();
    this->mData = buffer;
    this->mDataSize = size;
}

void Data::copy(unsigned char* buffer, long size) {
    this->free();
    this->mDataSize = size;
    this->mData = (unsigned char*)malloc(size);
    memcpy(this->mData, buffer, size);
}

void Data::free() {
    if (this->mData != NULL) {
        ::free(this->mData);
        this->mData = NULL;
    }
    this->mDataSize = 0;
}

bool Data::isNull() const {
    return this->mData == NULL;
}

unsigned char* Data::getData() const {
    return this->mData;
}

long Data::getDataSize() const {
    return this->mDataSize;
}

NS_BF_END
