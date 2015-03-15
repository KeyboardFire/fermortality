#include "GameScene.h"
#include "GameLayer.h"
#include "HUDLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool GameScene::init() {
    // super init first
    if (!Scene::init()) return false;

    GameLayer *gameLayer = GameLayer::create();
    HUDLayer *hudLayer = HUDLayer::create();
    gameLayer->hudLayer = hudLayer;
    addChild(gameLayer);
    addChild(hudLayer);

    return true;
}
