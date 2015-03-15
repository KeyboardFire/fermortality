#include "GameLayer.h"

#include <iostream>
#include <cmath>
#include <algorithm>

USING_NS_CC;

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    // super init first
    if (!Layer::init()) return false;

    // FIXME this is a hack
    screenWidth = Director::getInstance()->getOpenGLView()->getFrameSize().width;
    screenHeight = Director::getInstance()->getOpenGLView()->getFrameSize().height;

    // add tilemap
    auto map = TMXTiledMap::create("tileset.tmx");
    addChild(map);

    // add slimes
    for (int i = 0; i < 10; ++i) {
        auto slime = Creature::create("slime");
        slime->setPosition(Vec2(
            RandomHelper::random_int(0, (int) map->getContentSize().width),
            RandomHelper::random_int(0, (int) map->getContentSize().height)
        ));
        slime->setAnchorPoint(Vec2(0, 0));
        addChild(slime, 0);
        enemies.push_back(slime);
    }

    // add player sprite
    player = Player::create();
    player->setPosition(Vec2(map->getTileSize().width, map->getTileSize().height));
    player->setAnchorPoint(Vec2(0, 0));
    addChild(player, 1);

    // set up collision detection
    auto layer = map->getLayer("Tile Layer 1");
    for (int x = 0; x < layer->getLayerSize().width; ++x) {
        for (int y = 0; y < layer->getLayerSize().height; ++y) {
            auto tile = layer->getTileAt(Vec2(x, y));
            if (tile != nullptr) {
                int gid = layer->getTileGIDAt(Vec2(x, y));
                auto properties = map->getPropertiesForGID(gid);
                ValueMap nullMap;
                ValueMap propertiesMap = (properties.getType() == Value::Type::NONE) ? nullMap : properties.asValueMap();
                if (propertiesMap.find("background") != propertiesMap.end()) {
                    // nop
                } else {
                    tiles.push_back(tile);
                }
            }
        }
    }

    // add keyboard listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // go!
    scheduleUpdate();

    return true;
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    auto pVelocity = player->velocity;
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            // can only jump when standing still on ground
            if (pVelocity->y == 0) {
                pVelocity->y = JUMP_SPEED;
            }
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            player->dir = player->dir < 0 ? 2 : 1;
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            player->dir = player->dir > 0 ? -2 : -1;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            cheat = true;
            break;
    }
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            player->dir = player->dir == 1 ? 0 : (player->dir == 2 ? -1 : (player->dir == -2 ? -1 : player->dir));
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            player->dir = player->dir == -1 ? 0 : (player->dir == -2 ? 1 : (player->dir == 2 ? 1 : player->dir));
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            cheat = false;
            break;
    }
}

void HelloWorld::update(float dt) {
    int oldHealth = player->health;

    auto pVelocity = player->velocity;
    if (player->dir > 0 && pVelocity->x <  RUN_SPEED) pVelocity->x += RUN_ACCELERATION;
    if (player->dir < 0 && pVelocity->x > -RUN_SPEED) pVelocity->x -= RUN_ACCELERATION;
    if (player->dir == 0) pVelocity->x *= RUN_FRICTION;

    if (cheat) pVelocity->y = JUMP_SPEED;

    updateCreature(player);

    for (auto s : enemies) {
        updateCreature(s);

        char collision = collide(player, s);
        if (collision) {
            if (s->collidedWithPlayer(collision, player)) {
                enemies.erase(std::remove(enemies.begin(), enemies.end(), s), enemies.end());
                continue;
            }
        }
    }

    // ugly hack
    if (player->health != oldHealth) hudLayer->updateHearts(player->health);
    if (player->health == 0) std::cout << "player is kill :(" << std::endl;

    setPosition(Vec2(screenWidth/2 - player->getPositionX(), screenHeight/2 - player->getPositionY()));
}

void HelloWorld::updateCreature(Creature *s) {
    auto v = s->velocity;

    v->y -= GRAVITY;

    float px = s->getPositionX() + v->x,
        py = s->getPositionY() + v->y;
    s->setPosition(Vec2(round(px), round(py)));

    for (auto tile : tiles) {
        switch (collide(s, tile)) {
            case 'b':
                v->y = 0;
                while (s->getBoundingBox().intersectsRect(tile->getBoundingBox())) {
                    s->setPositionY(++py);
                }
                break;
            case 'r':
                v->x = 0;
                while (s->getBoundingBox().intersectsRect(tile->getBoundingBox())) {
                    s->setPositionX(--px);
                }
                break;
            case 'l':
                v->x = 0;
                while (s->getBoundingBox().intersectsRect(tile->getBoundingBox())) {
                    s->setPositionX(++px);
                }
                break;
            case 't':
                v->y = 0;
                while (s->getBoundingBox().intersectsRect(tile->getBoundingBox())) {
                    s->setPositionY(--py);
                }
                break;
        }
    }

    s->update();
}

// returns 'b' for bottom, 'r' for right, 'l' for left, 't' for top, '\0' for no collision
char HelloWorld::collide(Sprite *s1, Sprite *s2) {
    // Algorithm slightly altered from
    // http://gamedev.stackexchange.com/a/29796/19034
    auto b1 = s1->getBoundingBox(), b2 = s2->getBoundingBox();
    float w = (b1.size.width  + b2.size.width)  / 2.0f,
          h = (b1.size.height + b2.size.height) / 2.0f,
          dx = b1.getMidX() - b2.getMidX(),
          dy = b1.getMidY() - b2.getMidY();
    if (fabs(dx) <= w && fabs(dy) <= h && !(fabs(dx) == w && fabs(dy) == h)) {
        // collision
        float wy = w * dy, hx = h * dx;
        if (wy > hx) {
            if (wy > -hx) {
                return 'b';
            } else {
                return 'r';
            }
        } else {
            if (wy > -hx) {
                return 'l';
            } else {
                return 't';
            }
        }
    }
    return 0;
}
