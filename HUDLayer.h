#ifndef __HUDLAYER_H__
#define __HUDLAYER_H__

#include "cocos2d.h"
#include "GameplayScene.h"

class HUDLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	
    // a selector callback
    void restartPressed();
	
	void pausePressed();
	
    
    // implement the "static create()" method manually
    CREATE_FUNC(HUDLayer);
	
private:	
	bool isPaused;
	
	cocos2d::Label* pauseLabel;
};

#endif // __HUDLAYER_H__
