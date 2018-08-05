//
//  KvProxy.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#ifndef KvProxy_hpp
#define KvProxy_hpp

#include <stdio.h>
#include "app/macros.h"
#include "puremvc/pmvcpp.h"

NS_BF_BEGIN

class KvProxy : public PureMVC::Proxy {
public:
    typedef std::map<std::string, std::string> KvMap;
public:
    static std::string const Name;
    static std::string const Common;
    static std::string const PublicKvPath;
public:
    KvProxy(std::string const& proxyName);
public:
    std::string const& getValue(std::string const& key);
    void setValue(std::string const& key, std::string const& value);
    void removeKey(std::string const& key);
    void clear();
public:
    // public kv api
    bool writeToFile(std::string const& filename);
    bool readFile(std::string const& filename);
public:
    template<class Archive>
    void save(Archive & archive) const {
        archive(mValues);
    }
    template<class Archive>
    void load(Archive & archive) {
        archive(mValues);
    }
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    std::string mEmptyStr;
    KvMap mValues;
};

NS_BF_END

#endif /* KvProxy_hpp */
