#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

#include "MainMenuLayer.h"

USING_NS_CC;

class MainMenuScene: public cocos2d::Scene {
public:
    // auto-generated "static create()" method
    CREATE_FUNC(MainMenuScene);

    virtual bool init();

    static void keyPressed();
};

#endif // __MAIN_MENU_SCENE_H__
