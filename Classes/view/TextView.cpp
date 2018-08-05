//
//  TextView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/30.
//

#include "TextView.hpp"

NS_BF_BEGIN

TextSpriteState::TextSpriteState() {
    
}

void TextSpriteState::Set(BF::AvgSprite *sprite) {
    auto view = sprite->type<TextView>();
    if (view != nullptr) {
        view->setAlignment(this->mAlign);
        view->setWidth(this->mWidth);
        view->setString(this->mTitle);
    }
    SimpleSpriteState::Set(sprite);
}

void TextSpriteState::Get(BF::AvgSprite *sprite) {
    SimpleSpriteState::Get(sprite);
    auto view = sprite->type<TextView>();
    if (view != nullptr) {
        this->mWidth = view->getWidth();
        this->mAlign = view->getTextAlignment();
        this->mTitle = view->getString();
    }
}

TextView::TextView():BF::AvgSprite(TYPE_LABEL, this) {
    this->setBMFontFilePath("font/sets.fnt");
}

TextView::SpriteStatePtr TextView::getSpriteState() {
    SpriteStatePtr state(new TextSpriteState());
    state->Get(this);
    return state;
}

NS_BF_END
