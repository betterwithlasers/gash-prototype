#ifndef __BGLAYER_H__
#define __BGLAYER_H__

#include "cocos2d.h"

class BGLayer : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(BGLayer);
	
private:	
	cocos2d::Label* bigLabel;
	
};

#endif // __BGLAYER_H__
