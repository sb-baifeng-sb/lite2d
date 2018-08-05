//
//  script.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/10.
//

#ifndef script_hpp
#define script_hpp

#include <stdio.h>
#include "app/macros.h"

NS_BF_BEGIN

class storyScriptData {
public:
    typedef std::function<void(int index, unsigned char* curr_buffer, long buf_size)> TraveralFunc;
public:
    storyScriptData();
    ~storyScriptData();
public:
    void load(unsigned char* buffer, long bufSize);
    void load(unsigned char* buffer, long bufSize, std::string const& filename);
    void travel(TraveralFunc func) const;
public:
    long getDataSize() const;
    std::string const& getFileName() const;
private:
    void clear();
private:
    unsigned char* mData;
    long mDataSize;
    long mLineSize;
    std::string mFileName;
};

class storyScript {
public:
    typedef std::vector<std::string> ScriptList;
    typedef std::map<std::string, int> TagMap;
    typedef std::function<void(std::string const& line)> ScriptStepFunc;
public:
    storyScript();
    ~storyScript();
public:
    void load(storyScriptData* data);
public:
    void back(int step = 1);
    void seek(std::string const& tag);
    void seek(int index);
    void line(ScriptStepFunc func);
    bool isEnd();
public:
    void update(float dt);
    void pause(float seconds);
    bool isPause() const;
    void setPauseTime(int curTime, int endTime);
public:
    int getIndex() const;
    int getCurTime() const;
    int getEndTime() const;
    std::string const& getName() const;
private:
    void clear();
    int getTagValue(std::string const& tag) const;
private:
    void onAdd(int index, unsigned char* curr_buffer, long buf_size);
private:
    int mIndex; // 脚本游标
    int mCurTime, mEndTime;
    std::string mName; // 脚本名字
    TagMap mTags; // 标签行号映射
    ScriptList mScript; // 剧本脚本集合
};

NS_BF_END

#endif /* script_hpp */
