#include "GameLayer.h"

#include <iostream>
#include <cmath>
#include <algorithm>

USING_NS_CC;

// on "init" you need to initialize your instance
bool GameLayer::init() {
    // super init first
    if (!Layer::init()) return false;

    // FIXME this is a hack
    screenWidth = Director::getInstance()->getOpenGLView()->getFrameSize().width;
    screenHeight = Director::getInstance()->getOpenGLView()->getFrameSize().height;

    // add tilemap
    map = TMXTiledMap::create("tileset.tmx");
    addChild(map);

    // set up collision detection
    layer = map->getLayer("Tile Layer 1");
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

    // add player sprite
    player = Player::create();
    player->setPosition(Vec2(map->getTileSize().width, map->getTileSize().height));
    player->setAnchorPoint(Vec2(0, 0));
    addChild(player, 1);

    // add slimes
    for (int i = 0; i < 25; ++i) {
        auto slime = Creature::create("slime");
        bool retry;
        do {
            slime->setPosition(Vec2(
                RandomHelper::random_int((int) map->getTileSize().width + 1, (int) (map->getContentSize().width - map->getTileSize().width)),
                RandomHelper::random_int((int) map->getTileSize().height + 1, (int) (map->getContentSize().height - map->getTileSize().height))
            ));

            retry = false;

            // no floating slimes
            Vec2 stp = tilePosition(slime);
            if (layer->getTileAt(Vec2((int)stp.x, (int)stp.y + 1)) == nullptr) {
                retry = true;
            }

            // no slimes stuck in tiles
            if (!retry) {
                for (auto t : tiles) {
                    if (t->getBoundingBox().intersectsRect(slime->getBoundingBox())) {
                        retry = true;
                    }
                }
            }
        } while (retry);
        slime->setAnchorPoint(Vec2(0, 0));
        addChild(slime, 0);
        enemies.push_back(slime);
    }

    // add keyboard listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameLayer::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // go!
    scheduleUpdate();

    return true;
}

void GameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
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

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
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

void GameLayer::update(float dt) {
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
    if (player->health != oldHealth) scene->updateHearts(player->health);

    setPosition(Vec2(screenWidth/2 - player->getPositionX(), screenHeight/2 - player->getPositionY()));
}

void GameLayer::updateCreature(Creature *s) {
    auto v = s->velocity;

    v->y -= GRAVITY;
    if (v->y < -20) v->y = -20; // TODO figure this hack out

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

    int aiInfo = 0;

    Vec2 stp = tilePosition(s);

    if (layer->getTileAt(Vec2((int)stp.x - 1, (int)stp.y + 1)) == nullptr) aiInfo |= Creature::AIInfo::cliffLeft;
    if (layer->getTileAt(Vec2((int)stp.x + 1, (int)stp.y + 1)) == nullptr) aiInfo |= Creature::AIInfo::cliffRight;

    s->update(aiInfo);
}

Vec2 GameLayer::tilePosition(Sprite *s) {
    return Vec2(
        s->getPositionX() / map->getTileSize().width,
        ((map->getMapSize().height * map->getTileSize().height) - s->getPositionY()) / map->getTileSize().height
    );
}

// returns 'b' for bottom, 'r' for right, 'l' for left, 't' for top, '\0' for no collision
char GameLayer::collide(Sprite *s1, Sprite *s2) {
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
