#ifndef _PLAYER_
#define _PLAYER_

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
    public:
        Player();
        ~Player();
        static Player* create(const std::string & filename);

        void initOptions();

        void addTouchEvents();
        void touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _p);

    private:

};

#endif // _PLAYER_