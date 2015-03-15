#ifndef __SLIME_H__
#define __SLIME_H__

#include "../../cocos2d/cocos/cocos2d.h"

#include "../Creature.h"

USING_NS_CC;

class Slime: public Creature {
    virtual std::string filename() { return "sprites/Enemies/slimeWalk1.png"; };
    virtual bool collidedWithPlayer(char dir, Creature *player);
    virtual void update(int aiInfo);
};

#endif // __SLIME_H__
