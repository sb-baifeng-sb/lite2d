//
//  App.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/1.
//

#include "App.hpp"

NS_BF_BEGIN

static std::string appKey = "_GalEngine";
static bool _fire = false;

App::FuncArray App::_registerVec;
App::FuncArray App::_removeVec;

void App::addRegisterFunc(CallBack func) {
    _registerVec.push_back(func);
}

void App::addRemoveFunc(CallBack func) {
    _removeVec.push_back(func);
}

void App::doFuncAtOnce(CallBack func) {
    func(App::Facade());
}

void App::fire() {
    if (_fire) {
        return;
    }
    _fire = true;
    PureMVC::Facade* facade = App::Facade();
    for (int i=0; i < (int)_registerVec.size(); i++) {
        _registerVec[i](facade);
    }
}

void App::stop() {
    if (!_fire) {
        return;
    }
    _fire = false;
    PureMVC::Facade* facade = App::Facade();
    for (int i=0; i < (int)_removeVec.size(); i++) {
        _removeVec[i](facade);
    }
}

PureMVC::Facade* App::Facade() {
    return PureMVC::Facade::getInstance(appKey);
}

void App::notify( int notificationName, void* body, int notificationType) {
    Facade()->sendNotification(notificationName, body, notificationType);
}

void App::notify( int notificationName, int notificationType ) {
    Facade()->sendNotification(notificationName, notificationType);
}

void App::notify( int notificationName, void* body ) {
    Facade()->sendNotification(notificationName, body);
}

void App::notify( int notificationName ) {
    Facade()->sendNotification(notificationName);
}

NS_BF_END
