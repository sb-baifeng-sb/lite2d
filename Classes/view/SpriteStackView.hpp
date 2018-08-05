//
//  SpriteStackView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/14.
//

#ifndef SpriteStackView_hpp
#define SpriteStackView_hpp

#include <stdio.h>
#include "app/macros.h"
#include "Sprite.hpp"
#include "BFImageView.hpp"
#include "ButtonView.hpp"
#include "GifView.hpp"
#include "DragonBonesView.hpp"
#include "TextView.hpp"
#include "SpeakView.hpp"
#include "CcbUiView.hpp"
#include "jsapi.h"

NS_BF_BEGIN

class SpriteStackView : public cocos2d::Layer {
public:
    typedef cocos2d::Node Node;
    typedef BF::AvgSprite Sprite;
    typedef std::vector<Node*> NodeStack;
    typedef std::vector<Sprite*> SpriteStack;
public:
    CREATE_FUNC(SpriteStackView);
    SpriteStackView();
    ~SpriteStackView();
public:
    void setStackSize(int size);
    int getStackSize() const;
public:
    void setSprite(int index, Sprite* sprite);
    Sprite* getSprite(int index) const;
    Node* getSpriteAsNode(int index) const;
    bool isSpriteType(int index, Sprite::Type type) const;
public:
    static BFImageView* makeImageView(std::string const& imgName);
    static ButtonView* makeButtonView(std::string const& normal, std::string const& selected, std::string const& disabled, bool scale9 = false);
    static GifView* makeGifView(std::string const& gifName);
    static DragonBonesView* makeDBView(std::string const& dragonBonesName, std::string const& armatureName);
    static TextView* makeTextView(std::string const& title, float width);
    static HalfSpeakView* makeHalfSpeakView();
    static CcbUiView* makeCcbUiView(std::string const& ccbi, std::string const& className);
public:
    void setSpriteXY(int index, float x, float y);
    void setSpriteAnchor(int index, float x, float y);
    void setSpriteScale(int index, float x, float y);
    void setSpriteSize(int index, int w, int h);
    void showSprite(int index, bool show);
    void movSprite(int index0, int index1);
    void cpySprite(int index0, int index1);
    void swapSprite(int index0, int index1);
public:
    void removeAllSprite();
    BF::AvgSprite* copySprite(BF::AvgSprite* source) const;
    bool loadSprite(int index, std::string const& type, JSContext* cx, uint32_t argc, JS::CallArgs& args);
private:
    void clear();
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
private:
    SpriteStack mSprites;
public:
    template<class Archive>
    void save(Archive & archive) const {
        int size = (int)mSprites.size();
        archive(size);
        
        for (int i=0; i < size; i++) {
            auto sprite = mSprites[i];
            if (sprite == nullptr) {
                archive(Sprite::TYPE_NONE);
                continue;
            }
            
            auto type = sprite->getSpriteType();
            archive(type);
            
            auto ptr = sprite->getSpriteState();
            
            if (type == Sprite::TYPE_IMG) {
                auto state = (&(*ptr))->type<ImageSpriteState>();
                archive(*state);
                continue;
            }
            
            if (type == Sprite::TYPE_BTN) {
                auto state = (&(*ptr))->type<ButtonSpriteState>();
                archive(*state);
                continue;
            }
            
            if (type == Sprite::TYPE_GIF) {
                auto state = (&(*ptr))->type<GifSpriteState>();
                archive(*state);
                continue;
            }
            
            if (type == Sprite::TYPE_DB) {
                auto state = (&(*ptr))->type<DBSpriteState>();
                archive(*state);
                continue;
            }
            
            if (type == Sprite::TYPE_LABEL) {
                auto state = (&(*ptr))->type<TextSpriteState>();
                archive(*state);
                continue;
            }
            
            if (type == Sprite::TYPE_HALF_SPEAK) {
                auto state = (&(*ptr))->type<HalfSpeakViewState>();
                archive(*state);
                continue;
            }
            
            if (type == Sprite::TYPE_CCB_UI) {
                auto state = (&(*ptr))->type<CcbUiViewState>();
                archive(*state);
                continue;
            }
        }
    }
    template<class Archive>
    void load(Archive & archive) {
        int size = 0;
        archive(size);
        
        int type = 0;
        this->setStackSize(size);
        for (int i=0; i < size; i++) {
            
            archive(type);
            if (type == Sprite::TYPE_NONE) {
                continue;
            }
            
            if (type == Sprite::TYPE_IMG) {
                auto state = std::make_shared<ImageSpriteState>();
                archive(*state);
                auto view = this->makeImageView("");
                this->setSprite(i, view);
                state->Set(view);
                continue;
            }
            
            if (type == Sprite::TYPE_BTN) {
                auto state = std::make_shared<ButtonSpriteState>();
                archive(*state);
                auto view = this->makeButtonView("", "", "");
                this->setSprite(i, view);
                state->Set(view);
                continue;
            }
            
            if (type == Sprite::TYPE_GIF) {
                auto state = std::make_shared<GifSpriteState>();
                archive(*state);
                auto view = this->makeGifView("");
                this->setSprite(i, view);
                state->Set(view);
                continue;
            }
            
            if (type == Sprite::TYPE_DB) {
                auto state = std::make_shared<DBSpriteState>();
                archive(*state);
                auto view = this->makeDBView(state->getDragonBonesName(), state->getArmatureName());
                this->setSprite(i, view);
                state->Set(view);
                continue;
            }
            
            if (type == Sprite::TYPE_LABEL) {
                auto state = std::make_shared<TextSpriteState>();
                archive(*state);
                auto view = this->makeTextView("", 0.0f);
                this->setSprite(i, view);
                state->Set(view);
                continue;
            }
            
            if (type == Sprite::TYPE_HALF_SPEAK) {
                auto state = std::make_shared<HalfSpeakViewState>();
                archive(*state);
                auto view = this->makeHalfSpeakView();
                this->setSprite(i, view);
                state->Set(view);
                continue;
            }
            
            if (type == Sprite::TYPE_CCB_UI) {
                auto state = std::make_shared<CcbUiViewState>();
                archive(*state);
                auto view = this->makeCcbUiView(state->getCcbName(), state->getClassName());
                this->setSprite(i, view);
                state->Set(view);
                continue;
            }
        }
    }
};

NS_BF_END

#endif /* SpriteStackView_hpp */
