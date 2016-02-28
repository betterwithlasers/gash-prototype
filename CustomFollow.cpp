#include "CustomFollow.h"

using namespace cocos2d;

CustomFollow::CustomFollow() {}

CustomFollow::~CustomFollow()
{
    CC_SAFE_RELEASE(_followedNode);
}

CustomFollow* CustomFollow::create(Node *followedNode, const Rect& rect/* = Rect::ZERO*/)
{
    CustomFollow *follow = new (std::nothrow) CustomFollow();
    if (follow && follow->initWithTarget(followedNode, rect))
    {
        follow->autorelease();
        return follow;
    }
    CC_SAFE_DELETE(follow);
    return nullptr;
}

bool CustomFollow::initWithTarget(Node *followedNode, const Rect& rect/* = Rect::ZERO*/)
{
    CCASSERT(followedNode != nullptr, "FollowedNode can't be NULL");
 
    followedNode->retain();
    _followedNode = followedNode;
    _worldRect = rect;
    _boundarySet = !rect.equals(Rect::ZERO);
    _boundaryFullyCovered = false;

    Size winSize = Director::getInstance()->getWinSize();
    _fullScreenSize.set(winSize.width, winSize.height);
    _halfScreenSize = _fullScreenSize * 0.5f;

    if (_boundarySet)
    {
        _leftBoundary = -((rect.origin.x+rect.size.width) - _fullScreenSize.x);
        _rightBoundary = -rect.origin.x ;
        _topBoundary = -rect.origin.y;
        _bottomBoundary = -((rect.origin.y+rect.size.height) - _fullScreenSize.y);

        if(_rightBoundary < _leftBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            _rightBoundary = _leftBoundary = (_leftBoundary + _rightBoundary) / 2;
        }
        if(_topBoundary < _bottomBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            _topBoundary = _bottomBoundary = (_topBoundary + _bottomBoundary) / 2;
        }

        if( (_topBoundary == _bottomBoundary) && (_leftBoundary == _rightBoundary) )
        {
            _boundaryFullyCovered = true;
        }
    }
    
    return true;
}

void CustomFollow::step(float dt)
{
    CC_UNUSED_PARAM(dt);

    if(_boundarySet)
    {
        // whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
        if(_boundaryFullyCovered)
        {
            return;
        }

        Vec2 tempPos = _halfScreenSize - _followedNode->getPosition();

        _target->setPosition(clampf(tempPos.x, _leftBoundary, _rightBoundary),
                                   clampf(tempPos.y, _bottomBoundary, _topBoundary));
    }
    else
    {
        _target->setPosition(_halfScreenSize - _followedNode->getPosition());
    }
}

//
//(personal addition) this creates a pointer back to the layer that called the CustomFollow action, and is also the parent of the sprite being followed
void CustomFollow::setSpriteParent(Layer* spriteParent)
{	
	this->spriteParent = spriteParent;
}

//(personal addition)
void CustomFollow::endAdjusting() 
{
	adjusting = false;
}
