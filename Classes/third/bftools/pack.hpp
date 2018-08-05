//
//  pack.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/4.
//

#ifndef pack_hpp
#define pack_hpp

#include <stdio.h>
#include "macros.h"

NS_BF_BEGIN

class Data {
public:
    Data();
public:
    void fastSet(unsigned char* buffer, long size);
    void copy(unsigned char* buffer, long size);
    void free();
    bool isNull() const;
    unsigned char* getData() const;
    long getDataSize() const;
private:
    unsigned char* mData;
    long mDataSize;
};

class PackAPI {
public:
    ~PackAPI() {}
public:
    virtual bool open(std::string const& filename, std::string const& password) = 0;
    virtual bool isEmpty() const = 0;
    virtual Data getData(std::string const& filename) const = 0;
public:
    virtual bool Encrypt(unsigned char* buffer, int byteSize) {
		return false;
	}
    virtual bool Decrypt(unsigned char* buffer, int byteSize) {
		return false;
	}
};

NS_BF_END

#endif /* pack_hpp */
