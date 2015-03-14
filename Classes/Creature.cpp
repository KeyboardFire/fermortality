#include "Creature.h"
#include "Enemies/Enemies.h"

USING_NS_CC;

Creature* Creature::create(std::string type) {
    Creature *c;
    if (type == "player") c = new Player();
    else if (type == "slime") c = new Slime();

    if (c && c->initWithFile(c->filename())) {
        c->autorelease();

        c->velocity = new Vec2(0, 0); // TODO we have to delete this somewhere

        return c;
    }

    CC_SAFE_DELETE(c);
    return NULL;
}
