#include "GameplayScene.h"

#define PTM_RATIO 32.0	

USING_NS_CC;

Scene* Gameplay::createScene()
{
    // 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();	//as opposed to Scene::create();
	
	auto physicsWorld = scene->getPhysicsWorld();
	physicsWorld->setGravity(Vec2(0.0f,-250.0f)); 	//default is 0, -98
    //physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
    // 'layer' is an autorelease object
    auto layer = Gameplay::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
	BGLayer *bg = new BGLayer();
	bg->init();
	scene->addChild(bg, -1);	//put it in behind everything else
	
	HUDLayer *hud = new HUDLayer();
	hud->init();
	scene->addChild(hud, 3);	//put it in front of everything else

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Gameplay::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//due to possible slow motion being toggled from previous player life, must always reset it
	Director::getInstance()->getScheduler()->setTimeScale(1.0f);
	
	tileCount = 0;
	
	TMXTiledMap* map1 = TMXTiledMap::create("tilemaps/mapX.tmx");		
	map1->setPosition(Vec2(0,0)); 		
    this->addChild(map1, 2);
	maps.push_back(map1);
	
	populateMapPhysics(map1);
	
	TMXTiledMap* map2 = TMXTiledMap::create("tilemaps/map0.tmx");		
	map2->setPosition(Vec2(2400,0)); 		
    this->addChild(map2, 2);
	maps.push_back(map2);
	
	populateMapPhysics(map2);
	
	
    label1 = Label::createWithTTF("Hi World", "fonts/Marker Felt.ttf", 24);
    label1->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 20));
    this->addChild(label1, 3);
	
	label2 = Label::createWithTTF("Hi World", "fonts/Marker Felt.ttf", 24);
    label2->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 60));
	this->addChild(label2, 3);
	
	label3 = Label::createWithTTF("Hi World", "fonts/Marker Felt.ttf", 24);
    label3->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 100));
	this->addChild(label3, 3);
	
	
	isPaused = false;
	screenHeld = false;
	
	gashAllowed = false;
	
	movingRight = true;
	
	player = Player::create("player.png");
	player->setAnchorPoint(Vec2(0,0));
	player->setPosition(Vec2(0,180));
	this->addChild(player, 4);
	
	player->setOpacity(25);	//out of a max 255
	
	playerPhysics = PhysicsBody::createBox(Size(72.0f, 72.0f), PhysicsMaterial(0.2f, 0.88f, 1.0f));	//density, friction, bounciness 		//friction tradeoff between 0.8 (control) and 0.9 (speed)
	playerPhysics->setRotationEnable(false);
	player->setPhysicsBody(playerPhysics);
	playerPhysics->applyImpulse(Vec2(600000,0));
	

	//playerPhysics->setVelocity(Vec2(cocos2d::random(0,600), cocos2d::random(0,200)));
	
	
	arrowR = Sprite::create("arrow.png");
	arrowR->setAnchorPoint(Vec2(0,0.5f));
	this->addChild(arrowR, 3);
	arrowR->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.75f, -60), RotateTo::create(0.75f, 60), NULL)));
	
	arrowL = Sprite::create("arrow.png");
	arrowL->setAnchorPoint(Vec2(0,0.5f));
	this->addChild(arrowL, 3);
	arrowL->setRotation(-180);
	arrowL->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.75f, 240), RotateTo::create(0.75f, 120), NULL)));
	
	arrowR->setVisible(false);
	arrowL->setVisible(false);
	
	
	Enemy* enemy1 = Enemy::create("grunt.png");
	enemy1->setAnchorPoint(Vec2(0,0));
	enemy1->setPosition(Vec2(700, 60));		//72
	enemy1->setHealth(1);
	this->addChild(enemy1, 2);
	enemies.push_back(enemy1);
	
	Enemy* enemy2 = Enemy::create("grunt.png");
	enemy2->setAnchorPoint(Vec2(0,0));
	enemy2->setPosition(Vec2(400, 60));		//72
	enemy2->setHealth(1);
	this->addChild(enemy2, 2);
	enemies.push_back(enemy2);
	
	Enemy* enemy3 = Enemy::create("grunt.png");
	enemy3->setAnchorPoint(Vec2(0,0));
	enemy3->setPosition(Vec2(200, 60));		//72
	enemy3->setHealth(1);
	this->addChild(enemy3, 2);
	enemies.push_back(enemy3);
	
	
	playerParticle = ParticleSystemQuad::create("particles/test2.plist");
	playerParticle->setPositionType(kCCPositionTypeGrouped);
	this->addChild(playerParticle,4);
	
	trailParticle = ParticleSystemQuad::create("particles/cool3.plist");
	trailParticle->setPositionType(kCCPositionTypeRelative);
	//trailParticle->setTotalParticles(500);
	this->addChild(trailParticle,3);
	
	
	//CustomFollow* scrolling = CustomFollow::create(player, Rect(0, 0, visibleSize.width * 100, visibleSize.height));	//scrolling is limited within this rectangle
	//scrolling->setSpriteParent(this);	//an important link (theres probably a 'cleaner' way to do this)
	//this->runAction(scrolling);
	
	Rect followRect = Rect(0, 0, visibleSize.width * 100, visibleSize.height);
	//this->runAction(Follow::create(player, followRect));                      //NEED TO SOLVE THIS ASAP
	
	startTime = getTimeTick();
	
	this->scheduleUpdate();
	
	auto touchListener = EventListenerTouchOneByOne::create();
	
	touchListener->onTouchBegan = CC_CALLBACK_2(Gameplay::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Gameplay::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Gameplay::onTouchEnded, this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	log("started");
		
    return true;
}

