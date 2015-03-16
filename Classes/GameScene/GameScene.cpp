#include "GameScene.h"
#include "../SceneDispatcher.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool GameScene::init() {
    // super init first
    if (!Scene::init()) return false;

    gameLayer = GameLayer::create();
    hudLayer = HUDLayer::create();
    gameLayer->scene = this;
    addChild(gameLayer);
    addChild(hudLayer);

    return true;
}

void GameScene::updateHearts(int health) {
    hudLayer->updateHearts(health);
    if (health <= 0) {
        SceneDispatcher::mainMenuScene(true);
    }
}
