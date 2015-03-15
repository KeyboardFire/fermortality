#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

// forward declarations
class GameLayer;
class HUDLayer;

#include "cocos2d.h"
#include "GameLayer.h"
#include "HUDLayer.h"

USING_NS_CC;

class GameScene: public cocos2d::Scene {
public:
    // auto-generated "static create()" method
    CREATE_FUNC(GameScene);

    GameLayer *gameLayer;
    HUDLayer *hudLayer;

    void updateHearts(int health);

    virtual bool init();
};

#endif // __GAME_SCENE_H__