void Gameplay::update(float dt)
{
	//collision section
	collidePlayerEnemies();
	
	playerParticle->setPosition(Vec2(player->getPositionX()+36, player->getPositionY()+36));
	trailParticle->setPosition(Vec2(player->getPositionX()+36, player->getPositionY()+36));
	
	arrowR->setPosition(Vec2(player->getPositionX()+36, player->getPositionY()+36));
	arrowL->setPosition(Vec2(player->getPositionX()+36, player->getPositionY()+36));
	
	
	Vec2 playerPosDifference = lastPlayerPos - player->getPosition();
	scrollMaps(playerPosDifference);
	
	
	//THE NORMAL CASE
	if (playerPosDifference.x < 0)
	{
		movingRight = true;
		lastMoveRight = getTimeTick();
	}
	else if (playerPosDifference.x > 0)
	{
		if (getTimeTick() - lastMoveRight < 750) //&& getTimeTick() - lastSlowFocus > 2500)
		{
			movingRight = false;
			
			//Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0.04f);
			//lastSlowFocus = getTimeTick();
		}
		else
		{
			movingRight = true;
		}
	}
	
	
	if (movingRight && screenHeld)
	{	
		arrowR->setVisible(true); 
		arrowL->setVisible(false);	
	}		
	else if (!movingRight && screenHeld)
	{
		arrowL->setVisible(true); 
		arrowR->setVisible(false);	
	}
	
	
	for (int i = 0 ; i <walls.size(); i++)
	{
		Sprite* w = walls[i];
		w->setPositionX(w->getPositionX() + playerPosDifference.x);	 	 //moves the same amount as the player, but in opposite direction
	}
	
	lastPlayerPos = player->getPosition();
	
	
	char text[256];
	sprintf(text, "Tile count: %d", tileCount); 
	label1->setString(text);
	
	
	char text2[256];
	sprintf(text2, "# of tilemaps:  %d", maps.size()); 
	label2->setString(text2);
	
	char text3[256];
	sprintf(text3, " %d", 2); 
	label3->setString(text3);
}

void Gameplay::collidePlayerEnemies()
{
	Rect playerBox = player->getBoundingBox();
	
	for (int i = 0; i<enemies.size();i++)
	{
		Enemy* e = enemies[i];
		Rect enemyBox = e->getBoundingBox();
			
		if (playerBox.intersectsRect(enemyBox))  //or containsPoint
		{
			e->setHealth(e->getHealth()-1);
				
			if (e->getHealth() <=0)
			{
				ParticleFireworks* bloodSpray = ParticleFireworks::create();
				bloodSpray->setPositionType(kCCPositionTypeGrouped);
				bloodSpray->setStartColor(Color4F(100.0/255.0f, 30.0/255.0f, 30.0/255.0f, 1.0f));
				bloodSpray->setEndColor(Color4F(55.0/255.0f, 12.0/255.0f, 12.0/255.0f, 1.0f));
				bloodSpray->setStartColorVar(Color4F(30.0/255.0f,0.0f,0.0f,0.0f));
				bloodSpray->setEndColorVar(Color4F(0.0f,0.0f,0.0f,0.0f));
				bloodSpray->setLife(3.0f); 
				bloodSpray->setStartSizeVar(18.0f);
				bloodSpray->setDuration(4.0f);
				bloodSpray->setTotalParticles(750);
	
				bloodSpray->setPosition(Vec2(e->getPositionX()+25, e->getPositionY()+96));
				this->addChild(bloodSpray,1);
				
				this->removeChild(e);
				enemies.erase(enemies.begin()+i);
				break;
			}
		}	
	}
}

