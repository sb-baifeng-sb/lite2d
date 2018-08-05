//
//  ImageView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/19.
//

#include "BFImageView.hpp"
#include "proxy/ResProxy.hpp"
#include "app/App.hpp"

NS_BF_BEGIN

ImageSpriteState::ImageSpriteState() {
    
}

void ImageSpriteState::Set(BF::AvgSprite *sprite) {
	auto view = sprite->type<BFImageView>();
    if (view != nullptr) {
        view->loadImageFromResProxy(this->mFilename);
    }
    SimpleSpriteState::Set(sprite);
}

void ImageSpriteState::Get(BF::AvgSprite *sprite) {
    SimpleSpriteState::Get(sprite);
	auto view = sprite->type<BFImageView>();
    if (view != nullptr) {
        this->mFilename = view->mFilename;
    }
}

std::string const& ImageSpriteState::getFileName() const {
    return this->mFilename;
}

BFImageView::BFImageView() :BF::AvgSprite(TYPE_IMG, this) {
}

void BFImageView::loadImageFromResProxy(std::string const& filename, ResProxy* res) {
    if (filename.length() == 0) {
        return;
    }
    if (res == nullptr) {
        res = App::getProxy<ResProxy>(ResProxy::Name);
    }
    auto texture = res->getTexture(filename);
    this->initWithTexture(texture);
    this->mFilename = filename;
}

BFImageView::SpriteStatePtr BFImageView::getSpriteState() {
    SpriteStatePtr state(new ImageSpriteState);
    state->Get(this);
    return state;
}

NS_BF_END
