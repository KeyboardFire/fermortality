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

    if (whip != nullptr) {
        whip->setFlippedX(isFlippedX());
        whip->setAnchorPoint(Vec2(isFlippedX() ? 0 : 1, 0.5));
        whip->setPositionX(getHandPosition().x);
        whip->setPositionY(getHandPosition().y);

        int *whipTime = (int*) whip->getUserData();
        if (--(*whipTime) == 0) {
            delete whipTime;
            whip->removeFromParent();
            whip = nullptr;
        }
    }

    if (objectHeld != nullptr) {
        objectHeld->setPosition(getHandPosition());
    }
}

Vec2 Player::getHandPosition() {
    return Vec2(
        getPositionX() + (isFlippedX() ? getContentSize().width : 0),
        getPositionY() + getContentSize().height / 2
    );
}
