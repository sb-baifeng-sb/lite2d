//
//  ConfigProxy.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/7/1.
//

#ifndef ConfigProxy_hpp
#define ConfigProxy_hpp

#include <stdio.h>
#include "puremvc/pmvcpp.h"
#include "app/macros.h"

NS_BF_BEGIN

class ConfigProxy : public PureMVC::Proxy {
public:
    static std::string const Name;
public:
    typedef std::map<std::string, std::string> Config;
    typedef std::map<std::string, Config> ConfigMap;
public:
    class Session {
    public:
        explicit Session(Config const& conf);
    public:
        int getValueAsInt(std::string const& name) const;
        std::string const& getValueAsStr(std::string const& name) const;
        double getValueAsDouble(std::string const& name) const;
    private:
        std::string mValue;
        Config const& mConfig;
    };
    typedef std::shared_ptr<Session> SessionPtr;
public:
    ConfigProxy();
public:
    SessionPtr getSession(std::string const& session_name) const;
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    ConfigMap mConfigs;
};

NS_BF_END

#endif /* ConfigProxy_hpp */
