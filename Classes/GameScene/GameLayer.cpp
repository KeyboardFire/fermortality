#include "GameLayer.h"

#include <iostream>
#include <cmath>
#include <algorithm>

USING_NS_CC;

// on "init" you need to initialize your instance
bool GameLayer::init() {
    // super init first
    if (!Layer::init()) return false;

    screenWidth = Director::getInstance()->getOpenGLView()->getFrameSize().width;
    screenHeight = Director::getInstance()->getOpenGLView()->getFrameSize().height;

    // add tilemap
    map = TMXTiledMap::create("tilemap.tmx");
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

    // background image
    auto bg = Sprite::create("bg.png");
    bg->setContentSize(layer->getContentSize());
    bg->getTexture()->setTexParameters({GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT});
    bg->setTextureRect(Rect(0, 0, bg->getContentSize().width, bg->getContentSize().height));
    bg->setAnchorPoint(Vec2(0, 0));
    addChild(bg, -1);

    // add player sprite
    player = Player::create();
    player->setPosition(Vec2(map->getTileSize().width, map->getTileSize().height));
    player->setAnchorPoint(Vec2(0, 0));
    addChild(player, 1);

    // add slimes
    for (int i = 0; i < 25; ++i) {
        auto slime = Entity::create("slime");
        placeSprite(slime);
        enemies.push_back(slime);
    }

    // add rocks
    for (int i = 0; i < 10; ++i) {
        auto rock = Entity::create("rock");
        placeSprite(rock);
        items.push_back(rock);
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

void GameLayer::placeSprite(Sprite *s) {
    s->setAnchorPoint(Vec2(0, 0));

    bool retry;
    do {
        s->setPosition(Vec2(
            RandomHelper::random_int((int) map->getTileSize().width + 1, (int) (map->getContentSize().width - map->getTileSize().width)),
            RandomHelper::random_int((int) map->getTileSize().height + 1, (int) (map->getContentSize().height - map->getTileSize().height))
        ));

        retry = false;

        // no floating objects
        Vec2 stp = tilePosition(s);
        if (layer->getTileAt(Vec2((int)stp.x, (int)stp.y + 1)) == nullptr) {
            retry = true;
        }

        // no objects stuck in tiles
        if (!retry) {
            for (auto t : tiles) {
                if (t->getBoundingBox().intersectsRect(s->getBoundingBox())) {
                    retry = true;
                }
            }
        }
    } while (retry);

    addChild(s);
}

void GameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    auto pVelocity = player->velocity;
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            player->lookDir = 1;
            player->setSpriteFrame(player->getFrame(1));
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            player->lookDir = -1;
            player->setSpriteFrame(player->getFrame(2));
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            player->dir = player->dir < 0 ? 2 : 1;
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            player->dir = player->dir > 0 ? -2 : -1;
            break;
        case EventKeyboard::KeyCode::KEY_SHIFT:
            player->isRunning = true;
            break;
        case EventKeyboard::KeyCode::KEY_Z:
            // can only jump when standing still on ground
            if (pVelocity->y == 0) {
                pVelocity->y = Player::JUMP_SPEED;
            }
            break;
        case EventKeyboard::KeyCode::KEY_X:
            if (player->objectHeld != nullptr) {
                // throw / drop object
                player->objectHeld->velocity->x = (player->lookDir == -1 ? 5 : 25) * (player->isFlippedX() ? 1 : -1);
                player->objectHeld->velocity->y = player->lookDir == 1 ? 25 : 5;
                player->objectHeld = nullptr;
            } else {
                switch (player->lookDir) {
                    case 0:
                    case 1:
                        if (player->whip == nullptr) {
                            player->whip = Sprite::create("whip.png");
                            int *whipTime = new int(30);
                            player->whip->setUserData((void*) whipTime);
                            player->whip->setAnchorPoint(Vec2(1, 0.5));
                            addChild(player->whip);
                        }
                        break;
                    case -1:
                        for (auto s : items) {
                            if (player->getBoundingBox().intersectsRect(s->getBoundingBox())) {
                                player->objectHeld = s;
                                break;
                            }
                        }
                        break;
                }
            }
            break;
        default: // to get rid of annoying warnings
            break;
    }
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            player->lookDir = 0;
            player->setSpriteFrame(player->getFrame(0));
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            player->dir = player->dir == 1 ? 0 : (player->dir == 2 ? -1 : (player->dir == -2 ? -1 : player->dir));
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            player->dir = player->dir == -1 ? 0 : (player->dir == -2 ? 1 : (player->dir == 2 ? 1 : player->dir));
            break;
        case EventKeyboard::KeyCode::KEY_SHIFT:
            player->isRunning = false;
            break;
        default: // to get rid of annoying warnings
            break;
    }
}

