#include "Enemy.h"

using namespace cocos2d;

Enemy::Enemy() {}

Enemy::~Enemy() {}

Enemy* Enemy::create(const std::string & filename)
{
    Enemy* pSprite = new Enemy();

    if (pSprite->initWithFile(filename))
    {
        pSprite->autorelease();

        pSprite->initOptions();

        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void Enemy::initOptions()
{
    // do things here like setHealth(), setPosition(), any custom logic.
}

void Enemy::setHealth(int health)
{
	this->health = health;
}

int Enemy::getHealth()
{
	return health;
}

