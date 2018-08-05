//
//  DragonBonesFactory.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/4/18.
//

#ifndef DragonBonesFactory_hpp
#define DragonBonesFactory_hpp

#include <stdio.h>
#include "app/macros.h"
#include "dragonBones/cocos2dx/CCFactory.h"

NS_BF_BEGIN

class DragonBonesView;
class DragonBonesFactory : public dragonBones::CCFactory {
public:
    typedef std::vector<std::string> CacheList;
public:
    static DragonBonesFactory* getInstance();
    void loadDragonBones(std::string const& name);
    void removeDragonBones(std::string const& name);
    void removeAllDragonBones();
    DragonBonesView* makeView(std::string const& dragonBonesName, std::string const& armatureName) const;
protected:
    virtual dragonBones::Armature* _buildArmature(const dragonBones::BuildArmaturePackage& dataPackage) const override;
private:
    DragonBonesFactory();
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(DragonBonesFactory);
    static DragonBonesFactory* _factory;
private:
    CacheList mKeys;
};

NS_BF_END

#endif /* DragonBonesFactory_hpp */
