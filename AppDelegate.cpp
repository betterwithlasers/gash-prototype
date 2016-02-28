#include "AppDelegate.h"
#include "GameplayScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);		//what art asset is made relatively to, the default is 480, 320 (way too small)
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1920, 1080);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Prototype", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Prototype");
#endif
        director->setOpenGLView(glview);
    }
	
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	

    // Set the design resolution																	
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT); 	//alternatives are fixed_width, which is better for portrait games
    Size frameSize = glview->getFrameSize();	//the device's screen resolution
    
	director->setContentScaleFactor(mediumResolutionSize.height/designResolutionSize.height);		//this line is the ratio of max resource height (e.g. a background) to your design resolution
																									//here it is clearly 1/1, since all assets are made specially for the 720p design resolution
	
	/*		OPTIONAL SECTION IF DIFFERENT SIZE ASSETS FOR DIFFERENT RESOLUTIONS ARE BUILT -> where content scaling factor is actually important
	if (frameSize.height >= largeResolutionSize.height)		//1080p and beyond (modern phones + tablets)
    {    
        director->setContentScaleFactor(largeResolutionSize.height/designResolutionSize.height);
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height >= mediumResolutionSize.height)		//all 2012+ phones, 
    {        
        director->setContentScaleFactor(mediumResolutionSize.height/designResolutionSize.height);
    }
    // if the frame's height is smaller than the height of medium size.
    else	//non HD phones
    {        
       director->setContentScaleFactor(smallResolutionSize.height/designResolutionSize.height);
    }
	*/

	
    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = Gameplay::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
