#include "MainMenuScene.h"
#include "../SceneDispatcher.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool MainMenuScene::init() {
    // super init first
    if (!Scene::init()) return false;

    auto mainMenuLayer = MainMenuLayer::create();
    addChild(mainMenuLayer);

    return true;
}

void MainMenuScene::keyPressed() {
    SceneDispatcher::gameScene(true);
}
