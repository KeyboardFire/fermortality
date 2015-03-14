#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "../../cocos2d/cocos/cocos2d.h"

#include "../Creature.h"

USING_NS_CC;

class Player: public Creature {
public:
    virtual std::string filename() { return "sprites/Player/p1_stand.png"; };
    int dir;
};

#endif // __PLAYER_H__
