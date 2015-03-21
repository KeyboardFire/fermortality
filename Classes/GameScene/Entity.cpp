#include "Entity.h"
#include "Entities/Entities.h"

USING_NS_CC;

Entity* Entity::create(std::string type) {
    Entity *c;
    if (type == "player") c = new Player();
    else if (type == "slime") c = new Slime();
    else if (type == "rock") c = new Rock();

    return Entity::setupEntity<Entity>(c);
}

SpriteFrame* Entity::getFrame(int i) {
    return SpriteFrameCache::getInstance()->getSpriteFrameByName(filename() + std::to_string(i));
}
