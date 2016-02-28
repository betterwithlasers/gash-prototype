#ifndef _WALL_
#define _WALL_

#include "cocos2d.h"

class Wall : public cocos2d::Sprite
{
    public:
        Wall();
        ~Wall();
        static Wall* create(const std::string & filename);

        void initOptions();

        void addTouchEvents();
        void touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _p);

    private:

};

#endif // _WALL_