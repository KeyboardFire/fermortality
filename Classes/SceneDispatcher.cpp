#include "SceneDispatcher.h"

#include "GameScene/GameScene.h"
#include "MainMenuScene/MainMenuScene.h"

void SceneDispatcher::gameScene(bool replace) {
    Scene *s = GameScene::create();
    dispatch(s, replace);
}

void SceneDispatcher::mainMenuScene(bool replace) {
    Scene *s = MainMenuScene::create();
    dispatch(s, replace);
}

void SceneDispatcher::dispatch(Scene *s, bool replace) {
    if (replace) {
        Director::getInstance()->replaceScene(s);
    } else {
        Director::getInstance()->pushScene(s);
    }
}