void GameLayer::update(float dt) {
    int oldHealth = player->health;

    auto pVelocity = player->velocity;
    if (player->dir > 0 && pVelocity->x < Player::RUN_SPEED * (player->isRunning ? 2 : 1)) {
        pVelocity->x += Player::RUN_ACCELERATION * (player->isRunning ? 2 : 1);
        player->setFlippedX(true);
    }
    if (player->dir < 0 && pVelocity->x > -Player::RUN_SPEED * (player->isRunning ? 2 : 1)) {
        pVelocity->x -= Player::RUN_ACCELERATION * (player->isRunning ? 2 : 1);
        player->setFlippedX(false);
    }
    if (player->dir == 0) pVelocity->x *= PLAYER_FRICTION;

    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        auto s = *it;

        updateEntity(s);

        char collision = collide(player, s);
        if (collision) {
            s->collidedWithPlayer(collision, player);
        }

        if (player->whip != nullptr && player->whip->getBoundingBox().intersectsRect(s->getBoundingBox())) {
            s->collidedWithWhip(player->whip);
        }

        for (auto it2 = items.begin(); it2 != items.end(); ++it2) {
            auto item = *it2;

            if ((fabs(item->velocity->x) + fabs(item->velocity->y)) > 0.1 && item->getBoundingBox().intersectsRect(s->getBoundingBox())) {
                // TODO make this customizable per item
                s->damage(1);
            }
        }

        if (s->health <= 0) {
            s->removeFromParent();
            enemies.erase(std::remove(enemies.begin(), enemies.end(), s), enemies.end());
            --it;
        }
    }

    for (auto it = items.begin(); it != items.end(); ++it) {
        auto item = *it;

        updateEntity(item);
    }

    updateEntity(player);

    // ugly hack
    if (player->health != oldHealth) scene->updateHearts(player->health);

    setPosition(Vec2(screenWidth/2 - player->getPositionX(), screenHeight/2 - player->getPositionY()));
}

void GameLayer::updateEntity(Entity *s) {
    auto v = s->velocity;

    v->y -= GRAVITY;
    if (v->y < MAX_FALL_SPEED) v->y = MAX_FALL_SPEED;

    // don't apply to player for more accurate run speed
    if (s != player) v->x *= FRICTION;

    int px = s->getPositionX() + round(v->x),
        py = s->getPositionY() + round(v->y);
    s->setPosition(Vec2(px, py));

    std::vector<Sprite*> collisions;
    for (auto tile : tiles) {
        if (fabs(s->getPositionX() - tile->getPositionX()) <= map->getTileSize().width &&
            fabs(s->getPositionY() - tile->getPositionY()) <= map->getTileSize().height) {
            collisions.push_back(tile);
        }
    }

    char collideDir;
    for (auto tile : collisions) {
        collideDir = collide(s, tile, true);

        // the following section exists because sometimes (read: often) the
        // player will move multiple pixels in a frame, gliding over 1-wide
        // pits or failing to enter 1-wide holes in walls. this solves the
        // problem by putting the player inside them anyway if he's "close
        // enough." yes, it's a hack. a very ugly one. but it works.
        if (s == player && collideDir >= 'A' && collideDir <= 'Z' && collisions.size() == 1) {
            // shift it inside so it doesn't pop back out
            if (collideDir == 'B') s->setPositionY(s->getPositionY() - THRESHOLD);
            collideDir = (collideDir == 'B' || collideDir == 'T') ?
                (s->getPositionX() < tile->getPositionX() ? 'r' : 'l') :
                (s->getPositionY() < tile->getPositionY() ? 't' : 'b');
        }

        switch (collideDir) {
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

    if ((int)stp.x == 0) ++stp.x;
    if ((int)stp.x == layer->getLayerSize().width - 1) --stp.x;
    if ((int)stp.y == 0) ++stp.y;
    if ((int)stp.y == layer->getLayerSize().height - 1) --stp.y;

    if (layer->getTileAt(Vec2((int)stp.x - 1, (int)stp.y + 1)) == nullptr) aiInfo |= Entity::AIInfo::cliffLeft;
    if (layer->getTileAt(Vec2((int)stp.x + 1, (int)stp.y + 1)) == nullptr) aiInfo |= Entity::AIInfo::cliffRight;

    s->update(aiInfo);
}

Vec2 GameLayer::tilePosition(Sprite *s) {
    return Vec2(
        s->getPositionX() / map->getTileSize().width,
        ((map->getMapSize().height * map->getTileSize().height) - s->getPositionY()) / map->getTileSize().height
    );
}

// returns 'b' for bottom, 'r' for right, 'l' for left, 't' for top, '\0' for no collision
char GameLayer::collide(Sprite *s1, Sprite *s2, bool veryclose) {
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
                return (veryclose && w - fabs(dx) < THRESHOLD ? 'B' : 'b');
            } else {
                return (veryclose && h - fabs(dy) < THRESHOLD ? 'R' : 'r');
            }
        } else {
            if (wy > -hx) {
                return (veryclose && h - fabs(dy) < THRESHOLD ? 'L' : 'l');
            } else {
                return (veryclose && w - fabs(dx) < THRESHOLD ? 'T' : 't');
            }
        }
    }
    return 0;
}
