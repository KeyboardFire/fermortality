#ifndef __ROCK_H__
#define __ROCK_H__

#include "cocos2d.h"

#include "../Creature.h"

USING_NS_CC;

class Rock: public Creature {
    virtual std::string filename() { return "rock.png"; };
};

#endif // __ROCK_H__
