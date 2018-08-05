//
//  SaveProxy.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/3.
//

#include "SaveProxy.hpp"
#include "ResProxy.hpp"
#include "StoryScriptProxy.hpp"
#include "MediaProxy.hpp"
#include "KvProxy.hpp"
#include "bftools/simpleapi.hpp"
#include "bftools/archives.hpp"
#include "app/App.hpp"
#include "mediator/StageMediator.hpp"

NS_BF_BEGIN

#define GAME_FLAG 0x1010EEAA
#define SAVE_VERSION 101

typedef struct {
    int game_flag;
    int save_ver;
    int play_time;
    int save_time;
    unsigned char title[80];
} GameSaveHeader;

typedef struct {
    int size;
    unsigned char* source;
} GameDataBin;

class GameSaveData {
public:
    typedef std::shared_ptr<GameSaveData> GameSaveDataPtr;
public:
    static GameSaveDataPtr New() {
        GameSaveDataPtr ptr(new GameSaveData);
        return ptr;
    }
public:
    GameSaveData();
    ~GameSaveData();
public:
    bool WriteToFile(std::string const& fullName);
    bool ReadFile(std::string const& fullName);
public:
    bool checkVersionValid() const;
public:
    GameSaveHeader& getHeader();
    GameSaveHeader const& getHeader() const;
public:
    void setGameDataBin(unsigned char* buffer, int byteSize);
    GameDataBin& getGameDataBin();
private:
    void freeGameDataBin();
    void makeGameDataBin(int byteSize);
private:
    GameSaveHeader mHeader;
    GameDataBin mGameData;
};

GameSaveData::GameSaveData() {
    memset(&mHeader, 0, sizeof(mHeader));
    memset(&mGameData, 0, sizeof(mGameData));
}

GameSaveData::~GameSaveData() {
    this->freeGameDataBin();
}

bool GameSaveData::checkVersionValid() const {
    return mHeader.save_ver == SAVE_VERSION;
}

void GameSaveData::freeGameDataBin() {
    if (mGameData.source != nullptr) {
        free(mGameData.source);
    }
    mGameData.source = nullptr;
    mGameData.size = 0;
}

void GameSaveData::makeGameDataBin(int byteSize) {
    freeGameDataBin();
    mGameData.size = byteSize;
    mGameData.source = (unsigned char*)malloc(byteSize);
    memset(mGameData.source, 0, mGameData.size);
}

bool GameSaveData::WriteToFile(std::string const& fullName) {
    FILE* fp = fopen(fullName.c_str(), "wb");
    if (fp == nullptr) {
        return false;
    }
    
    mHeader.game_flag = GAME_FLAG;
    mHeader.save_ver = SAVE_VERSION;
    fwrite(&mHeader, sizeof(mHeader), 1, fp);
    
    // 保存游戏数据
    if (mGameData.size > 0) {
        fwrite(&mGameData.size, sizeof(mGameData.size), 1, fp);
        fwrite(mGameData.source, mGameData.size, 1, fp);
    }

    fclose(fp);
    return true;
}

bool GameSaveData::ReadFile(std::string const& fullName) {
    FILE* fp = fopen(fullName.c_str(), "rb");
    if (fp == nullptr) {
        return false;
    }
    
    fread(&mHeader, sizeof(mHeader), 1, fp);
    if (mHeader.game_flag != GAME_FLAG) {
        fclose(fp);
        return false;
    }
    /*if (mHeader.save_ver != SAVE_VERSION) {
        fclose(fp);
        return false;
    }*/
    
    // 读取游戏数据
    fread(&mGameData.size, sizeof(mGameData.size), 1, fp);
    if (mGameData.size > 0) {
        this->makeGameDataBin(mGameData.size);
        fread(mGameData.source, mGameData.size, 1, fp);
    }

    fclose(fp);
    return true;
}

GameSaveHeader& GameSaveData::getHeader() {
    return this->mHeader;
}

GameSaveHeader const& GameSaveData::getHeader() const {
    return this->mHeader;
}

void GameSaveData::setGameDataBin(unsigned char* buffer, int byteSize) {
    if (buffer == nullptr || byteSize <= 0) {
        return;
    }
    this->makeGameDataBin(byteSize);
    memcpy(mGameData.source, buffer, byteSize);
}

GameDataBin& GameSaveData::getGameDataBin() {
    return this->mGameData;
}

// =================================================================================================

std::string const SaveProxy::Name = "SaveProxy";
std::string const ScreenshotName = "screenshot.png";
std::string const SavedataName = "savedata.bin";

SaveProxy::SaveProxy():PureMVC::Proxy(Name), mCurNumber(0), mChapter("EMPTY") {
    
}

