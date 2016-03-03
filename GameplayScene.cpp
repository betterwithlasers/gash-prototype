#include "GameplayScene.h"

#define PTM_RATIO 32.0	

USING_NS_CC;

Scene* Gameplay::createScene()
{
    // 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();	//as opposed to Scene::create();
	
	auto physicsWorld = scene->getPhysicsWorld();
	physicsWorld->setGravity(Vec2(0.0f,-250.0f)); 	//default is 0, -98
    
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
	
	tileMap = TMXTiledMap::create("tilemaps/mapX.tmx");		
	auto wallLayer = tileMap->layerNamed("Walls");			//note: no layer can have 0 tiles on it, or the game will crash
	auto floorLayer = tileMap->layerNamed("Floors");
	//auto mapSize = tileMap->getMapSize();
	tileMap->setPosition(Vec2(0,0)); 		//note: the map does not cover the top and bottom 60 pixels, which are at the moment fully covered in floor/ceiling
    this->addChild(tileMap, 2);
	
	tileCount = 0;
	
	for	(int x=0; x < 40; x++)			//width of map, in tiles
	{
		for (int y = 0; y < 12; y++)	//height of map, in tiles
		{
			auto spriteTileW = wallLayer->getTileAt(Vec2(x,y));
			if (spriteTileW != NULL)
			{
				auto tilePhysics = PhysicsBody::createBox(Size(60.0f, 60.0f), PhysicsMaterial(0.7f, 1.0f, 0.0f));	//density, friction, bounciness 
				tilePhysics->setDynamic(false);
				spriteTileW->setPhysicsBody(tilePhysics);
				
				walls.push_back(spriteTileW);
				
				tileCount++;
			}
			
			auto spriteTileF = floorLayer->getTileAt(Vec2(x,y));
			if (spriteTileF != NULL)
			{
				auto tilePhysics = PhysicsBody::createBox(Size(240.0f, 60.0f), PhysicsMaterial(0.7f, 1.0f, 0.0f));	//density, friction, bounciness 
				tilePhysics->setDynamic(false);
				spriteTileF->setPhysicsBody(tilePhysics);
				
				walls.push_back(spriteTileF);
				
				tileCount++;
			}
		}
	}

	
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
	
	playerRight = Sprite::create("collideRight.png");
	playerRight->setAnchorPoint(Vec2(0,0));
	playerRight->setPosition(Vec2(player->getPosition().x + 60, player->getPosition().y +11));
	this->addChild(playerRight,4);
	
	playerRight->setVisible(false);
	
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
	
	/*
	Wall* wall1 = Wall::create("wallV.png");
	wall1->setAnchorPoint(Vec2(0,0));
	wall1->setPosition(Vec2(980, 72));
	this->addChild(wall1, 2);
	walls.push_back(wall1);
	
	PhysicsBody* wallPhysics = PhysicsBody::createBox(Size(50.0f, 250.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	wallPhysics->setDynamic(false);
	wall1->setPhysicsBody(wallPhysics);
	
	Wall* wall2 = Wall::create("wallH.png");
	wall2->setAnchorPoint(Vec2(0,0));
	wall2->setPosition(Vec2(500, 500));
	this->addChild(wall2, 2);
	walls.push_back(wall2);
	
	PhysicsBody* wallPhysics2 = PhysicsBody::createBox(Size(250.0f, 50.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	wallPhysics2->setDynamic(false);
	wall2->setPhysicsBody(wallPhysics2);
	
	addDummyWalls();
	
	*/
	
	/*
	Wall* floor = Wall::create("floor.png");
	floor->setAnchorPoint(Vec2(0,0));
	floor->setPosition(Vec2(0, 0));
	floor->setTag(77);
	this->addChild(floor, 1);
	walls.push_back(floor);
	
	Wall* ceiling = Wall::create("floor.png");
	ceiling->setAnchorPoint(Vec2(0,0));
	ceiling->setPosition(Vec2(0, 660));
	ceiling->setTag(78);
	this->addChild(ceiling, 1);
	walls.push_back(ceiling);
	
	PhysicsBody* floorPhysics = PhysicsBody::createBox(Size(1280.0f, 60.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	floorPhysics->setDynamic(false);
	floor->setPhysicsBody(floorPhysics);
	
	PhysicsBody* ceilingPhysics = PhysicsBody::createBox(Size(1280.0f, 60.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	ceilingPhysics->setDynamic(false);
	ceiling->setPhysicsBody(ceilingPhysics);
	

	
	//DELETE BELOW SECTION
	
	Wall* floor2 = Wall::create("floor.png");
	floor2->setAnchorPoint(Vec2(0,0));
	floor2->setPosition(Vec2(1280, 0));
	floor2->setTag(77);
	this->addChild(floor2, 1);
	walls.push_back(floor2);
	floor->setVisible(false);
	
	Wall* ceiling2 = Wall::create("floor.png");
	ceiling2->setAnchorPoint(Vec2(0,0));
	ceiling2->setPosition(Vec2(1280, 660));
	ceiling2->setTag(78);
	this->addChild(ceiling2, 1);
	walls.push_back(ceiling2);
	ceiling->setVisible(false);	
	
	PhysicsBody* floorPhysics2 = PhysicsBody::createBox(Size(1280.0f, 60.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	floorPhysics2->setDynamic(false);
	floor2->setPhysicsBody(floorPhysics2);
	
	PhysicsBody* ceilingPhysics2 = PhysicsBody::createBox(Size(1280.0f, 60.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	ceilingPhysics2->setDynamic(false);
	ceiling2->setPhysicsBody(ceilingPhysics2);
	
	//END OF SECTION TO DELETE
	*/
	
	//this->runAction(Follow::create(player));
	
	//CustomFollow* scrolling = CustomFollow::create(player, Rect(0, 0, visibleSize.width * 100, visibleSize.height));	//scrolling is limited within this rectangle
	//scrolling->setSpriteParent(this);	//an important link (theres probably a 'cleaner' way to do this)
	//this->runAction(scrolling);
	
	Rect followRect = Rect(0, 0, visibleSize.width * 100, visibleSize.height);
	this->runAction(Follow::create(player, followRect));
	
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
	collidePlayerWalls();
	
	playerParticle->setPosition(Vec2(player->getPositionX()+36, player->getPositionY()+36));
	trailParticle->setPosition(Vec2(player->getPositionX()+36, player->getPositionY()+36));
	
	arrowR->setPosition(Vec2(player->getPositionX()+36, player->getPositionY()+36));
	arrowL->setPosition(Vec2(player->getPositionX()+36, player->getPositionY()+36));
	
	playerRight->setPosition(Vec2(player->getPosition().x + 60, player->getPosition().y +11));
	
	
	Vec2 playerPosDifference = lastPlayerPos - player->getPosition();
	
	tileMap->setPositionX(tileMap->getPositionX() + playerPosDifference.x);	

	for (int i = 0 ; i <walls.size(); i++)
	{
		Sprite* w = walls[i];
		//if (w->getTag() != 77 && w->getTag() != 78)		//every wall except the floor
		//{	w->setPositionX(w->getPositionX() + playerPosDifference.x);	 }	 //moves the same amount as the player, but in opposite direction
		w->setPositionX(w->getPositionX() + playerPosDifference.x);	
	}
	
	
	char text[256];
	sprintf(text, "Tile count: %d", tileCount); 
	label1->setString(text);
	
	char text2[256];
	sprintf(text2, "x force: %f", 1000 * cos(CC_DEGREES_TO_RADIANS(-arrowR->getRotation()))); 
	label2->setString(text2);
	
	char text3[256];
	sprintf(text3, "y force: %f", 1000 * sin(CC_DEGREES_TO_RADIANS(-arrowR->getRotation()))); 
	label3->setString(text3);
	
	lastPlayerPos = player->getPosition();
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

void Gameplay::collidePlayerWalls()
{
	Rect playerBox = player->getBoundingBox();
	Rect playerRightBox = playerRight->getBoundingBox();
	
	movingRight = true;
	
	for (int j = 0; j<walls.size();j++)
	{
		Sprite* w = walls[j];
		Rect wallBox = w->getBoundingBox();
			
		//if (playerBox.intersectsRect(wallBox) && screenHeld)  
		//{
					//or just 0
			//Director::getInstance()->getScheduler()->setTimeScale(0.30f);			NOT WORKABLE YET, SLOWS DOWN ROTATION OF DIRECTIONAL ARROW
			
			if (wallBox.intersectsRect(playerRightBox))
			{	
				movingRight = false;	
				arrowL->setVisible(true);	
				arrowR->setVisible(false);	
			}
		//}	
	}
}

bool Gameplay::onTouchBegan(Touch* touch, Event *event) {
	log("touch began");
	//std::cout << std::string("touch began") << "\n";
	
	//label1->setString("touch began");
	
	Point touchLocation = touch->getLocationInView();
	
	screenHeld = true;
	
	if (getTimeTick() - lastSlowFocus > 5000)
	{
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0.04f);
	
	lastSlowFocus = getTimeTick();
	}
	
	//arrowR->setVisible(true);
	
	if (movingRight)
	{	arrowR->setVisible(true); 
		arrowL->setVisible(false);	}		
		//possibly add the else condition here
	
	return true;
}

