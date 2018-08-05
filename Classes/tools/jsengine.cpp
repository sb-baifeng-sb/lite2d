//
//  jsengine.cpp
//  HelloTuiCpp-mobile
//
//  Created by baifeng on 2018/4/12.
//

#include "jsengine.hpp"
#include "jsfriendapi.h"
#include "js/OldDebugAPI.h"

NS_BF_BEGIN

class _JSEngineFriend {
public:
    static void sc_finalize(JSFreeOp *freeOp, JSObject *obj) {
        JSEngine::sc_finalize(freeOp, obj);
    }
};

// Just a wrapper around JSPrincipals that allows static construction.
class CCJSPrincipals : public JSPrincipals {
public:
    explicit CCJSPrincipals(int rc = 0):JSPrincipals() {
        refcount = rc;
    }
};

static CCJSPrincipals shellTrustedPrincipals(1);

static bool
CheckObjectAccess(JSContext *cx) {
    return true;
}

static JSSecurityCallbacks securityCallbacks = {
    CheckObjectAccess,
    NULL
};

static const JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_DeletePropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, _JSEngineFriend::sc_finalize,
    nullptr, nullptr, nullptr,
    JS_GlobalObjectTraceHook
};

JSObject* NewGlobalObject(JSContext* cx, bool debug = false) {
    JS::CompartmentOptions options;
    options.setVersion(JSVERSION_LATEST);
    
    JS::RootedObject glob(cx, JS_NewGlobalObject(cx, &global_class, &shellTrustedPrincipals, JS::DontFireOnNewGlobalHook, options));
    if (!glob) {
        return nullptr;
    }
    JSAutoCompartment ac(cx, glob);
    bool ok = true;
    ok = JS_InitStandardClasses(cx, glob);
    if (ok)
        JS_InitReflect(cx, glob);
    if (ok && debug)
        ok = JS_DefineDebuggerObject(cx, glob);
    if (!ok)
        return nullptr;
    
    JS_FireOnNewGlobalObject(cx, glob);
    
    return glob;
}

JSObject* NewObject(JSContext* cx) {
    static const JSClass _class = {
        "_", JSCLASS_GLOBAL_FLAGS,
        JS_PropertyStub, JS_DeletePropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
        JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, _JSEngineFriend::sc_finalize,
        nullptr, nullptr, nullptr,
        JS_GlobalObjectTraceHook
    };
    return JS_NewObject(cx, &_class, JS::Handle<JSObject*>(JS::NullPtr()), JS::Handle<JSObject*>(JS::NullPtr()));
}

static int _JSEngineCount = 0;
static const int MAX_LOG_LENGTH = 16*1024;
static char js_log_buf[MAX_LOG_LENGTH+1] = {0};

#ifndef JSB_MAX_STACK_QUOTA
#define JSB_MAX_STACK_QUOTA 500000
#endif // JSB_MAX_STACK_QUOTA

//====================================================================

JSEngine::LogFunc JSEngine::mErrorLogFunc = nullptr;
JSEngine::LogFunc JSEngine::mInfoLogFunc = nullptr;

JSEngine::JSEngine():mRt(NULL), mCx(NULL), mGlobal(NULL) {
    mRegisterFuncs.reserve(64);
    _JSEngineCount++;
    if (_JSEngineCount==1) {
        JS_Init();
    }
}

JSEngine::~JSEngine() {
    freeEngine();
    mRegisterFuncs.clear();
    _JSEngineCount--;
    if (_JSEngineCount==0) {
        JS_ShutDown();
    }
}

void JSEngine::addRegisterFunc(RegisterFunc func) {
    mRegisterFuncs.push_back(func);
}

void JSEngine::fire() {
    if (!this->initEngine()) {
        return;
    }
    
    JS::RootedObject global(this->mCx, this->mGlobal->get());
    // Removed in Firefox v34
    js::SetDefaultObjectForContext(this->mCx, global);
    
    JSAutoCompartment ac(this->mCx, this->mGlobal->get());
    
    for (int i=0; i < mRegisterFuncs.size(); i++) {
        auto& f = mRegisterFuncs[i];
        f(this->mCx, global);
    }
}

void JSEngine::doCode(std::string const& code) {
    this->evalString(code.c_str());
}

void JSEngine::doCode(std::string const& code, std::string const& filename) {
    JS::RootedValue outVal(this->mCx);
    JS::RootedObject global(this->mCx, this->mGlobal->get());
    evalString(code.c_str(), &outVal, filename.c_str(), this->mCx, global);
}

JSContext* JSEngine::getJSContext() const {
    return this->mCx;
}

JSObject* JSEngine::getJSGlobal() const {
    return this->mGlobal->get();
}

