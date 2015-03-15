#include "HUDLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool HUDLayer::init() {
    // super init first
    if (!Layer::init()) return false;

    for (int i = 1; i <= 5; ++i) {
        auto heart = Sprite::create("sprites/HUD/hud_heartFull.png");
        auto size = heart->getContentSize();
        heart->setPosition(Vec2(i * (size.width + 10) - 10, size.height));
        addChild(heart);
        hearts.push_back(heart);
    }

    return true;
}

void HUDLayer::updateHearts(int heartCount) {
    for (int i = 0; i < 5; ++i) {
        if (heartCount > i) hearts[i]->setOpacity(255);
        else hearts[i]->setOpacity(0);
    }
}
