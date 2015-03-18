#include "HUDLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool HUDLayer::init() {
    // super init first
    if (!Layer::init()) return false;

    for (int i = 1; i <= 5; ++i) {
        auto heart = Sprite::create("heartFull.png");
        auto size = heart->getContentSize();
        heart->setPosition(Vec2(i * (size.width + 10) - 10, size.height));
        addChild(heart);
        hearts.push_back(heart);
    }

    // this is a hack FIXME use sprite sheets
    fullHeart = CCTextureCache::getInstance()->addImage("heartFull.png");
    emptyHeart = CCTextureCache::getInstance()->addImage("heartEmpty.png");

    return true;
}

void HUDLayer::updateHearts(int heartCount) {
    for (int i = 0; i < 5; ++i) {
        if (heartCount > i) hearts[i]->setTexture(fullHeart);
        else hearts[i]->setTexture(emptyHeart);
    }
}
