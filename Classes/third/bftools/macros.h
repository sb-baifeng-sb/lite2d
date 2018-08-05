//
//  macros.h
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/1.
//

#ifndef bftools_macros_h
#define bftools_macros_h

#ifdef __cplusplus
#define NS_BF_BEGIN                     namespace BF {
#define NS_BF_END                       }
#define USING_NS_BF                     using namespace BF
#else
#define NS_BF_BEGIN
#define NS_BF_END
#define USING_NS_BF
#endif

#define BF_GETTER(type, name) \
public: \
    type get##name() const { \
        return m##name; \
    } \
    void set##name(type v) { \
        m##name = v; \
    } \
private: \
    type m##name;

#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <map>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
namespace std {
	template <typename T>
	std::string to_string(T value) {
    	std::ostringstream os;
    	os << value;
    	return os.str();
	}
}
#endif

#endif /* macros_h */
