2048 game by cocos2dx-3.5
========
  this is a 2048 game wrote by cocos2dx 3.5 engine.  

Basic Info
---
**language:** cpp  
**dev  ide:** vs2012 + eclipse  
**engine:** cocos2dx 3.5  
**说明：**在进行测试的时候，建议仅加载class中的文件，然后自己通过cocos命令新建一个工程，然后将class中的文件加载如工程中即可。创建工程命令如下:  
    `cocos new 2048 -p org.test.2048 -l cpp -d D:\2048`

游戏已经完成，本工程中有两个分支，其中master分支对应的是下面博客中讲述的内容；andr分支则是后来将工程移植到android项目中时，对项目进行的修改和完善。后面会说明新增和修改的说明



2048 开发笔记博客地址
---
1.[cocos2dx-v3.5 2048 (一）: 项目架构](http://my.oschina.net/u/566591/blog/407393)  
2.[cocos2dx-v3.5 2048 (二）: GameTool的设计与实现](http://my.oschina.net/u/566591/blog/407468)  
3.[cocos2dx-v3.5 2048（三）:菜单实现](http://my.oschina.net/u/566591/blog/408135)  
4.[cocos2dx-v3.4 2048（四）：单元格的设计与实现](http://my.oschina.net/u/566591/blog/410428)  
5.[cocos2dx-v3.4 2048（四）：游戏逻辑的设计与实现](http://my.oschina.net/u/566591/blog/411001)  
以上几篇博客使用的代码为 master分支的内容  



移植android工程
---
###1. 新建工程
执行命令 `cd proj.android; python build_native.py`编译Class中的cpp文件  
在使用cocos2dx 3.5的命令创建工程时，执行命令会出现问题（如有成功的请不吝赐教）因此我选择cocos2dx 3.3命令创建的项目，与3.5对比，其中包含了cocos2d文件夹，里面包含所用到的源码  
接着修改jni文件夹下的Android.mk中的内容，将新增的文件包含进  

    `LOCAL_PATH := $(call my-dir)
    include $(CLEAR_VARS)
    
    $(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
    $(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
    $(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
    
    LOCAL_MODULE := cocos2dcpp_shared
    
    LOCAL_MODULE_FILENAME := libcocos2dcpp
    
    LOCAL_SRC_FILES := hellocpp/main.cpp \
                       ../../Classes/AppDelegate.cpp \
                       ../../Classes/DataConf.cpp \
    				   ../../Classes/GameLayer.cpp \
    				   ../../Classes/GameMenuLayer.cpp \
    				   ../../Classes/GameScene.cpp \
    				   ../../Classes/GameTool.cpp \
    				   ../../Classes/Grid.cpp \
    				   ../../Classes/MenuButton.cpp \
    				   ../../Classes/SetMenu.cpp
    
    LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
    
    LOCAL_STATIC_LIBRARIES := cocos2dx_static
    
    include $(BUILD_SHARED_LIBRARY)
    
    $(call import-module,.)`
    `
   
**[如果上面有问题欢迎联系我](http://my.oschina.net/u/566591)**    

###2.屏幕适配
实际在android设备上运行项目后，首先出现的问题就是屏幕适配，因此需要修改AppDelegate.cpp中的相关代码，这里可以参考 **[Cocos2dx-3.x 学习笔记一：屏幕分辨率及适配问题](http://blog.csdn.net/liuyueyi25/article/details/45083057)**, 本项目的代码**andr分支**中也有相关实现.
```cpp
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
```

###3.中文显示
将原先的代码移植到android工程时，之前的转码函数无法正常运行，因此对于如何正常的友好的显示中文又重新成了个问题，转码神马的挺麻烦的，因此采用了从UTF-8 no BOM格式保存的文件中读取数据然后加载在代码中  
此处我们采用的是从json文件中获取数据，然后显示.	
同样在**andr分支**中也有相关实现

###4.增加声音和显示特效等
相对于**master**中的版本，andr分支中增加了声音的支持，并且在SetMenu中区分了当前模式的字体颜色，使用户更加方便的确定正在玩耍的是哪个模式的游戏。在*SetMenu.cpp*中可以查看相应的实现

###5.android项目连续两次退出按钮才退出游戏的实现
在android项目中，常见的退出是连续按两次的退出或者按退出的时候，弹出一个确认对话框，对于这一块可以参考**[Cocos2d-x android使用onKeyDown监听返回键实现二次返回退出](http://50vip.com/390.html)**  
本处也给与了实现，请参考文件**[AppActivity.java](https://github.com/liuyueyi/2048/blob/master/proj.android/src/org/cocos2dx/cpp/AppActivity.java)**,重写了**onKeyDown**函数  
```java
private long exitTime = 0;
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event){
		if(keyCode == KeyEvent.KEYCODE_BACK){
			if((System.currentTimeMillis() - exitTime) > 2000){
				exitTime = System.currentTimeMillis();
				Toast.makeText(this,  "click agian to exit the game", Toast.LENGTH_LONG).show();
				android.util.Log.d("wzb", "key keyk ");
				return false;
			} else{
				Log.d("wzb", "androi dexit game ");
				finish();
				System.exit(0);
			}
		}
		
		return super.onKeyDown(keyCode, event);
	}
```
值得注意的是，这里我们得修改libcocosd中包下的**Cocos2dxGLSurfaceView.java**的相关代码，更新后的代码如下：
```java
@Override
public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent) {
        switch (pKeyCode) {
            case KeyEvent.KEYCODE_BACK:
                Cocos2dxVideoHelper.mVideoHandler.sendEmptyMessage(Cocos2dxVideoHelper.KeyEventBack);
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleKeyDown(pKeyCode);
                    }
                });
                return false;
            case KeyEvent.KEYCODE_MENU:
            case KeyEvent.KEYCODE_DPAD_LEFT:
            case KeyEvent.KEYCODE_DPAD_RIGHT:
            case KeyEvent.KEYCODE_DPAD_UP:
            case KeyEvent.KEYCODE_DPAD_DOWN:
            case KeyEvent.KEYCODE_ENTER:
            case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
            case KeyEvent.KEYCODE_DPAD_CENTER:
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleKeyDown(pKeyCode);
                    }
                });
                return true;
            default:
                return super.onKeyDown(pKeyCode, pKeyEvent);
        }
    }

```
