//
//  parseini.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/7/1.
//

#include "parseini.hpp"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>

#define COMMENT_CHAR '#'
using namespace std;

NS_BF_BEGIN

CParseIniFile::CParseIniFile() {
}

CParseIniFile::~CParseIniFile() {
    
}

bool CParseIniFile::ReadConfig(std::string const& source, map<string, string>& mContent, char const* section) {
    if (source.length() == 0) {
        return false;
    }
    mContent.clear();
    stringstream infile;
    infile.str(source);
    string line, key, value;
    int pos = 0;
    string Tsection = string("[") + section + "]";
    bool flag = false;
    while (getline(infile, line)) {
        if(!flag) {
            pos = (int)line.find(Tsection, 0);
            if(-1 == pos) {
                continue;
            } else {
                flag = true;
                getline(infile, line);
            }
        }
        if(0 < line.length() && '[' == line.at(0)) {
            break;
        }
        if (0 < line.length() && AnalyseLine(line, key, value)) {
            if (value.length() > 0) {
                if(value[value.size()-1] == '\r') {
                    value[value.size()-1]='\0';
                }
            }
            mContent[key] = value;
        }
    }
    //infile.close();
    return true;
}

bool CParseIniFile::AnalyseLine(std::string const& line, string& key, string& val)
{
    if (line.empty()) {
        return false;
    }
    int start_pos = 0, end_pos = (int)line.size() - 1, pos = 0;
    if ((pos = (int)line.find(COMMENT_CHAR)) != -1) {
        if (0 == pos) {//行的第一个字符就是注释字符
            return false;
        }
        end_pos = pos - 1;
    }
    string new_line = line.substr(start_pos, start_pos + 1 - end_pos);  // 预处理，删除注释部分
    
    if ((pos = (int)new_line.find('=')) == -1) {
        return false;  // 没有=号
    }
    
    key = new_line.substr(0, pos);
    val = new_line.substr(pos + 1, end_pos + 1- (pos + 1));
    
    Trim(key);
    if (key.empty()) {
        return false;
    }
    Trim(val);
    return true;
}

void CParseIniFile::Trim(string& str) {
    if (str.empty()) {
        return;
    }
    int i, start_pos, end_pos;
    for (i = 0; i < str.size(); ++i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    if (i == str.size()) { //全部是空白字符串
        str = "";
        return;
    }
    
    start_pos = i;
    
    for (i = (int)str.size() - 1; i >= 0; --i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    end_pos = i;
    
    str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool CParseIniFile::IsSpace(char c) {
    if (' ' == c || '\t' == c) {
        return true;
    }
    return false;
}

bool CParseIniFile::IsCommentChar(char c) {
    switch(c) {
        case COMMENT_CHAR:
            return true;
        default:
            return false;
    }
}

void CParseIniFile::PrintConfig(map<string, string> const& mContent) {
    map<string, string>::const_iterator mite = mContent.begin();
    for (; mite != mContent.end(); ++mite) {
        cout << mite->first << "=" << mite->second << endl;
    }
}

NS_BF_END
