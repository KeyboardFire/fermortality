#ifndef __ROCK_H__
#define __ROCK_H__

#include "cocos2d.h"

#include "../Entity.h"

USING_NS_CC;

class Rock: public Entity {
    virtual std::string filename() { return "rock.png"; };
};

#endif // __ROCK_H__
