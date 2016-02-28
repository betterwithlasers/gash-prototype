#include "Wall.h"

using namespace cocos2d;

Wall::Wall() {}

Wall::~Wall() {}

Wall* Wall::create(const std::string & filename)
{
    Wall* pSprite = new Wall();

    if (pSprite->initWithFile(filename))
    {
        pSprite->autorelease();

        pSprite->initOptions();

        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void Wall::initOptions()
{
    // do things here like setHealth(), setPosition(), any custom logic.
}



