#include "Player.h"

Player* Player::create() {
    Player *p = new Player;
    return Creature::setupCreature<Player>(p);
}
