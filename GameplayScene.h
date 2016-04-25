#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__

#include "cocos2d.h"
#include "HUDLayer.h"
#include "BGLayer.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "CustomFollow.h"
#include <vector>
#include <string>
#include <iostream>

#include "SimpleAudioEngine.h"

class Gameplay : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	
	void update(float dt);
	
	
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event *event);

    
    //menu selector callbacks
    void restartPressed();
	void pausePressed();
	
	//helper methods
	unsigned long getTimeTick();
	float normaliseAngle(float theta);
	
	void collidePlayerEnemies();
	
	void populateMapPhysics(TMXTiledMap* newMap);
	void scrollMaps(Vec2 playerPosDifference);
	
    // implement the "static create()" method manually
    CREATE_FUNC(Gameplay);
	
private:	
	bool isPaused;
	
	bool screenHeld;
	
	unsigned long startTime;
	
	unsigned long lastSlowFocus;
	
	unsigned long lastMoveRight;
	
	Rect spriteRect;
	
	Player* player;
	
	cocos2d::Sprite* playerRight;
	
	std::vector<Enemy*> enemies;
	
	std::vector<Sprite*> walls;
	
	cocos2d::Label* label1;
	cocos2d::Label* label2;
	cocos2d::Label* label3;
	
	cocos2d::Sprite* arrowR;
	cocos2d::Sprite* arrowL;
	
	bool gashAllowed;
	
	cocos2d::PhysicsBody* playerPhysics;
	
	cocos2d::ParticleSystemQuad* playerParticle;
	cocos2d::ParticleSystemQuad* trailParticle;
	
	cocos2d::Vec2 impulse;
	
	cocos2d::Vec2 lastPlayerPos;
	
	//cocos2d::ParticleFireworks* bloodSpray;
	
	bool movingRight;
	
	std::vector<TMXTiledMap*> maps;
	
	int tileCount;
};

#endif // __GAMEPLAY_SCENE_H__
