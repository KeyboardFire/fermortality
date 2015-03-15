#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "../../cocos2d/cocos/cocos2d.h"

#include "../Creature.h"

USING_NS_CC;

class Player: public Creature {
public:
    Player();
    virtual std::string filename() { return "sprites/Player/p1_stand.png"; };
    virtual void damage(int amount);
    virtual void update();
    static Player* create();
    int dir;
    int invulnTime;
};

#endif // __PLAYER_H__
