#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

#include "../Creature.h"

USING_NS_CC;

class Player: public Creature {
public:
    Player();
    virtual std::string filename() { return "player.png"; };
    virtual Vec3 frameSize() { return Vec3(48, 48, 3); };
    virtual void damage(int amount);
    virtual void update(int aiInfo);
    Vec2 getHandPosition();
    static Player* create();
    int dir, lookDir;
    int invulnTime;
    Sprite *whip;
    Creature *objectHeld;
};

#endif // __PLAYER_H__
