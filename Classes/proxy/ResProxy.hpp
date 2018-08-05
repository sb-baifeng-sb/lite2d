//
//  ResProxy.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/9.
//

#ifndef ResProxy_hpp
#define ResProxy_hpp

#include <stdio.h>
#include "app/macros.h"
#include "puremvc/pmvcpp.h"
#include "bftools/pack.hpp"
#include "cocos2d.h"

NS_BF_BEGIN

class SimplePack : public PackAPI {
public:
    virtual bool open(std::string const& filename, std::string const& password);
    virtual bool isEmpty() const;
    virtual Data getData(std::string const& filename) const;
};

class PackAPI;
class storyScriptData;
class ResProxy : public PureMVC::Proxy {
public:
    typedef std::shared_ptr<PackAPI> PackResPtr;
    typedef std::shared_ptr<storyScriptData> storyScriptDataPtr;
    typedef std::shared_ptr<cocos2d::Data> DataPtr;
public:
    static std::string const Name;
public:
    ResProxy();
public:
    void loadResPack();
public:
    std::string getSavePath() const;
    std::string getSaveFolderName() const;
public:
    storyScriptDataPtr getStoryScriptData(std::string const& filename);
    DataPtr getData(std::string const& filename);
    cocos2d::Texture2D* getTexture(std::string const& filename);
public:
    bool Encrypt(unsigned char* buffer, int byteSize);
    bool Decrypt(unsigned char* buffer, int byteSize);
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    std::string mRootPath;
    PackResPtr mPack;
};

NS_BF_END

#endif /* ResProxy_hpp */
