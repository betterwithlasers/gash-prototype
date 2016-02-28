#ifndef _CUSTOMFOLLOW_H_
#define _CUSTOMFOLLOW_H_

#include "cocos2d.h"

using namespace cocos2d;

class CustomFollow : public cocos2d::Follow
{
    public:
		CustomFollow();
        ~CustomFollow();
		static CustomFollow* create(Node *followedNode, const Rect& rect/* = Rect::ZERO*/);
		
		bool initWithTarget(Node *followedNode, const Rect& rect/* = Rect::ZERO*/);
		void step(float dt);
		
		void setSpriteParent(Layer* spriteParent);
		void endAdjusting();
		

    private:
		Layer *spriteParent;
		bool adjusting;
		
		/** Node to follow. */
		Node *_followedNode;

		/** Whether camera should be limited to certain area. */
		bool _boundarySet;

		/** If screen size is bigger than the boundary - update not needed. */
		bool _boundaryFullyCovered;

		/** Fast access to the screen dimensions. */
		Vec2 _halfScreenSize;
		Vec2 _fullScreenSize;

		/** World boundaries. */
		float _leftBoundary;
		float _rightBoundary;
		float _topBoundary;
		float _bottomBoundary;
		Rect _worldRect;
};

#endif 