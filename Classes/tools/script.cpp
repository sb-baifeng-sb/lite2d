//
//  script.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/10.
//

#include <stdlib.h>
#include <math.h>
#include "script.hpp"
#include "bftools/simpleapi.hpp"

NS_BF_BEGIN

std::vector<std::string> split(std::string strtem, char a) {
    std::vector<std::string> strvec;
    std::string::size_type pos1, pos2;
    pos2 = strtem.find(a);
    pos1 = 0;
    while (std::string::npos != pos2) {
        strvec.push_back(strtem.substr(pos1, pos2 - pos1));
        
        pos1 = pos2 + 1;
        pos2 = strtem.find(a, pos1);
    }
    strvec.push_back(strtem.substr(pos1));
    return strvec;
}

void removeInvalidSymbol(std::string& s) {
	while (true) {
		if (s.size() == 0) {
			return;
		}
        if (s[0] == ' ') {
            s = s.substr(1);
            continue;
        }
        if (s.back() == '\r' || s.back() == '\t') {
            s = s.substr(0, s.size()-1);
            continue;
        }
        break;
    }
}

//=====================================================================

storyScriptData::storyScriptData():mData(NULL),mDataSize(0),mLineSize(0) {
    
}

storyScriptData::~storyScriptData() {
    clear();
}

void storyScriptData::clear() {
    if (this->mData != NULL) {
        free(this->mData);
        this->mData = NULL;
    }
    this->mDataSize = 0;
    this->mLineSize = 0;
}

void storyScriptData::load(unsigned char* buffer, long bufSize) {
    
    std::string s((char*)buffer, bufSize);
    //分割换行符
    std::vector<std::string> array = split(s, '\n');
    //printf("line: %d\n", (int)array.size());
    
    int realSize = 0;
    std::vector<int> indexArray;
    indexArray.reserve(array.size());
    
    for (int i=0; i < array.size(); i++) {
        std::string& stream = array[i];
        removeInvalidSymbol(stream);
        int size = (int)stream.size();
        if (size != 0) {
            indexArray.push_back(i);
            realSize += size;
        }
    }
    clear();
    
    this->mDataSize = realSize+sizeof(int)*indexArray.size();
    this->mData = (unsigned char*)malloc(this->mDataSize);
    this->mLineSize = indexArray.size();
    memset(this->mData, 0, this->mDataSize);
    
    int offset = 0;
    for (int i=0; i < indexArray.size(); i++) {
        int index = (int)indexArray[i];
        std::string const& line = array[index];
        int size = (int)line.size();
        unsigned char* curr = this->mData + offset;
        memcpy(curr, &size, sizeof(int));
        curr += sizeof(int);
        memcpy(curr, line.data(), line.size());
        offset += sizeof(int) + size;
    }
}

void storyScriptData::load(unsigned char* buffer, long bufSize, std::string const& filename) {
    this->load(buffer, bufSize);
    this->mFileName = filename;
}

void storyScriptData::travel(TraveralFunc func) const {
    int offset = 0;
    for (int i=0; i < this->mLineSize; i++) {
        unsigned char* buffer = this->mData + offset;
        int bufSize = 0;
        memcpy(&bufSize, buffer, sizeof(int));
        func(i, buffer+sizeof(int), bufSize);
        offset += bufSize + sizeof(int);
    }
}

long storyScriptData::getDataSize() const {
    return this->mDataSize;
}

std::string const& storyScriptData::getFileName() const {
    return this->mFileName;
}

//=====================================================================

storyScript::storyScript():mIndex(0), mCurTime(0), mEndTime(0) {
    mScript.reserve(1024);
}

storyScript::~storyScript() {
    clear();
}

void storyScript::load(storyScriptData* data) {
    clear();
    data->travel(std::bind(
        &storyScript::onAdd, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    );
    this->mName = data->getFileName();
}

void storyScript::back(int step) {
    mIndex -= fabs((float)step);
}

void storyScript::seek(std::string const& tag) {
    int linenum = getTagValue(tag);
    if (linenum >= 0) {
        this->seek(linenum);
    }
}

void storyScript::seek(int index) {
    if (index >= 0) {
        mIndex = index;
    }
}

void storyScript::line(ScriptStepFunc func) {
    if (isEnd()) {
        return;
    }
    std::string const& line = mScript[mIndex];
    func(line);
    mIndex++;
}

bool storyScript::isEnd() {
    return mIndex >= mScript.size();
}

void storyScript::update(float dt) {
    if (isPause()) {
        this->mCurTime += dt * 1000;
        if (this->mCurTime >= this->mEndTime) {
            this->mCurTime = 0;
            this->mEndTime = 0;
        }
    }
}

void storyScript::pause(float seconds) {
    this->mCurTime = 0;
    this->mEndTime = seconds * 1000;
    if (this->mEndTime < 0) {
        this->mEndTime = 0x0fffffff;
    }
}

bool storyScript::isPause() const {
    return this->mCurTime != this->mEndTime;
}

void storyScript::setPauseTime(int curTime, int endTime) {
    this->mCurTime = curTime;
    this->mEndTime = endTime;
}

int storyScript::getIndex() const {
    return mIndex;
}
int storyScript::getCurTime() const {
    return mCurTime;
}
int storyScript::getEndTime() const {
    return mEndTime;
}
std::string const& storyScript::getName() const {
    return mName;
}

void storyScript::clear() {
    mScript.clear();
    mTags.clear();
    mIndex = 0;
}

int storyScript::getTagValue(std::string const& tag) const {
    TagMap::const_iterator iter = mTags.find(tag);
    if (mTags.end() == iter) {
        return -1;
    }
    return iter->second;
}

void storyScript::onAdd(int index, unsigned char* curr_buffer, long buf_size) {
    std::string line((char*)curr_buffer, buf_size);
    mScript.push_back(line);
    if (line[0] == '*') {
        mTags[line] = index;
    }
}

NS_BF_END
