#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

#include "../Creature.h"

USING_NS_CC;

class Player: public Creature {
public:
    Player();
    virtual std::string filename() { return "player.png"; };
    virtual void damage(int amount);
    virtual void update(int aiInfo);
    static Player* create();
    int dir;
    int invulnTime;
};

#endif // __PLAYER_H__