bool SaveProxy::SaveGame(int number, CompletedFunc func) {
    auto savePath = this->getSavePath(number);
    auto screenshot = this->getScreenShotSavePath(number)+ScreenshotName;
    auto savedata = savePath+SavedataName;
    auto gamedata = GameSaveData::New();
    auto file = cocos2d::FileUtils::getInstance();
    auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    
    file->createDirectory(savePath);
    gamedata->getHeader().play_time = script->getPlayTime();
    gamedata->getHeader().save_time = (int)getTimeStamp();
    sprintf((char*)gamedata->getHeader().title, "%s", mChapter.c_str());
    
    this->onSaveData(&(*gamedata));
    
    if (!gamedata->WriteToFile(savedata)) {
        if (func != nullptr) {
            func(number, "存档保存失败!");
        }
        return false;
    }

    this->mCurNumber = number;
    this->mSaveCompleted = func;
    auto mediator = App::getMediator<StageMediator>(StageMediator::Name);
    mediator->getGameScreenShot(screenshot, [&](std::string const& pngName) {
        if (this->mSaveCompleted != nullptr) {
            this->mSaveCompleted(this->mCurNumber, "");
        }
    });
    
    // 保存公共键值对
    auto kv = App::getProxy<KvProxy>(KvProxy::Common);
    kv->writeToFile(KvProxy::PublicKvPath);
    
    return true;
}

bool SaveProxy::LoadGame(int number, CompletedFunc func) {
    auto savePath = this->getSavePath(number);
    auto savedata = savePath+SavedataName;
    auto file = cocos2d::FileUtils::getInstance();
    auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    
    file->createDirectory(savePath);
    if (!file->isFileExist(savedata)) {
        // 判断存档是否存在
        return false;
    }
    
    auto gamedata = GameSaveData::New();
    if (!gamedata->ReadFile(savedata)) {
        if (func != nullptr) {
            func(number, "该存档不是有效的格式!");
        }
        return false;
    }
    if (!gamedata->checkVersionValid()) {
        if (func != nullptr) {
            func(number, "该存档不被当前版本的引擎支持!");
        }
        return false;
    }
    
    script->setPlayTime(gamedata->getHeader().play_time);
    this->mChapter = (char*)gamedata->getHeader().title;
    this->onLoadData(&(*gamedata));
    
    if (func != nullptr) {
        func(number, "");
    }
    
    return true;
}

void SaveProxy::setChapter(std::string const& title) {
    this->mChapter = title;
}

std::string const& SaveProxy::getChapter() const {
    return this->mChapter;
}

SaveProxy::SaveInfoPtr SaveProxy::loadGameSaveInfo(int number) {
    auto savePath = this->getSavePath(number);
    auto screenshot = savePath+ScreenshotName;
    auto savedata = savePath+SavedataName;
    auto file = cocos2d::FileUtils::getInstance();
    
    file->createDirectory(savePath);
    if (!file->isFileExist(screenshot)) {
        // 判断截图是否存在
        return nullptr;
    }
    if (!file->isFileExist(savedata)) {
        // 判断存档是否存在
        return nullptr;
    }
    
    auto gamedata = GameSaveData::New();
    if (!gamedata->ReadFile(savedata)) {
        return nullptr;
    }
    
    SaveInfoPtr ptr(new SaveInfo);
    ptr->icon = screenshot;
    ptr->title.append((char*)gamedata->getHeader().title);
    ptr->play_time = gamedata->getHeader().play_time;
    ptr->save_time = gamedata->getHeader().save_time;
    return ptr;
}

std::string SaveProxy::getSavePath(int number) {
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    char temp[64] = {0};
    sprintf(temp, "%02d/", number+1);
    return res->getSavePath() + temp;
}

std::string SaveProxy::getScreenShotSavePath(int number) {
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    char temp[64] = {0};
    sprintf(temp, "%02d/", number+1);
    return res->getSaveFolderName() + temp;
}

void SaveProxy::onRegister() {
    
}

void SaveProxy::onRemove() {
    
}

void SaveProxy::onSaveData(GameSaveData* data) {
    // 把画面备份到data
    auto screen = App::getMediator<StageMediator>(StageMediator::Name);
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
    auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    
    // 保存数据到流
    Archives arch;
    arch.Set([&](Archives::OutputArchive& ar) {
        ar(*kv);
        ar(*media);
        ar(*script);
        ar(*screen);
    });
    auto str = arch.getStream().str();
    if (str.length() == 0) {
        return;
    }
    data->setGameDataBin((unsigned char*)str.c_str(), (int)str.length());
    
    // 加密游戏数据
    auto databin = data->getGameDataBin();
    res->Encrypt(databin.source, databin.size);
}

void SaveProxy::onLoadData(GameSaveData* data) {
    // 从data还原画面
    auto screen = App::getMediator<StageMediator>(StageMediator::Name);
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto media = App::getProxy<MediaProxy>(MediaProxy::Name);
    auto script = App::getProxy<StoryScriptProxy>(StoryScriptProxy::Name);
    auto kv = App::getProxy<KvProxy>(KvProxy::Name);
    
    // 清除声音
    media->stopMusic();
    media->stopAllEffects();
    
    // 清除画面
    while (screen->getSpriteStackSize() != 0) {
        screen->popSpriteStack();
    }
    
    // 清除脚本
    script->clear();
    
    auto databin = data->getGameDataBin();
    if (databin.size == 0) {
        return;
    }
    // 解密游戏数据
    res->Decrypt(databin.source, databin.size);
    
    std::string str((char*)databin.source, databin.size);
    // 从流读取数据
    Archives arch;
    arch.setStream(str);
    arch.Get([&](Archives::InputArchive& ar) {
        ar(*kv);
        ar(*media);
        ar(*script);
        ar(*screen);
    });
    
    if (media->getMusicName().length() != 0) {
        media->playMusic(media->getMusicName());
    }
}

NS_BF_END
