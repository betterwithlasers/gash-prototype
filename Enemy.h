#ifndef _ENEMY_
#define _ENEMY_

#include "cocos2d.h"

class Enemy : public cocos2d::Sprite
{
    public:
        Enemy();
        ~Enemy();
        static Enemy* create(const std::string & filename);

        void initOptions();

        void addTouchEvents();
        void touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _p);
		
		void setHealth(int health);
		int getHealth();

    private:
		int health;

};

#endif // _ENEMY_