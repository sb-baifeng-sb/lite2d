//
//  jsengine.hpp
//  HelloTuiCpp-mobile
//
//  Created by baifeng on 2018/4/12.
//

#ifndef jsengine_hpp
#define jsengine_hpp

#include <stdio.h>
#include "app/macros.h"
#include "js/RootingAPI.h"
#include "jsapi.h"

NS_BF_BEGIN

class JSEngine {
    friend class _JSEngineFriend;
public:
    typedef std::function<void(JSContext* cx, JS::HandleObject global)> RegisterFunc;
    typedef std::function<void(std::string const&)> LogFunc;
    typedef std::vector<RegisterFunc> RegisterFuncArray;
public:
    JSEngine();
    ~JSEngine();
public:
    void addRegisterFunc(RegisterFunc func);
    void fire();
public:
    void doCode(std::string const& code);
    void doCode(std::string const& code, std::string const& filename);
public:
    JSContext* getJSContext() const;
    JSObject* getJSGlobal() const;
private:
    bool initEngine();
    void freeEngine();
private:
    bool evalString(const char *string, JS::MutableHandleValue outVal, const char *filename, JSContext* cx, JS::HandleObject global);
    bool evalString(const char *string, JS::MutableHandleValue outVal);
    bool evalString(const char *string);
public:
    static void setErrorLogFunc(LogFunc func);
    static void setInfoLogFunc(LogFunc func);
    static void Log(const char *format, ...);
private:
    static LogFunc mErrorLogFunc;
    static LogFunc mInfoLogFunc;
private:
    static void js_log(const char *format, ...);
    static void reportError(JSContext *cx, const char *message, JSErrorReport *report);
    static void sc_finalize(JSFreeOp *freeOp, JSObject *obj);
private:
    JSRuntime* mRt;
    JSContext* mCx;
    JS::PersistentRootedObject* mGlobal;
    RegisterFuncArray mRegisterFuncs;
};

JSObject* NewObject(JSContext* cx);

//jssugar
void setJSSugarGlobalContext(JSContext* cx); // 使用jsval_to_xxx之前需要进行调用
bool jsval_to_std_string(JSContext *cx, JS::HandleValue v, std::string* ret);
bool jsval_to_int32( JSContext *cx, JS::HandleValue vp, int32_t *ret );
bool jsval_to_uint32( JSContext *cx, JS::HandleValue vp, uint32_t *ret );
bool jsval_to_float( JSContext *cx, JS::HandleValue vp, float *ret );

jsval std_string_to_jsval(JSContext* cx, const std::string& v, JSContext* globalContext = NULL, JSObject* globalObject = NULL);

NS_BF_END

#endif /* jsengine_hpp */
