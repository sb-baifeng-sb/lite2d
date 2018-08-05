//
//  App.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/1.
//

#ifndef App_hpp
#define App_hpp

#include <stdio.h>
#include "macros.h"
#include "puremvc/pmvcpp.h"

NS_BF_BEGIN

class App {
public:
    typedef std::function<void(PureMVC::Facade*)> CallBack;
    typedef std::vector<CallBack> FuncArray;
public:
    static void addRegisterFunc(CallBack func);
    static void addRemoveFunc(CallBack func);
    static void doFuncAtOnce(CallBack func);
    
    static void fire();
    static void stop();
    
    static void notify( int notificationName, void* body, int notificationType);
    static void notify( int notificationName, int notificationType );
    static void notify( int notificationName, void* body );
    static void notify( int notificationName );
    
    static PureMVC::Facade* Facade();
    
    template<class T>
    static T* getProxy(std::string const& proxyName) {
        return dynamic_cast<T*>(Facade()->retrieveProxy(proxyName));
    }
    template<class T>
    static T* getMediator(std::string const& mediatorName) {
        return dynamic_cast<T*>(Facade()->retrieveMediator(mediatorName));
    }
    
private:
    static FuncArray _registerVec;
    static FuncArray _removeVec;
};

NS_BF_END

#endif /* App_hpp */