bool Gameplay::onTouchBegan(Touch* touch, Event *event) {
	log("touch began");
	//std::cout << std::string("touch began") << "\n";
	
	//label1->setString("touch began");
	
	Point touchLocation = touch->getLocationInView();
	
	screenHeld = true;
	
	if (getTimeTick() - lastSlowFocus > 2500)
	{
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0.04f);
	
		lastSlowFocus = getTimeTick();
	}
	
	return true;
}

void Gameplay::onTouchMoved(Touch* touch, Event *event) {
	log("touch moved");
}

void Gameplay::onTouchEnded(Touch* touch, Event *event) {
	log("touch ended");
	
	//label1->setString("touch ended");
	
	screenHeld = false;
	
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
	//Director::getInstance()->getScheduler()->setTimeScale(1);
	
	playerPhysics->setVelocity(Vec2(0, 0));	
	
	if (movingRight)
	{
		impulse.x = 900000 * cos(CC_DEGREES_TO_RADIANS(-arrowR->getRotation()));
		impulse.y = 900000 * sin(CC_DEGREES_TO_RADIANS(-arrowR->getRotation()));
	}
	else
	{
		impulse.x = 900000 * cos(CC_DEGREES_TO_RADIANS(-arrowL->getRotation()));
		impulse.y = 900000 * sin(CC_DEGREES_TO_RADIANS(-arrowL->getRotation()));
	}	
	
	playerPhysics->applyImpulse(impulse);		//impulse = immediate max speed, 	force = gradual build
	
	arrowR->setVisible(false);
	arrowL->setVisible(false);
}


void Gameplay::populateMapPhysics(TMXTiledMap* newMap)
{
	auto wallLayer = newMap->layerNamed("Walls");			//note: no layer can have 0 tiles on it, or the game will crash
	auto floorLayer = newMap->layerNamed("Floors");
	
	for	(int x=0; x < 40; x++)			//width of map, in tiles
	{
		for (int y = 0; y < 12; y++)	//height of map, in tiles
		{
			auto spriteTileW = wallLayer->getTileAt(Vec2(x,y));
			
			if (spriteTileW != NULL)
			{
				//this->addChild(spriteTileW);
				auto blackSprite = Sprite::create("black.png");
				blackSprite->setPosition(spriteTileW->getPosition());
				this->addChild(blackSprite);
				
				auto tilePhysics = PhysicsBody::createBox(Size(60.0f, 60.0f), PhysicsMaterial(0.7f, 1.0f, 0.0f));	//density, friction, bounciness 
				tilePhysics->setDynamic(false);
				spriteTileW->setPhysicsBody(tilePhysics);
				
				walls.push_back(blackSprite);
				
				tileCount++;
			}
			
			auto spriteTileF = floorLayer->getTileAt(Vec2(x,y));
			if (spriteTileF != NULL)
			{
				//this->addChild(spriteTileF);
				auto tilePhysics = PhysicsBody::createBox(Size(240.0f, 60.0f), PhysicsMaterial(0.7f, 1.0f, 0.0f));	//density, friction, bounciness 
				tilePhysics->setDynamic(false);
				spriteTileF->setPhysicsBody(tilePhysics);
				
				//walls.push_back(spriteTileF);
				
				tileCount++;
			}
		}
	}	
}

//MAP SCROLLING AND HANDLING
void Gameplay::scrollMaps(Vec2 playerPosDifference)
{
	for (int i = 0; i< maps.size(); i ++)
	{
		TMXTiledMap* m = maps[i];
		m->setPositionX(m->getPositionX() + playerPosDifference.x);	
		
		//add new maps
		/*
	    if (maps.back()->getPosition().x < -1120) 	//the 2400px width map is still fully covering the screen at x-pos: -1120
		{
			TMXTiledMap* newMap = TMXTiledMap::create("tilemaps/map0.tmx");		
			newMap->setPosition(Vec2(player->getPosition().x,0)); 		//add a new map just off-screen to the right
			this->addChild(newMap, 2);
			maps.push_back(newMap);
			
			populateMapPhysics(newMap);	
		}
		*/
		if (m->getPosition().x < -2400 )	//if any map is completely invisble to the left
		{
			this->removeChild(m);			//delete it from the screen
			maps.erase(maps.begin()+i);		//and the vector of maps
			break;
		}
	}
}


//helper method to keep angles between 0 and 360 for clarity
float Gameplay::normaliseAngle(float theta)
{
    theta = fmod(theta,360);
    if (theta < 0)
        theta += 360;
    return theta;
}

//helper method keeping track of time elapsed (since e.g. starting the game)
unsigned long Gameplay::getTimeTick() {     //used to return float
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return millisecs;    //used to cast it to float here
}