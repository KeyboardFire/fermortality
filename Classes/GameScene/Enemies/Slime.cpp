#include "Slime.h"

void Slime::update(int aiInfo) {
    velocity->x += RandomHelper::random_real<float>(
        (aiInfo & AIInfo::cliffLeft ? 0 : -0.5),
        (aiInfo & AIInfo::cliffRight ? 0 : 0.5)
    );
    if (velocity->y == 0 && RandomHelper::random_int(0, 15) == 0) {
        velocity->y += 9;
    }
}

void Slime::collidedWithPlayer(char dir, Creature *player) {
    if (player->velocity->y < 0 && player->getPositionY() > getPositionY()) {
        damage(1);
    } else {
        player->damage(1);
    }
}