void Gameplay::onTouchMoved(Touch* touch, Event *event) {
	log("touch moved");
}

void Gameplay::addDummyWalls()
{
	Wall* wall3 = Wall::create("wallV.png");
	wall3->setAnchorPoint(Vec2(0,0));
	wall3->setPosition(Vec2(2260, 400));
	this->addChild(wall3, 2);
	walls.push_back(wall3);
	
	PhysicsBody* wallPhysics3 = PhysicsBody::createBox(Size(50.0f, 250.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	wallPhysics3->setDynamic(false);
	wall3->setPhysicsBody(wallPhysics3);
	
	Wall* wall4 = Wall::create("wallH.png");
	wall4->setAnchorPoint(Vec2(0,0));
	wall4->setPosition(Vec2(1580, 200));
	this->addChild(wall4, 2);
	walls.push_back(wall4);
	
	PhysicsBody* wallPhysics4 = PhysicsBody::createBox(Size(250.0f, 50.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	wallPhysics4->setDynamic(false);
	wall4->setPhysicsBody(wallPhysics4);
	
	
	Wall* wall5 = Wall::create("wallV.png");
	wall5->setAnchorPoint(Vec2(0,0));
	wall5->setPosition(Vec2(3540, 72));
	this->addChild(wall5, 2);
	walls.push_back(wall5);
	
	PhysicsBody* wallPhysics5 = PhysicsBody::createBox(Size(50.0f, 250.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	wallPhysics5->setDynamic(false);
	wall5->setPhysicsBody(wallPhysics5);
	
	Wall* wall6 = Wall::create("wallH.png");
	wall6->setAnchorPoint(Vec2(0,0));
	wall6->setPosition(Vec2(2860, 330));
	this->addChild(wall6, 2);
	walls.push_back(wall6);
	
	PhysicsBody* wallPhysics6 = PhysicsBody::createBox(Size(250.0f, 50.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));	//density, friction, bounciness 
	wallPhysics6->setDynamic(false);
	wall6->setPhysicsBody(wallPhysics6);
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