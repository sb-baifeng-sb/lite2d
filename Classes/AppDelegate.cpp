#include "AppDelegate.h"
#include "app/App.hpp"
#include "app/Register.hpp"
#include "app/notify.hpp"

// #define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

#include "audio/include/SimpleAudioEngine.h"
#include "bftools/archives.hpp"
#include "proxy/ResProxy.hpp"
#include "proxy/StoryScriptProxy.hpp"
#include "proxy/ConfigProxy.hpp"

#include "view/CcbUiView.hpp"
#include "view/UI/GameDataMgrView.hpp"
#include "view/UI/CGLibraryView.hpp"
#include "view/UI/MusicLibraryView.hpp"
#include "view/UI/MovieLibraryView.hpp"
#include "view/UI/GameOptionView.hpp"
#include "view/UI/BacklogView.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "tea/TEA_Encrypt.h"
#ifdef __cplusplus
}
#endif

USING_NS_CC;
//USING_NS_BF;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLViewImpl::create("Lite2D");
		director->setOpenGLView(glview);
    }
    
    std::string const writeablePath = FileUtils::getInstance()->getWritablePath();
    std::vector<std::string> paths;
    paths.push_back(writeablePath);
    paths.push_back("ui");
    paths.push_back("media");
    FileUtils::getInstance()->setSearchPaths(paths);

    BF::App::addRegisterFunc(std::bind(&BF::registerSystem, std::placeholders::_1));
    BF::App::addRemoveFunc(std::bind(&BF::removeSystem, std::placeholders::_1));
    BF::App::fire();
    
    //glview->setDesignResolutionSize(800,480,ResolutionPolicy::EXACT_FIT);
    auto config = BF::App::getProxy<BF::ConfigProxy>(BF::ConfigProxy::Name);
    auto engine = config->getSession("engine");
    auto width = engine->getValueAsInt("screen_width");
    auto height = engine->getValueAsInt("screen_height");
    CCASSERT(width != 0 && height != 0, "屏幕分辨率配置无效！");
    
    glview->setDesignResolutionSize(width, height, ResolutionPolicy::SHOW_ALL);
#ifdef WIN32
    glview->setFrameSize(width, height);
#endif
    
    // turn on display FPS
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60.0f);
    
    BF::App::notify(BF::Event::SystemFire);
    
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
