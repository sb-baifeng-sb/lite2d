//
//  SqlProxy.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/8.
//

#include "SqlProxy.hpp"
#include "easySQLite/easySqlite.h"
#include "cocos2d.h"

NS_BF_BEGIN

std::string const SqlProxy::Name = "SqlProxy";

SqlProxy::SqlProxy():PureMVC::Proxy(Name) {
    
}

void SqlProxy::buildBacklogTable() {
    
}

void SqlProxy::onRegister() {
    auto fileutil = cocos2d::FileUtils::getInstance();
    auto dbPath = fileutil->getWritablePath() + "cache.db";
    mDB = std::make_shared<sql::Database>();
    
    try {
        mDB->open(dbPath.c_str());
        this->buildBacklogTable();
        
    } catch (Exception e) {
        CCLOG("SqlProxy onRegister error: %s", e.msg().c_str());
    }
}

void SqlProxy::onRemove() {
    try {
        mDB = nullptr;
    } catch (Exception e) {
        CCLOG("SqlProxy onRemove error: %s", e.msg().c_str());
    }
    
}

NS_BF_END
