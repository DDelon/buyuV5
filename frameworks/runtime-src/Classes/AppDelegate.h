#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__
#ifdef WIN32
#pragma warning(push,3)
#include "cocos2d.h"
#pragma warning(push,4)
#else
#include "cocos2d.h"
#endif

//0表示不打日志，	1.表示打日志
//const int LUA_DEBUG = 1;

#define  LUA_DEBUG 0

/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/


class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  Called when the application moves to the background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  Called when the application reenters the foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

	void loadFishConfig();
    
    static void setSocketPause(bool isPause);

	static void createFile();
};

#endif  // __APP_DELEGATE_H__

