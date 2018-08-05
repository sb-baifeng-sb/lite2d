//
//  SqlProxy.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/8.
//

#ifndef SqlProxy_hpp
#define SqlProxy_hpp

#include <stdio.h>
#include "app/macros.h"
#include "puremvc/pmvcpp.h"

namespace sql {
    class Database;
}

NS_BF_BEGIN

class SqlProxy : public PureMVC::Proxy {
public:
    typedef std::shared_ptr<sql::Database> DBPtr;
public:
    static std::string const Name;
public:
    SqlProxy();
private:
    void buildBacklogTable();
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    DBPtr mDB;
};

NS_BF_END
    
#endif /* SqlProxy_hpp */
