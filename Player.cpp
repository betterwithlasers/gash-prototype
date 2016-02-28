#include "Player.h"

using namespace cocos2d;

Player::Player() {}

Player::~Player() {}

Player* Player::create(const std::string & filename)
{
    Player* pSprite = new Player();

    if (pSprite->initWithFile(filename))
    {
        pSprite->autorelease();

        pSprite->initOptions();

        //pSprite->addTouchEvents();

        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void Player::initOptions()
{
    // do things here like setHealth(), setPosition(), any custom logic.
}

void Player::addTouchEvents()	//directly attached to the sprite, removed for now due to possible confusion
{
	/*
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {   
        cocos2d::Vector2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();

        if(rect.containsPoint(p))
        {
            return true; // to indicate that we have consumed it.
        }

        return false; // we did not consume this event, pass thru.
    };

    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        MySprite::touchEvent(touch);
    };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
	*/
}

void Player::touchEvent(Touch* touch, Vec2 point)
{
    //CCLOG("touched MySprite");
}

