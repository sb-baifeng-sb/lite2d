//
//  StoryScriptProxy.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/2.
//

#ifndef StoryScriptProxy_hpp
#define StoryScriptProxy_hpp

#include <stdio.h>
#include "bftools/archives.hpp"
#include "puremvc/pmvcpp.h"
#include "cocos2d.h"

NS_BF_BEGIN

class StoryScriptNote {
public:
    ~StoryScriptNote() {}
public:
    template<class Archive>
    void serialize(Archive & archive) {
        archive(scriptIndex, curTime, endTime);
        archive(fileName);
    }
public:
    int scriptIndex, curTime, endTime;
    std::string fileName;
};

class JSEngine;
class storyScript;
class SpeakDelegateHolder;

class StoryScriptProxy : public PureMVC::Proxy {
public:
    typedef enum {
        MODE_NORMAL = 0,
        MODE_AUTO = 1,
        MODE_SKIP = 2,
    } ScriptMode;
    typedef std::shared_ptr<JSEngine> JSEnginePtr;
    typedef std::shared_ptr<storyScript> StoryScriptPtr;
    typedef std::vector<StoryScriptPtr> ScriptStack;
    typedef std::shared_ptr<SpeakDelegateHolder> SpeakDelegateHolderPtr;
    typedef std::shared_ptr<StoryScriptNote> StoryScriptNotePtr;
    typedef std::function<void(std::string const&)> StoryScriptPopCallBack;
public:
    static std::string const Name;
public:
    StoryScriptProxy();
public:
    void setScriptPopCallBack(StoryScriptPopCallBack func);
public:
    void update(float dt);
    void setMode(ScriptMode mode);
    ScriptMode getMode() const;
    void setPlayTime(int seconds);
    int getPlayTime() const;
public:
    void popScript();
    void runScript(std::string const& filename);
    void jumpScript(std::string const& filename);
    void doScript(std::string const& filename);
    void doCode(std::string const& code);
public:
    bool isEmpty() const;
    int getStackSize() const;
    void step(float dt); // do one line script
    void seek(std::string const& tag); // seek index with tag
    void pause(float seconds);
    void back_step(int step);
    void clear();
public:
    void startUpdate();
    void stopUpdate();
public:
    JSEnginePtr getJSEngine() const;
    SpeakDelegateHolderPtr getSpeakHolder() const;
public:
    template<class Archive>
    void save(Archive & archive) const {
        int scriptSize = (int)mStack.size();
        archive(scriptSize);
        
        for (int i=0; i < scriptSize; i++) {
            auto note = this->makeScriptNote(mStack[i]);
            archive(*note);
        }
    }
    template<class Archive>
    void load(Archive & archive) {
        int scriptSize = 0;
        archive(scriptSize);
        
        this->clear();
        for (int i=0; i < scriptSize; i++) {
            StoryScriptNotePtr note(new StoryScriptNote);
            archive(*note);
            this->loadScriptNode(note);
        }
    }
private:
    StoryScriptNotePtr makeScriptNote(StoryScriptPtr const& p) const;
    void loadScriptNode(StoryScriptNotePtr const& p);
private:
    virtual void onRegister();
    virtual void onRemove();
    void onDoScript(std::string const& line);
    void onUpdate(float dt);
private:
    bool mUpdate;
    ScriptMode mMode;
    float mPlayTime;
    JSEnginePtr mEngine;
    ScriptStack mStack;
    SpeakDelegateHolderPtr mSpeakHolder;
    StoryScriptPopCallBack mScriptPopCallBack;
};

class SpeakDelegate {
public:
    typedef StoryScriptProxy::ScriptMode SpeakMode;
public:
    ~SpeakDelegate() {}
public:
    virtual bool isSpeakFinish() = 0;
    virtual void onSpeakClick() = 0;
    virtual void setSpeakMode(SpeakMode mode) = 0;
};

class SpeakDelegateHolder {
public:
    typedef StoryScriptProxy::ScriptMode Mode;
    typedef cocos2d::Ref Delegate;
    typedef SpeakDelegate RealDelegate;
    typedef std::vector<Delegate*> DelegateList;
public:
    SpeakDelegateHolder();
    ~SpeakDelegateHolder();
public:
    void setMode(Mode mode);
    bool isSpeakFinish() const;
    void ClickSpeak();
public:
    bool addDelegate(Delegate* delegate);
    bool removeDelegate(Delegate* delegate);
    void removeAllDelegate();
private:
    void travel(std::function<void(RealDelegate* delegate)> func);
private:
    Mode mMode;
    DelegateList mList;
};

NS_BF_END

#endif /* StoryScriptProxy_hpp */
