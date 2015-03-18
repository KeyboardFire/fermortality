#include "Player.h"
#include <iostream>

USING_NS_CC;

Player::Player(): dir(0), lookDir(0), invulnTime(0) {
}

Player* Player::create() {
    Player *p = new Player;
    Creature::setupCreature<Player>(p);
    p->health = 5;
    p->whip = nullptr;
    p->objectHeld = nullptr;
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

Vec2 Player::getHandPosition() {
    return Vec2(
        getPositionX() + (isFlippedX() ? getContentSize().width : 0),
        getPositionY() + getContentSize().height / 2
    );
}
