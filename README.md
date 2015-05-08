# 2048 game by cocos2dx-3.5

  this is a 2048 game wrote by cocos2dx 3.5 engine.  

## Basic Info

**language:** cpp
**dev  ide:** vs2012 + eclipse
**engine:** cocos2dx 3.5
**说明：**在进行测试的时候，建议仅加载class中的文件，然后自己通过cocos命令新建一个工程，然后将class中的文件加载如工程中即可。创建工程命令如下:
    `cocos new 2048 -p org.test.2048 -l cpp -d D:\2048`

游戏已经完成，本工程中有两个分支，其中master分支对应的是下面博客中讲述的内容；andr分支则是后来将工程移植到android项目中时，对项目进行的修改和完善。后面会说明新增和修改的说明

## 2048 开发笔记博客地址

1.[cocos2dx-v3.5 2048 (一）: 项目架构](http://my.oschina.net/u/566591/blog/407393)
2.[cocos2dx-v3.5 2048 (二）: GameTool的设计与实现](http://my.oschina.net/u/566591/blog/407468)
3.[cocos2dx-v3.5 2048（三）:菜单实现](http://my.oschina.net/u/566591/blog/408135)
4.[cocos2dx-v3.4 2048（四）：单元格的设计与实现](http://my.oschina.net/u/566591/blog/410428)
5.[cocos2dx-v3.4 2048（四）：游戏逻辑的设计与实现](http://my.oschina.net/u/566591/blog/411001)
以上几篇博客使用的代码为 master分支的内容  

## 移植android工程

### 1. 新建工程

执行命令 `cd proj.android; python build_native.py`编译Class中的cpp文件
在使用cocos2dx 3.5的命令创建工程时，执行命令会出现问题（如有成功的请不吝赐教）因此我选择cocos2dx 3.3命令创建的项目，与3.5对比，其中包含了cocos2d文件夹，里面包含所用到的源码
接着修改jni文件夹下的Android.mk中的内容，将新增的文件包含进  

    `<span class="hljs-string">LOCAL_PATH :</span>= $(call my-dir)
    include $(CLEAR_VARS)

    $(call <span class="hljs-keyword">import</span>-add-path,$(LOCAL_PATH)<span class="hljs-regexp">/../</span>../cocos2d)
    $(call <span class="hljs-keyword">import</span>-add-path,$(LOCAL_PATH)<span class="hljs-regexp">/../</span>..<span class="hljs-regexp">/cocos2d/</span>external)
    $(call <span class="hljs-keyword">import</span>-add-path,$(LOCAL_PATH)<span class="hljs-regexp">/../</span>..<span class="hljs-regexp">/cocos2d/</span>cocos)

    <span class="hljs-string">LOCAL_MODULE :</span>= cocos2dcpp_shared

    <span class="hljs-string">LOCAL_MODULE_FILENAME :</span>= libcocos2dcpp

    <span class="hljs-string">LOCAL_SRC_FILES :</span>= hellocpp/main.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/AppDelegate.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/DataConf.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/GameLayer.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/GameMenuLayer.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/GameScene.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/GameTool.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/Grid.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/MenuButton.cpp \
                       ..<span class="hljs-regexp">/../</span>Classes/SetMenu.cpp

    <span class="hljs-string">LOCAL_C_INCLUDES :</span>= $(LOCAL_PATH)<span class="hljs-regexp">/../</span>../Classes

    <span class="hljs-string">LOCAL_STATIC_LIBRARIES :</span>= cocos2dx_static

    include $(BUILD_SHARED_LIBRARY)

    $(call <span class="hljs-keyword">import</span>-module,.)`
    `
    `</pre>

    **[如果上面有问题欢迎联系我](http://my.oschina.net/u/566591)**    

    ### 2.屏幕适配

    实际在android设备上运行项目后，首先出现的问题就是屏幕适配，因此需要修改AppDelegate.cpp中的相关代码，这里可以参考 **[Cocos2dx-3.x 学习笔记一：屏幕分辨率及适配问题](http://blog.csdn.net/liuyueyi25/article/details/45083057)**, 本项目的代码**andr分支**中也有相关实现.

    <pre>`<span class="hljs-keyword">bool</span> AppDelegate::applicationDidFinishLaunching() {
        <span class="hljs-comment">// initialize director</span>
        <span class="hljs-keyword">auto</span> director = Director::getInstance();
        <span class="hljs-keyword">auto</span> glview = director->getOpenGLView();
        <span class="hljs-keyword">if</span>(!glview) {
    <span class="hljs-preprocessor">#<span class="hljs-keyword">if</span> (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)</span>
            glview = GLViewImpl::create(<span class="hljs-string">"2048"</span>);
    <span class="hljs-preprocessor">#<span class="hljs-keyword">else</span></span>
            glview = GLViewImpl::createWithRect(<span class="hljs-string">"2048"</span>, Rect(<span class="hljs-number">0</span>, <span class="hljs-number">0</span>,<span class="hljs-number">320</span>, <span class="hljs-number">480</span>));
    <span class="hljs-preprocessor">#<span class="hljs-keyword">endif</span></span>
            director->setOpenGLView(glview);
        }
    <span class="hljs-preprocessor">#<span class="hljs-keyword">if</span> (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)</span>
        glview->setFrameSize(<span class="hljs-number">320</span>,<span class="hljs-number">480</span>); <span class="hljs-comment">// set the device's size</span>
    <span class="hljs-preprocessor">#<span class="hljs-keyword">endif</span></span>

        <span class="hljs-keyword">auto</span> winSize = director->getWinSize();
        <span class="hljs-keyword">float</span> deviceWidth = winSize.width;
        <span class="hljs-keyword">float</span> deviceHeight = winSize.height;
        <span class="hljs-keyword">float</span> designWidth = <span class="hljs-number">320</span>;
        <span class="hljs-keyword">float</span> designHeight = <span class="hljs-number">480</span>;

        <span class="hljs-keyword">float</span> scaleX = deviceWidth / designWidth;
        <span class="hljs-keyword">float</span> scaleY = deviceHeight / designHeight;
        <span class="hljs-keyword">if</span>(scaleX < scaleY)
            designHeight = deviceHeight / scaleX;
        <span class="hljs-keyword">else</span>
            designWidth = deviceWidth / scaleY;
        director->getOpenGLView()->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::SHOW_ALL);

        <span class="hljs-comment">// turn on display FPS</span>
        director->setDisplayStats(<span class="hljs-keyword">true</span>);

        <span class="hljs-comment">// set FPS. the default value is 1.0/60 if you don't call this</span>
        director->setAnimationInterval(<span class="hljs-number">1.0</span> / <span class="hljs-number">60</span>);

        FileUtils::getInstance()->addSearchPath(<span class="hljs-string">"res"</span>);

        <span class="hljs-comment">// add audio</span>
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(<span class="hljs-string">"move.mp3"</span>);
        <span class="hljs-comment">//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("merge.mp3");</span>

        <span class="hljs-comment">// create a scene. it's an autorelease object</span>
        <span class="hljs-comment">//auto scene = HelloWorld::createScene();</span>
        <span class="hljs-keyword">auto</span> scene = GameScene::createScene();

        <span class="hljs-comment">// run</span>
        director->runWithScene(scene);

        <span class="hljs-keyword">return</span> <span class="hljs-keyword">true</span>;
    }
    `</pre>

    ### 3.中文显示

    将原先的代码移植到android工程时，之前的转码函数无法正常运行，因此对于如何正常的友好的显示中文又重新成了个问题，转码神马的挺麻烦的，因此采用了从UTF-8 no BOM格式保存的文件中读取数据然后加载在代码中
    此处我们采用的是从json文件中获取数据，然后显示.
    同样在**andr分支**中也有相关实现

    ### 4.增加声音和显示特效等

    相对于**master**中的版本，andr分支中增加了声音的支持，并且在SetMenu中区分了当前模式的字体颜色，使用户更加方便的确定正在玩耍的是哪个模式的游戏。在_SetMenu.cpp_中可以查看相应的实现

    ### 5.android项目连续两次退出按钮才退出游戏的实现

    在android项目中，常见的退出是连续按两次的退出或者按退出的时候，弹出一个确认对话框，对于这一块可以参考**[Cocos2d-x android使用onKeyDown监听返回键实现二次返回退出](http://50vip.com/390.html)**
    本处也给与了实现，请参考文件**[AppActivity.java](https://github.com/liuyueyi/2048/blob/master/proj.android/src/org/cocos2dx/cpp/AppActivity.java)**,重写了**onKeyDown**函数  

    <pre>`<span class="hljs-keyword">private</span> <span class="hljs-keyword">long</span> exitTime = <span class="hljs-number">0</span>;
        <span class="hljs-annotation">@Override</span>
        <span class="hljs-function"><span class="hljs-keyword">public</span> <span class="hljs-keyword">boolean</span> <span class="hljs-title">onKeyDown</span><span class="hljs-params">(<span class="hljs-keyword">int</span> keyCode, KeyEvent event)</span></span>{
            <span class="hljs-keyword">if</span>(keyCode == KeyEvent.KEYCODE_BACK){
                <span class="hljs-keyword">if</span>((System.currentTimeMillis() - exitTime) > <span class="hljs-number">2000</span>){
                    exitTime = System.currentTimeMillis();
                    Toast.makeText(<span class="hljs-keyword">this</span>,  <span class="hljs-string">"click agian to exit the game"</span>, Toast.LENGTH_LONG).show();
                    android.util.Log.d(<span class="hljs-string">"wzb"</span>, <span class="hljs-string">"key keyk "</span>);
                    <span class="hljs-keyword">return</span> <span class="hljs-keyword">false</span>;
                } <span class="hljs-keyword">else</span>{
                    Log.d(<span class="hljs-string">"wzb"</span>, <span class="hljs-string">"androi dexit game "</span>);
                    finish();
                    System.exit(<span class="hljs-number">0</span>);
                }
            }

            <span class="hljs-keyword">return</span> <span class="hljs-keyword">super</span>.onKeyDown(keyCode, event);
        }
    `</pre>

    值得注意的是，这里我们得修改libcocosd中包下的**Cocos2dxGLSurfaceView.java**的相关代码，更新后的代码如下：

    <pre>`<span class="hljs-annotation">@Override</span>
    <span class="hljs-function"><span class="hljs-keyword">public</span> <span class="hljs-keyword">boolean</span> <span class="hljs-title">onKeyDown</span><span class="hljs-params">(<span class="hljs-keyword">final</span> <span class="hljs-keyword">int</span> pKeyCode, <span class="hljs-keyword">final</span> KeyEvent pKeyEvent)</span> </span>{
            <span class="hljs-keyword">switch</span> (pKeyCode) {
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_BACK:
                    Cocos2dxVideoHelper.mVideoHandler.sendEmptyMessage(Cocos2dxVideoHelper.KeyEventBack);
                    <span class="hljs-keyword">this</span>.queueEvent(<span class="hljs-keyword">new</span> Runnable() {
                        <span class="hljs-annotation">@Override</span>
                        <span class="hljs-function"><span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">run</span><span class="hljs-params">()</span> </span>{
                            Cocos2dxGLSurfaceView.<span class="hljs-keyword">this</span>.mCocos2dxRenderer.handleKeyDown(pKeyCode);
                        }
                    });
                    <span class="hljs-keyword">return</span> <span class="hljs-keyword">false</span>;
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_MENU:
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_DPAD_LEFT:
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_DPAD_RIGHT:
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_DPAD_UP:
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_DPAD_DOWN:
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_ENTER:
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
                <span class="hljs-keyword">case</span> KeyEvent.KEYCODE_DPAD_CENTER:
                    <span class="hljs-keyword">this</span>.queueEvent(<span class="hljs-keyword">new</span> Runnable() {
                        <span class="hljs-annotation">@Override</span>
                        <span class="hljs-function"><span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">run</span><span class="hljs-params">()</span> </span>{
                            Cocos2dxGLSurfaceView.<span class="hljs-keyword">this</span>.mCocos2dxRenderer.handleKeyDown(pKeyCode);
                        }
                    });
                    <span class="hljs-keyword">return</span> <span class="hljs-keyword">true</span>;
                <span class="hljs-keyword">default</span>:
                    <span class="hljs-keyword">return</span> <span class="hljs-keyword">super</span>.onKeyDown(pKeyCode, pKeyEvent);
            }
        }
    