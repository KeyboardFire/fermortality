#ifndef __SCENE_DISPATCHER_H__
#define __SCENE_DISPATCHER_H__

#include "cocos2d.h"

USING_NS_CC;

class SceneDispatcher {
public:
    static void gameScene(bool replace);
    static void mainMenuScene(bool replace);
    static void dispatch(Scene *s, bool replace);
};

#endif // __SCENE_DISPATCHER_H__
