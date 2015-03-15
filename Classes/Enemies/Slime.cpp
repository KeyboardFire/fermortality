#include "Slime.h"

void Slime::update() {
    velocity->x += RandomHelper::random_real<float>(-0.5, 0.5);
    if (velocity->y == 0 && RandomHelper::random_int(0, 15) == 0) {
        velocity->y += 9;
    }
}

bool Slime::collidedWithPlayer(char dir, Creature *player) {
    if (player->velocity->y < 0 && player->getPositionY() > getPositionY()) {
        removeFromParent();
        return true;
    }
    player->damage(1);
    return false;
}
