//
//  jssugar.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/13.
//

#include <stdio.h>
#include "jsengine.hpp"
#include "cocos2d.h"

NS_BF_BEGIN

#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
    || (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
    #define BF_DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName &) = delete; \
        TypeName &operator =(const TypeName &) = delete;
#else
    #define BF_DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName &); \
        TypeName &operator =(const TypeName &);
#endif

#define JSB_PRECONDITION3( condition, context, ret_value, ...) do { \
    if( ! (condition) ) return (ret_value); \
} while(0)

// just a simple utility to avoid mem leaking when using JSString
class JSStringWrapper {
public:
    JSStringWrapper();
    JSStringWrapper(JSString* str, JSContext* cx = NULL);
    JSStringWrapper(jsval val, JSContext* cx = NULL);
    ~JSStringWrapper();
    void set(jsval val, JSContext* cx);
    void set(JSString* str, JSContext* cx);
    const char* get();
private:
    const char* _buffer;
    JSContext* cx;
private:
    void freeBuffer();
private:
    BF_DISALLOW_COPY_AND_ASSIGN(JSStringWrapper);
};

// JSStringWrapper
JSStringWrapper::JSStringWrapper()
: _buffer(nullptr), cx(nullptr) {
}

JSStringWrapper::JSStringWrapper(JSString* str, JSContext* cx/* = NULL*/)
: _buffer(nullptr), cx(nullptr) {
    set(str, cx);
}

JSStringWrapper::JSStringWrapper(jsval val, JSContext* cx/* = NULL*/)
: _buffer(nullptr), cx(nullptr) {
    set(val, cx);
}

JSStringWrapper::~JSStringWrapper() {
    JS_free(this->cx, (void*)_buffer);
}

void JSStringWrapper::freeBuffer() {
    if (_buffer != NULL) {
        delete [] _buffer;
        _buffer = NULL;
    }
}

void JSStringWrapper::set(jsval val, JSContext* cx) {
    if (val.isString()) {
        this->set(val.toString(), cx);
    } else {
        freeBuffer();
    }
}

void JSStringWrapper::set(JSString* str, JSContext* cx) {
    freeBuffer();
    this->cx = cx;
    JS::RootedString jsstr(cx, str);
    _buffer = JS_EncodeStringToUTF8(cx, jsstr);
}

const char* JSStringWrapper::get() {
    return _buffer ? _buffer : "";
}

//======================================================================================

static JSContext* _JSContext = NULL;

void setJSSugarGlobalContext(JSContext* cx) {
    _JSContext = cx;
}

bool jsval_to_std_string(JSContext *cx, JS::HandleValue v, std::string* ret) {
    if (v.isString() || v.isBoolean() || v.isNumber()) {
        JSString *tmp = JS::ToString(cx, v);
        JSB_PRECONDITION3(tmp, cx, false, "Error processing arguments");
        JSStringWrapper str(tmp, _JSContext);
        *ret = str.get();
        return true;
    }
    if (v.isNullOrUndefined()) {
        *ret = "";
        return true;
    }
    return false;
}

bool jsval_to_int32( JSContext *cx, JS::HandleValue vp, int32_t *outval ) {
    bool ok = true;
    double dp;
    ok &= JS::ToNumber(cx, vp, &dp);
    JSB_PRECONDITION3(ok, cx, false, "Error processing arguments");
    ok &= !std::isnan(dp);
    JSB_PRECONDITION3(ok, cx, false, "Error processing arguments");
    
    *outval = (int32_t)dp;
    
    return ok;
}

bool jsval_to_uint32( JSContext *cx, JS::HandleValue vp, uint32_t *outval ) {
    bool ok = true;
    double dp;
    ok &= JS::ToNumber(cx, vp, &dp);
    JSB_PRECONDITION3(ok, cx, false, "Error processing arguments");
    ok &= !std::isnan(dp);
    JSB_PRECONDITION3(ok, cx, false, "Error processing arguments");
    
    *outval = (uint32_t)dp;
    
    return ok;
}

bool jsval_to_float( JSContext *cx, JS::HandleValue vp, float *outval ) {
    bool ok = true;
    double dp;
    ok &= JS::ToNumber(cx, vp, &dp);
    JSB_PRECONDITION3(ok, cx, false, "Error processing arguments");
    ok &= !std::isnan(dp);
    JSB_PRECONDITION3(ok, cx, false, "Error processing arguments");
    
    *outval = (float)dp;
    
    return ok;
}

jsval c_string_to_jsval(JSContext* cx, const char* v, size_t length /* = -1 */, JSContext* globalContext, JSObject* globalObject)
{
    if (v == NULL)
    {
        return JSVAL_NULL;
    }
    if (length == -1)
    {
        length = strlen(v);
    }
    
    JSAutoCompartment __jsb_ac(globalContext, globalObject);
    
    if (0 == length)
    {
        auto emptyStr = JS_NewStringCopyZ(cx, "");
        return STRING_TO_JSVAL(emptyStr);
    }
    
    jsval ret = JSVAL_NULL;
    
#if defined(_MSC_VER) && (_MSC_VER <= 1800)
    // NOTE: Visual Studio 2013 (Platform Toolset v120) is not fully C++11 compatible.
    // It also doesn't provide support for char16_t and std::u16string.
    // For more information, please see this article
    // https://blogs.msdn.microsoft.com/vcblog/2014/11/17/c111417-features-in-vs-2015-preview/
    int utf16_size = 0;
    const jschar* strUTF16 = (jschar*)cocos2d::cc_utf8_to_utf16(v, (int)length, &utf16_size);
    
    if (strUTF16 && utf16_size > 0) {
        JSString* str = JS_NewUCStringCopyN(cx, strUTF16, (size_t)utf16_size);
        if (str) {
            ret = STRING_TO_JSVAL(str);
        }
        delete[] strUTF16;
    }
#else
    std::u16string strUTF16;
    bool ok = cocos2d::StringUtils::UTF8ToUTF16(std::string(v, length), strUTF16);
    
    if (ok && !strUTF16.empty()) {
        JSString* str = JS_NewUCStringCopyN(cx, reinterpret_cast<const jschar*>(strUTF16.data()), strUTF16.size());
        if (str) {
            ret = STRING_TO_JSVAL(str);
        }
    }
#endif
    
    return ret;
}

jsval std_string_to_jsval(JSContext* cx, const std::string& v, JSContext* globalContext, JSObject* globalObject)
{
    return c_string_to_jsval(cx, v.c_str(), v.size(), globalContext, globalObject);
}

NS_BF_END
