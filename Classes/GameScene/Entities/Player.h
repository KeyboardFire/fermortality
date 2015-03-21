#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

#include "../Entity.h"

USING_NS_CC;

class Player: public Entity {
public:
    Player();
    virtual std::string filename() { return "player.png"; };
    virtual Vec3 frameSize() { return Vec3(48, 48, 3); };
    virtual void damage(int amount);
    virtual void update(int aiInfo);
    Vec2 getHandPosition();
    static Player* create();
    int dir, lookDir;
    bool isRunning;
    int invulnTime;
    Sprite *whip;
    Entity *objectHeld;

    static const int RUN_SPEED = 5, RUN_ACCELERATION = 1, JUMP_SPEED = 18;
};

#endif // __PLAYER_H__
