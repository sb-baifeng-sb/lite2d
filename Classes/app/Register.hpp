//
//  register.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/17.
//

#ifndef Register_h
#define Register_h

#include "App.hpp"
#include "mediator/StageMediator.hpp"
#include "proxy/StoryScriptProxy.hpp"
#include "proxy/ResProxy.hpp"
#include "proxy/KvProxy.hpp"
#include "proxy/MediaProxy.hpp"
#include "proxy/SaveProxy.hpp"
#include "proxy/SqlProxy.hpp"
#include "proxy/BacklogProxy.hpp"
#include "proxy/ConfigProxy.hpp"

NS_BF_BEGIN

void registerSystem(PureMVC::Facade* facade) {
    // Command
    
    // Proxy
    facade->registerProxy(new ConfigProxy);
    facade->registerProxy(new ResProxy);
    facade->registerProxy(new MediaProxy);
    facade->registerProxy(new KvProxy(KvProxy::Name));
    facade->registerProxy(new KvProxy(KvProxy::Common));
    facade->registerProxy(new SqlProxy);
    facade->registerProxy(new StoryScriptProxy);
    facade->registerProxy(new SaveProxy);
    facade->registerProxy(new BacklogProxy);
    
    // Mediator
    facade->registerMediator(new StageMediator);
}

void removeSystem(PureMVC::Facade* facade) {
    // Mediator
    delete facade->removeMediator(StageMediator::Name);
    
    // Proxy
    delete facade->removeProxy(BacklogProxy::Name);
    delete facade->removeProxy(SaveProxy::Name);
    delete facade->removeProxy(StoryScriptProxy::Name);
    delete facade->removeProxy(SqlProxy::Name);
    delete facade->removeProxy(KvProxy::Common);
    delete facade->removeProxy(KvProxy::Name);
    delete facade->removeProxy(MediaProxy::Name);
    delete facade->removeProxy(ResProxy::Name);
    delete facade->removeProxy(ConfigProxy::Name);
    
    // Command
}

NS_BF_END

#endif /* register_h */
