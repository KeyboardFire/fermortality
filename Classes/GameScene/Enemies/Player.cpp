#include "Player.h"
#include <iostream>

USING_NS_CC;

Player::Player(): dir(0), lookDir(0), invulnTime(0) {
}

Player* Player::create() {
    Player *p = new Player;
    Creature::setupCreature<Player>(p);
    p->health = 5;
    return p;
}

void Player::damage(int amount) {
    if (!invulnTime) {
        health -= amount;
        invulnTime = 50;
        setOpacity(150);
    }
}

void Player::update(int aiInfo) {
    if (invulnTime) {
        if (!--invulnTime) setOpacity(255);
        else setOpacity(150 + (invulnTime * 5 % 105));
    }
}
