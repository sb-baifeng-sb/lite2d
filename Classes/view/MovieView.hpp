//
//  MovieView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/5/19.
//

#ifndef MovieView_hpp
#define MovieView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "ui/UIVideoPlayer.h"

NS_BF_BEGIN

#ifdef WIN32
class VideoPlayer : public cocos2d::Node {
public:
	enum class EventType {
		PLAYING = 0,
		PAUSED,
		STOPPED,
		COMPLETED
	};
	typedef std::function<void(cocos2d::Ref* sender, VideoPlayer::EventType eventType)> CompletedFunc;
public:
	VideoPlayer();
public:
	void setFileName(std::string const& fileName);
	void addEventListener(CompletedFunc func);
	void play();
private:
	CompletedFunc mFunc;
};
#else
typedef cocos2d::experimental::ui::VideoPlayer VideoPlayer;
#endif

class MovieView : public VideoPlayer {
public:
    static void PlayMovie(std::string const& fileName);
public:
    CREATE_FUNC(MovieView);
    MovieView();
private:
    void play(std::string const& fileName);
    void onEvent(cocos2d::Ref* sender, VideoPlayer::EventType eventType);
private:
    int mCount;
};

NS_BF_END

#endif /* MovieView_hpp */
