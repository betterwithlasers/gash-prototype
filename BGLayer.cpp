#include "BGLayer.h"

USING_NS_CC;

Scene* BGLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BGLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BGLayer::init()
{
    //////////////////////////////
    // 1. super init first
	if( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) ) 
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->setColor(Color3B(80, 80, 80));
	

	bigLabel = Label::createWithTTF("FASTER", "fonts/MAVERICK.ttf", 144);
	bigLabel->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bigLabel, 1); 
    //bigLabel->runAction(RepeatForever::create(Sequence::create(CCScaleTo::create(0.5f, 1.3f, 1.0f), CCScaleTo::create(0.5f, 1.0f, 1.3f), NULL)));
	bigLabel->runAction(RepeatForever::create(Sequence::create(CCMoveBy::create(0.05f, Vec2(-30.0f, 0.0f)), CCMoveBy::create(0.05f, Vec2(30.0f, 0.0f)), CCMoveBy::create(0.05f, Vec2(0.0f, 0.0f)), 
															   CCMoveBy::create(0.05f, Vec2(0.0f, 30.0f)), CCMoveBy::create(0.05f, Vec2(0.0f, -30.0f)), CCMoveBy::create(0.05f, Vec2(0.0f, 0.0f)), NULL)));														  
    
    return true;
}