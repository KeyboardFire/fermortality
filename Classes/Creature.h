#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "../cocos2d/cocos/cocos2d.h"

#include <string>

USING_NS_CC;

class Creature: public cocos2d::Sprite {
public:
    virtual std::string filename() { return "must be overridden"; };
    static Creature* create(std::string type);

    Vec2 *velocity;
};

#endif // __CREATURE_H__
