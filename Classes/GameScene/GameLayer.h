#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

class GameScene; // forward declaration

#include "cocos2d.h"
#include "Entity.h"
#include "Entities/Entities.h"
#include "GameScene.h"

#include <vector>

USING_NS_CC;

class GameLayer: public cocos2d::Layer {
public:
    // auto-generated "static create()" method
    CREATE_FUNC(GameLayer);

    virtual bool init();

    GameScene *scene;

    int screenWidth, screenHeight;

    Player* player;
    const int GRAVITY = 1, MAX_FALL_SPEED = -20;
    const float FRICTION = 0.95, PLAYER_FRICTION = 0.9;
    std::vector<Entity*> enemies, items;
    std::vector<Sprite*> tiles;

    void placeSprite(Sprite *s);

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    virtual void update(float dt);
    void updateEntity(Entity *s);
    Vec2 tilePosition(Sprite *s);
    char collide(Sprite *s1, Sprite *s2, bool veryclose = false);

    TMXTiledMap *map;
    TMXLayer *layer;

    const int THRESHOLD = 4;
};

#endif // __GAME_LAYER_H__
