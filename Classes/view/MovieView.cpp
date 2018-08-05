//
//  MovieView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/19.
//

#include "MovieView.hpp"
#include "cocos2d.h"
#include "app/App.hpp"
#include "app/notify.hpp"

NS_BF_BEGIN

#ifdef WIN32

VideoPlayer::VideoPlayer() {

}

void VideoPlayer::setFileName(std::string const& fileName) {

}

void VideoPlayer::addEventListener(CompletedFunc func) {
	mFunc = func;
}

void VideoPlayer::play() {
	if (mFunc != nullptr) {
		mFunc(this, EventType::COMPLETED);
	}
}

#endif

MovieView::MovieView():mCount(0) {
    
}

void MovieView::PlayMovie(std::string const& fileName) {
    auto director = cocos2d::Director::getInstance();
    auto scene = cocos2d::Scene::create();
    auto player = create();
    player->play(fileName);
    scene->addChild(player);
    director->pushScene(scene);
    
    App::notify(Event::PauseStoryScript);
    App::notify(Event::PauseBackGroundMusic);
}

void MovieView::play(std::string const& fileName) {
    auto director = cocos2d::Director::getInstance();
    this->setPosition(director->getWinSize()/2.0f);
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    this->setContentSize(director->getWinSize());
    this->setFileName(fileName);
    this->addEventListener(std::bind(&MovieView::onEvent, this, std::placeholders::_1, std::placeholders::_2));
    this->VideoPlayer::play();
}

void MovieView::onEvent(cocos2d::Ref* sender, VideoPlayer::EventType eventType) {
    switch (eventType) {
        case VideoPlayer::EventType::PLAYING:
            CCLOG("video playing.");
            break;
        case VideoPlayer::EventType::PAUSED:
            CCLOG("video paused.");
            break;
        case VideoPlayer::EventType::STOPPED:
            CCLOG("video stopped.");
            break;
        case VideoPlayer::EventType::COMPLETED:
            CCLOG("video finish.");
            this->mCount ++;
            if (this->mCount == 1) {
                auto director = cocos2d::Director::getInstance();
                director->popScene();
                this->mCount = 0;
                
                App::notify(Event::ResumeStoryScript);
                App::notify(Event::ResumeBackGroundMusic);
            }
            break;
        default:
            break;
    }
}

NS_BF_END
