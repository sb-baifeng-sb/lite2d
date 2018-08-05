//
//  BacklogProxy.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/9.
//

#ifndef BacklogProxy_hpp
#define BacklogProxy_hpp

#include <stdio.h>
#include "app/macros.h"
#include "puremvc/pmvcpp.h"

NS_BF_BEGIN

class BacklogProxy : public PureMVC::Proxy {
public:
    typedef struct {
        int height;
        std::string name;
        std::string text;
        std::string sound;
    } Log;
    typedef std::vector<Log> LogArray;
public:
    static std::string const Name;
public:
    BacklogProxy();
public:
    void setName(std::string const& name);
    void setText(std::string const& text);
    void setSound(std::string const& sound);
    void insertLog();
public:
    int getLogsSize() const;
    Log const& getLog(int index) const;
    void insertLog(std::string const& name, std::string const& text, std::string const& sound);
    void updateLogHeight(int index, int height);
    void clear();
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    std::string mName;
    std::string mText;
    std::string mSound;
    LogArray mLogs;
};

NS_BF_END

#endif /* BacklogProxy_hpp */
