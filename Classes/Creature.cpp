#include "Creature.h"
#include "Enemies/Enemies.h"

USING_NS_CC;

Creature* Creature::create(std::string type) {
    Creature *c;
    if (type == "player") c = new Player();
    else if (type == "slime") c = new Slime();

    return Creature::setupCreature<Creature>(c);
}
