#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "../cocos2d/cocos/cocos2d.h"

#include <string>

USING_NS_CC;

class Creature: public cocos2d::Sprite {
public:
    virtual std::string filename() { return "must be overridden"; };
    virtual void update(int aiInfo) { };
    virtual bool collidedWithPlayer(char dir, Creature *player) {};
    virtual void damage(int amount) { health -= amount; };
    static Creature* create(std::string type);

    // this has to be implemented in the header because C++ is idiotic
    template <typename T> static T* setupCreature(T *c) {
        if (c && c->initWithFile(c->filename())) {
            c->autorelease();

            c->velocity = new Vec2(0, 0); // TODO we have to delete this somewhere
            c->health = 1;

            return c;
        }

        CC_SAFE_DELETE(c);
        return NULL;
    };

    Vec2 *velocity;
    int health;

    enum AIInfo { cliffLeft = 1, cliffRight = 2 /*, foo = 4, ...*/  };
};

#endif // __CREATURE_H__
