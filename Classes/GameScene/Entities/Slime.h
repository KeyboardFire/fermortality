#ifndef __SLIME_H__
#define __SLIME_H__

#include "cocos2d.h"

#include "../Entity.h"

USING_NS_CC;

class Slime: public Entity {
    virtual std::string filename() { return "slime.png"; };
    virtual void collidedWithPlayer(char dir, Entity *player);
    virtual void collidedWithWhip(Sprite *whip);
    virtual void update(int aiInfo);
};

#endif // __SLIME_H__
