//
//  SaveProxy.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/3.
//

#ifndef SaveProxy_hpp
#define SaveProxy_hpp

#include <stdio.h>
#include "app/macros.h"
#include "puremvc/pmvcpp.h"

NS_BF_BEGIN

class GameSaveData;
class SaveProxy : public PureMVC::Proxy {
public:
    class SaveInfo {
    public:
        std::string icon;
        std::string title;
        int play_time;
        int save_time;
    };
    typedef std::shared_ptr<SaveInfo> SaveInfoPtr;
    typedef std::function<void(int number, std::string const& error)> CompletedFunc;
public:
    static std::string const Name;
public:
    SaveProxy();
public:
    bool SaveGame(int number, CompletedFunc func);
    bool LoadGame(int number, CompletedFunc func);
public:
    void setChapter(std::string const& title);
    std::string const& getChapter() const;
public:
    SaveInfoPtr loadGameSaveInfo(int number);
private:
    std::string getSavePath(int number);
    std::string getScreenShotSavePath(int number);
private:
    void onSaveData(GameSaveData* data);
    void onLoadData(GameSaveData* data);
private:
    virtual void onRegister();
    virtual void onRemove();
private:
    int mCurNumber;
    std::string mChapter;
    CompletedFunc mSaveCompleted;
};

NS_BF_END

#endif /* SaveProxy_hpp */
