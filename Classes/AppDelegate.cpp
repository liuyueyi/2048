#include "AppDelegate.h"
#include "GameScene.h"
#include "DataConf.h"
#include "Grid.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

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

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		glview = GLViewImpl::create("2048");
#else
		glview = GLViewImpl::createWithRect("2048", Rect(0, 0,320, 480));
#endif
		director->setOpenGLView(glview);
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	glview->setFrameSize(320,480); // set the device's size
#endif

	auto winSize = director->getWinSize();
	float deviceWidth = winSize.width;
	float deviceHeight = winSize.height;
	float designWidth = 320;
	float designHeight = 480;
	
	float scaleX = deviceWidth / designWidth;
	float scaleY = deviceHeight / designHeight;
	if(scaleX < scaleY)
		designHeight = deviceHeight / scaleX;
	else
		designWidth = deviceWidth / scaleY;
	director->getOpenGLView()->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::SHOW_ALL);

	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	FileUtils::getInstance()->addSearchPath("res");

	// add audio
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("move.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("merge.mp3");

	// create a scene. it's an autorelease object
	//auto scene = HelloWorld::createScene();
	auto scene = GameScene::createScene();

	// run
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	log("exit and save--->");
	DataConf::getInstance()->dumpData(Grid::getType());

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
