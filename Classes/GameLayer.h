#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "../cocos2d/cocos/cocos2d.h"
#include "Creature.h"
#include "Enemies/Enemies.h"

#include <vector>

USING_NS_CC;

class HelloWorld: public cocos2d::Layer {
public:
    // auto-generated "static create()" method
    CREATE_FUNC(HelloWorld);

    virtual bool init();

    int screenWidth, screenHeight;

    Player* player;
    const int RUN_SPEED = 5, RUN_ACCELERATION = 1, GRAVITY = 1, JUMP_SPEED = 18;
    const float RUN_FRICTION = 0.9;
    std::vector<Creature*> enemies;
    std::vector<Sprite*> tiles;

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    virtual void update(float dt);
    void updateCreature(Creature *s);
    char collide(Sprite *s1, Sprite *s2);

    bool cheat;
};

#endif // __HELLOWORLD_SCENE_H__
