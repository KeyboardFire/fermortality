#include "Player.h"
#include <iostream>

Player::Player(): dir(0) {
}

Player* Player::create() {
    Player *p = new Player;
    return Creature::setupCreature<Player>(p);
}
