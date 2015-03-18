#include "Creature.h"
#include "Enemies/Enemies.h"

USING_NS_CC;

Creature* Creature::create(std::string type) {
    Creature *c;
    if (type == "player") c = new Player();
    else if (type == "slime") c = new Slime();
    else if (type == "rock") c = new Rock();

    return Creature::setupCreature<Creature>(c);
}

SpriteFrame* Creature::getFrame(int i) {
    return SpriteFrameCache::getInstance()->getSpriteFrameByName(filename() + std::to_string(i));
}