void JSEngine::setErrorLogFunc(LogFunc func) {
    mErrorLogFunc = func;
}

void JSEngine::setInfoLogFunc(LogFunc func) {
    mInfoLogFunc = func;
}

void JSEngine::Log(const char *format, ...) {
    if (mInfoLogFunc == nullptr) {
        return;
    }
    va_list vl;
    va_start(vl, format);
    int len = vsnprintf(js_log_buf, MAX_LOG_LENGTH, format, vl);
    va_end(vl);
    if (len > 0) {
        //CCLOG("JS: %s", _js_log_buf);
        mInfoLogFunc(js_log_buf);
    }
}

bool JSEngine::initEngine() {
    
    if (this->mRt || this->mCx) {
        return false;
    }
    
    this->mRt = JS_NewRuntime(32L * 1024L * 1024L);
    JS_SetGCParameter(this->mRt, JSGCParamKey::JSGC_MAX_BYTES, 0xffffffff);
    JS_SetGCParameter(this->mRt, JSGCParamKey::JSGC_MODE, JSGC_MODE_COMPARTMENT);
    
    JS_SetTrustedPrincipals(this->mRt, &shellTrustedPrincipals);
    JS_SetSecurityCallbacks(this->mRt, &securityCallbacks);
    JS_SetNativeStackQuota(this->mRt, JSB_MAX_STACK_QUOTA);
    
    this->mCx = JS_NewContext(this->mRt, 32 * 1024);
    
    JS::RuntimeOptionsRef(this->mRt).setIon(true);
    JS::RuntimeOptionsRef(this->mRt).setBaseline(true);
    
    JS_SetErrorReporter(this->mCx, JSEngine::reportError);
    
    this->mGlobal = new (std::nothrow) JS::PersistentRootedObject(this->mRt, NewGlobalObject(this->mCx));
    
    return true;
}

void JSEngine::freeEngine() {
    if (this->mGlobal != NULL) {
        delete this->mGlobal;
        this->mGlobal = NULL;
    }
    if (this->mCx != NULL) {
        JS_DestroyContext(this->mCx);
        this->mCx = NULL;
    }
    if (this->mRt != NULL) {
        JS_DestroyRuntime(this->mRt);
        this->mRt = NULL;
    }
}

bool JSEngine::evalString(const char *string, JS::MutableHandleValue outVal, const char *filename, JSContext* cx, JS::HandleObject global) {
    
    JSAutoCompartment ac(cx, global);
    JS::PersistentRootedScript script(cx);
    
    JS::CompileOptions op(cx);
    op.setUTF8(true);
    
    std::string content = string;
    
    bool ok = false;
    bool evaluatedOK = false;
    if (!content.empty()) {
        ok = JS::Compile(cx, global, op, content.c_str(), content.size(), &(script) );
    }
    if (ok) {
        evaluatedOK = JS_ExecuteScript(cx, global, script, outVal);
        if (false == evaluatedOK) {
            JSEngine::Log("Evaluating %s failed (evaluatedOK == JS_FALSE)", content.c_str());
            JS_ReportPendingException(cx);
        }
    }
    else {
        JSEngine::Log("ScriptingCore:: evaluateScript fail: %s", content.c_str());
    }
    return evaluatedOK;
}

bool JSEngine::evalString(const char *string, JS::MutableHandleValue outVal) {
    JS::RootedObject global(this->mCx, this->mGlobal->get());
    return evalString(string, outVal, nullptr, this->mCx, global);
}

bool JSEngine::evalString(const char *string) {
    JS::RootedValue retVal(this->mCx);
    return evalString(string, &retVal);
}

void JSEngine::reportError(JSContext *cx, const char *message, JSErrorReport *report) {
    js_log("%s:%u:%s\n",
           report->filename ? report->filename : "<no filename=\"filename\">",
           (unsigned int) report->lineno,
           message);
}

void JSEngine::sc_finalize(JSFreeOp *freeOp, JSObject *obj) {
    if (mInfoLogFunc != nullptr) {
        char buf[128] = {0};
        sprintf(buf, "jsbindings: finalizing JS object %p (global class)", obj);
        mInfoLogFunc(buf);
    }
}

void JSEngine::js_log(const char *format, ...) {
    if (mErrorLogFunc == nullptr) {
        return;
    }
    va_list vl;
    va_start(vl, format);
    int len = vsnprintf(js_log_buf, MAX_LOG_LENGTH, format, vl);
    va_end(vl);
    if (len > 0) {
        //CCLOG("JS: %s", _js_log_buf);
        mErrorLogFunc(js_log_buf);
    }
}

NS_BF_END
