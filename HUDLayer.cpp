#include "HUDLayer.h"

USING_NS_CC;

Scene* HUDLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HUDLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HUDLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto restartItem = MenuItemImage::create(
                                           "restartButton.png",
                                           "restartButton.png",
                                           CC_CALLBACK_0(HUDLayer::restartPressed, this));
    
	restartItem->setPosition(Vec2(visibleSize.width-restartItem->getContentSize().width/2, restartItem->getContentSize().height/2));
	
								
										
								
	auto pauseItem = MenuItemImage::create(
                                           "pauseButton.png",
                                           "pauseButton.png",
                                           CC_CALLBACK_0(HUDLayer::pausePressed, this));
    
	pauseItem->setPosition(Vec2(pauseItem->getContentSize().width/2, visibleSize.height - pauseItem->getContentSize().height/2));			

    // create menu, it's an autorelease object
    auto menu = Menu::create(restartItem, pauseItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    
    pauseLabel = Label::createWithTTF("Paused", "fonts/Marker Felt.ttf", 24);
    pauseLabel->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 - 20));
	pauseLabel->setVisible(false);
    this->addChild(pauseLabel, 1);

    
    return true;
}


void HUDLayer::restartPressed()
{
	Scene* currentScene = Gameplay::createScene();
	
    Director::getInstance()->replaceScene(currentScene);	
}

void HUDLayer::pausePressed()
{
	if (!isPaused)
	{
		Director::getInstance()->pause();
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
		
		isPaused = true;
		pauseLabel->setVisible(true);
	}
	else 
	{
		Director::getInstance()->resume();
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
		
		isPaused = false;
		pauseLabel->setVisible(false);
	}
}