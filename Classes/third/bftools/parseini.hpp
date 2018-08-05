//
//  parseini.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/7/1.
//

#ifndef parseini_hpp
#define parseini_hpp

#include <string>
#include <map>
#include "macros.h"

NS_BF_BEGIN

class CParseIniFile {
public:
    CParseIniFile();
    ~CParseIniFile();
    bool ReadConfig(std::string const& source, std::map<std::string, std::string>& mContent, char const* section);
    bool AnalyseLine(std::string const& line, std::string& key, std::string& val);
    void Trim(std::string& str);
    bool IsSpace(char c);
    bool IsCommentChar(char c);
    void PrintConfig(std::map<std::string, std::string> const& mContent);
private:

};

NS_BF_END

#endif /* parseini_hpp */
