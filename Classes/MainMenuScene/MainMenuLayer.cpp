#include "MainMenuLayer.h"
#include "MainMenuScene.h"

#include <iostream>

USING_NS_CC;

bool MainMenuLayer::init() {
    // super init first
    if (!Layer::init()) return false;

    auto img = Sprite::create("mainmenu.png");
    auto screenWidth = Director::getInstance()->getOpenGLView()->getFrameSize().width;
    auto screenHeight = Director::getInstance()->getOpenGLView()->getFrameSize().height;
    img->setPosition(Vec2(screenWidth/2, screenHeight/2));
    addChild(img);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
        MainMenuScene::keyPressed();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
