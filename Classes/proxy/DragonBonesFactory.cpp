//
//  DragonBonesFactory.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/18.
//

#include "DragonBonesFactory.hpp"
#include "view/DragonBonesView.hpp"
#include "app/App.hpp"
#include "proxy/ResProxy.hpp"

NS_BF_BEGIN

DragonBonesFactory* DragonBonesFactory::_factory = nullptr;

DragonBonesFactory* DragonBonesFactory::getInstance() {
    if (_factory == nullptr) {
        _factory = new DragonBonesFactory();
    }
    return _factory;
}

DragonBonesFactory::DragonBonesFactory() {
    mKeys.reserve(10);
}

void DragonBonesFactory::loadDragonBones(std::string const& name) {
    std::string ske = name + "_ske.json";
    std::string tex = name + "_tex.json";
    std::string png = name + "_tex.png";
    if (getDragonBonesData(ske) != NULL) {
        return;
    }
    auto res = App::getProxy<ResProxy>(ResProxy::Name);
    auto skeData = res->getData(ske);
    auto texData = res->getData(tex);
    auto pngTex = res->getTexture(png);
    std::string skeStr((char*)skeData->getBytes(), skeData->getSize());
    parseDragonBonesData(skeStr.c_str(), ske);
    std::string texStr((char*)texData->getBytes(), texData->getSize());
    parseTextureAtlasData(texStr.c_str(), (void*)pngTex, ske);
    mKeys.push_back(name);
}

void DragonBonesFactory::removeDragonBones(std::string const& name) {
    CacheList::const_iterator iter = std::find(mKeys.begin(), mKeys.end(), name);
    if (iter == mKeys.end()) {
        return;
    }
    std::string ske = name + "_ske.json";
    removeDragonBonesData(ske);
    removeTextureAtlasData(ske);
    mKeys.erase(iter);
}

void DragonBonesFactory::removeAllDragonBones() {
    CacheList list = mKeys;
    for (int i = 0; i < list.size(); i++) {
        removeDragonBones(list[i]);
    }
}

DragonBonesView* DragonBonesFactory::makeView(std::string const& bonesName, std::string const& armatureName) const {
    std::string dragonBonesName = bonesName + "_ske.json";
    //std::string textureAtlasName = bonesName + "_tex.json";
    std::string skinName;
    const auto armature = buildArmature(armatureName, dragonBonesName, skinName);
    if (armature != nullptr) {
        _dragonBones->getClock()->add(armature);
        return static_cast<DragonBonesView*>(armature->getDisplay());
    }
    return nullptr;
}

dragonBones::Armature* DragonBonesFactory::_buildArmature(const dragonBones::BuildArmaturePackage& dataPackage) const {
    const auto armature = dragonBones::BaseObject::borrowObject<dragonBones::Armature>();
    const auto armatureDisplay = DragonBonesView::create();
    armatureDisplay->retain();
    armatureDisplay->setCascadeOpacityEnabled(true);
    armatureDisplay->setCascadeColorEnabled(true);
    armature->init(dataPackage.armature, armatureDisplay, armatureDisplay, _dragonBones);
    return armature;
}

NS_BF_END
