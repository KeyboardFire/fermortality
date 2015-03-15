#ifndef __HUD_LAYER_H__
#define __HUD_LAYER_H__

#include "../cocos2d/cocos/cocos2d.h"

#include <vector>

USING_NS_CC;

class HUDLayer: public cocos2d::Layer {
public:
    // auto-generated "static create()" method
    CREATE_FUNC(HUDLayer);

    virtual bool init();
    void updateHearts(int heartCount);

    Texture2D *fullHeart, *emptyHeart;

    std::vector<Sprite*> hearts;
};

#endif // __HUD_LAYER_H__
