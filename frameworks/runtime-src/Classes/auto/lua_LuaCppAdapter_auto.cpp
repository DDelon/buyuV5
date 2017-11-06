#include "lua_LuaCppAdapter_auto.hpp"
#include "LuaCppAdapter.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_LuaCppAdapter_LuaCppAdapter_getPlayerId(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_getPlayerId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getPlayerId'", nullptr);
            return 0;
        }
        int ret = cobj->getPlayerId();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:getPlayerId",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getPlayerId'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_changeTimeline(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_changeTimeline'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:changeTimeline");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_changeTimeline'", nullptr);
            return 0;
        }
        cobj->changeTimeline(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:changeTimeline",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_changeTimeline'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_playerFire(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_playerFire'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:playerFire");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_playerFire'", nullptr);
            return 0;
        }
        cobj->playerFire(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:playerFire",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_playerFire'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_startNets(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_startNets'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Layer* arg0;

        ok &= luaval_to_object<cocos2d::Layer>(tolua_S, 2, "cc.Layer",&arg0, "LuaCppAdapter:startNets");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_startNets'", nullptr);
            return 0;
        }
        cobj->startNets(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:startNets",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_startNets'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_setCannon(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_setCannon'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Node* arg0;
        int arg1;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0, "LuaCppAdapter:setCannon");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "LuaCppAdapter:setCannon");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_setCannon'", nullptr);
            return 0;
        }
        cobj->setCannon(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:setCannon",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_setCannon'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_openRankWeb(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_openRankWeb'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaCppAdapter:openRankWeb");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_openRankWeb'", nullptr);
            return 0;
        }
        cobj->openRankWeb(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:openRankWeb",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_openRankWeb'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_luaUseCppFun(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_luaUseCppFun'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:luaUseCppFun");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_luaUseCppFun'", nullptr);
            return 0;
        }
        cocos2d::ValueMap ret = cobj->luaUseCppFun(arg0);
        ccvaluemap_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:luaUseCppFun",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_luaUseCppFun'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_exitGame(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_exitGame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_exitGame'", nullptr);
            return 0;
        }
        cobj->exitGame();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:exitGame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_exitGame'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_addPlayer(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_addPlayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:addPlayer");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_addPlayer'", nullptr);
            return 0;
        }
        cobj->addPlayer(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:addPlayer",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_addPlayer'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_getPathPos(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_getPathPos'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:getPathPos");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "LuaCppAdapter:getPathPos");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getPathPos'", nullptr);
            return 0;
        }
        cocos2d::Vec3 ret = cobj->getPathPos(arg0, arg1);
        vec3_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:getPathPos",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getPathPos'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_callFish(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_callFish'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:callFish");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_callFish'", nullptr);
            return 0;
        }
        cobj->callFish(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:callFish",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_callFish'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_bindUI(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_bindUI'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::ValueMap arg0;
        cocos2d::ui::TextBMFont* arg1;
        cocos2d::ui::TextBMFont* arg2;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:bindUI");

        ok &= luaval_to_object<cocos2d::ui::TextBMFont>(tolua_S, 3, "ccui.TextBMFont",&arg1, "LuaCppAdapter:bindUI");

        ok &= luaval_to_object<cocos2d::ui::TextBMFont>(tolua_S, 4, "ccui.TextBMFont",&arg2, "LuaCppAdapter:bindUI");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_bindUI'", nullptr);
            return 0;
        }
        cobj->bindUI(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:bindUI",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_bindUI'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_callLuaFunc(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_callLuaFunc'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        cocos2d::ValueMap arg3;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaCppAdapter:callLuaFunc");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "LuaCppAdapter:callLuaFunc");

        ok &= luaval_to_std_string(tolua_S, 4,&arg2, "LuaCppAdapter:callLuaFunc");

        ok &= luaval_to_ccvaluemap(tolua_S, 5, &arg3, "LuaCppAdapter:callLuaFunc");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_callLuaFunc'", nullptr);
            return 0;
        }
        cobj->callLuaFunc(arg0, arg1, arg2, arg3);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:callLuaFunc",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_callLuaFunc'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_setLuaNode(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_setLuaNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        cocos2d::Node* arg1;
        cocos2d::ValueMap arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:setLuaNode");

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 3, "cc.Node",&arg1, "LuaCppAdapter:setLuaNode");

        ok &= luaval_to_ccvaluemap(tolua_S, 4, &arg2, "LuaCppAdapter:setLuaNode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_setLuaNode'", nullptr);
            return 0;
        }
        cobj->setLuaNode(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:setLuaNode",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_setLuaNode'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_getConfigData(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_getConfigData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaCppAdapter:getConfigData");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "LuaCppAdapter:getConfigData");

        ok &= luaval_to_std_string(tolua_S, 4,&arg2, "LuaCppAdapter:getConfigData");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getConfigData'", nullptr);
            return 0;
        }
        std::string ret = cobj->getConfigData(arg0, arg1, arg2);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:getConfigData",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getConfigData'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_loadDataBin(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_loadDataBin'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_loadDataBin'", nullptr);
            return 0;
        }
        cobj->loadDataBin();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:loadDataBin",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_loadDataBin'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_setFishState(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_setFishState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:setFishState");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_setFishState'", nullptr);
            return 0;
        }
        cobj->setFishState(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:setFishState",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_setFishState'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_getConfigDataByName(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_getConfigDataByName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaCppAdapter:getConfigDataByName");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getConfigDataByName'", nullptr);
            return 0;
        }
        cocos2d::ValueMap ret = cobj->getConfigDataByName(arg0);
        ccvaluemap_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:getConfigDataByName",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getConfigDataByName'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_removePlayerBullet(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_removePlayerBullet'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:removePlayerBullet");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_removePlayerBullet'", nullptr);
            return 0;
        }
        cobj->removePlayerBullet(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:removePlayerBullet",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_removePlayerBullet'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_getNBombKilledFishes(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_getNBombKilledFishes'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        cocos2d::Vec2 arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:getNBombKilledFishes");

        ok &= luaval_to_vec2(tolua_S, 3, &arg1, "LuaCppAdapter:getNBombKilledFishes");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getNBombKilledFishes'", nullptr);
            return 0;
        }
        cocos2d::ValueVector ret = cobj->getNBombKilledFishes(arg0, arg1);
        ccvaluevector_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:getNBombKilledFishes",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getNBombKilledFishes'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_getLuaValue(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_getLuaValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        cocos2d::Value arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaCppAdapter:getLuaValue");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "LuaCppAdapter:getLuaValue");

        ok &= luaval_to_ccvalue(tolua_S, 4, &arg2, "LuaCppAdapter:getLuaValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getLuaValue'", nullptr);
            return 0;
        }
        cobj->getLuaValue(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:getLuaValue",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getLuaValue'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_startGame(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_startGame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 5) 
    {
        cocos2d::Layer* arg0;
        cocos2d::Layer* arg1;
        cocos2d::ValueMap arg2;
        cocos2d::ValueVector arg3;
        cocos2d::ValueVector arg4;

        ok &= luaval_to_object<cocos2d::Layer>(tolua_S, 2, "cc.Layer",&arg0, "LuaCppAdapter:startGame");

        ok &= luaval_to_object<cocos2d::Layer>(tolua_S, 3, "cc.Layer",&arg1, "LuaCppAdapter:startGame");

        ok &= luaval_to_ccvaluemap(tolua_S, 4, &arg2, "LuaCppAdapter:startGame");

        ok &= luaval_to_ccvaluevector(tolua_S, 5, &arg3, "LuaCppAdapter:startGame");

        ok &= luaval_to_ccvaluevector(tolua_S, 6, &arg4, "LuaCppAdapter:startGame");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_startGame'", nullptr);
            return 0;
        }
        cobj->startGame(arg0, arg1, arg2, arg3, arg4);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:startGame",argc, 5);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_startGame'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_getVIPResId(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_getVIPResId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:getVIPResId");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getVIPResId'", nullptr);
            return 0;
        }
        std::string ret = cobj->getVIPResId(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:getVIPResId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getVIPResId'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_getCurFrame(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_getCurFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getCurFrame'", nullptr);
            return 0;
        }
        int ret = cobj->getCurFrame();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:getCurFrame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getCurFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_addDebugLayer(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_addDebugLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Layer* arg0;

        ok &= luaval_to_object<cocos2d::Layer>(tolua_S, 2, "cc.Layer",&arg0, "LuaCppAdapter:addDebugLayer");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_addDebugLayer'", nullptr);
            return 0;
        }
        cobj->addDebugLayer(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 2) 
    {
        cocos2d::Layer* arg0;
        std::string arg1;

        ok &= luaval_to_object<cocos2d::Layer>(tolua_S, 2, "cc.Layer",&arg0, "LuaCppAdapter:addDebugLayer");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "LuaCppAdapter:addDebugLayer");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_addDebugLayer'", nullptr);
            return 0;
        }
        cobj->addDebugLayer(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:addDebugLayer",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_addDebugLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_setGunRate(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_setGunRate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:setGunRate");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_setGunRate'", nullptr);
            return 0;
        }
        cobj->setGunRate(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:setGunRate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_setGunRate'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_otherPlayerBulletCollision(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_otherPlayerBulletCollision'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:otherPlayerBulletCollision");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_otherPlayerBulletCollision'", nullptr);
            return 0;
        }
        cobj->otherPlayerBulletCollision(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:otherPlayerBulletCollision",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_otherPlayerBulletCollision'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_updateNBombRate(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_updateNBombRate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:updateNBombRate");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_updateNBombRate'", nullptr);
            return 0;
        }
        cobj->updateNBombRate(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:updateNBombRate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_updateNBombRate'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_startBullet(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_startBullet'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Layer* arg0;
        cocos2d::ValueMap arg1;

        ok &= luaval_to_object<cocos2d::Layer>(tolua_S, 2, "cc.Layer",&arg0, "LuaCppAdapter:startBullet");

        ok &= luaval_to_ccvaluemap(tolua_S, 3, &arg1, "LuaCppAdapter:startBullet");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_startBullet'", nullptr);
            return 0;
        }
        cobj->startBullet(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:startBullet",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_startBullet'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_bulletChangeTarget(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_bulletChangeTarget'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:bulletChangeTarget");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_bulletChangeTarget'", nullptr);
            return 0;
        }
        cobj->bulletChangeTarget(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:bulletChangeTarget",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_bulletChangeTarget'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_fishAccelerateOut(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_fishAccelerateOut'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_fishAccelerateOut'", nullptr);
            return 0;
        }
        cobj->fishAccelerateOut();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:fishAccelerateOut",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_fishAccelerateOut'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_frameRunning(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_frameRunning'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:frameRunning");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_frameRunning'", nullptr);
            return 0;
        }
        cobj->frameRunning(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:frameRunning",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_frameRunning'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_updateThunderRate(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_updateThunderRate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "LuaCppAdapter:updateThunderRate");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_updateThunderRate'", nullptr);
            return 0;
        }
        cobj->updateThunderRate(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:updateThunderRate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_updateThunderRate'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_debugLog(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_debugLog'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaCppAdapter:debugLog");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_debugLog'", nullptr);
            return 0;
        }
        cobj->debugLog(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:debugLog",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_debugLog'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_fishTimeLineCome(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_fishTimeLineCome'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:fishTimeLineCome");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_fishTimeLineCome'", nullptr);
            return 0;
        }
        cobj->fishTimeLineCome(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:fishTimeLineCome",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_fishTimeLineCome'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_fishSwithColor(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_fishSwithColor'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:fishSwithColor");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_fishSwithColor'", nullptr);
            return 0;
        }
        cobj->fishSwithColor(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:fishSwithColor",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_fishSwithColor'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_syncFrame(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_syncFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:syncFrame");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_syncFrame'", nullptr);
            return 0;
        }
        cobj->syncFrame(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:syncFrame",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_syncFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_fishGroupCome(lua_State* tolua_S)
{
    int argc = 0;
    LuaCppAdapter* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaCppAdapter*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_LuaCppAdapter_LuaCppAdapter_fishGroupCome'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::ValueMap arg0;

        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "LuaCppAdapter:fishGroupCome");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_fishGroupCome'", nullptr);
            return 0;
        }
        cobj->fishGroupCome(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaCppAdapter:fishGroupCome",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_fishGroupCome'.",&tolua_err);
#endif

    return 0;
}
int lua_LuaCppAdapter_LuaCppAdapter_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaCppAdapter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_LuaCppAdapter_LuaCppAdapter_getInstance'", nullptr);
            return 0;
        }
        LuaCppAdapter* ret = LuaCppAdapter::getInstance();
        object_to_luaval<LuaCppAdapter>(tolua_S, "LuaCppAdapter",(LuaCppAdapter*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "LuaCppAdapter:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_LuaCppAdapter_LuaCppAdapter_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_LuaCppAdapter_LuaCppAdapter_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (LuaCppAdapter)");
    return 0;
}

int lua_register_LuaCppAdapter_LuaCppAdapter(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LuaCppAdapter");
    tolua_cclass(tolua_S,"LuaCppAdapter","LuaCppAdapter","",nullptr);

    tolua_beginmodule(tolua_S,"LuaCppAdapter");
        tolua_function(tolua_S,"getPlayerId",lua_LuaCppAdapter_LuaCppAdapter_getPlayerId);
        tolua_function(tolua_S,"changeTimeline",lua_LuaCppAdapter_LuaCppAdapter_changeTimeline);
        tolua_function(tolua_S,"playerFire",lua_LuaCppAdapter_LuaCppAdapter_playerFire);
        tolua_function(tolua_S,"startNets",lua_LuaCppAdapter_LuaCppAdapter_startNets);
        tolua_function(tolua_S,"setCannon",lua_LuaCppAdapter_LuaCppAdapter_setCannon);
        tolua_function(tolua_S,"openRankWeb",lua_LuaCppAdapter_LuaCppAdapter_openRankWeb);
        tolua_function(tolua_S,"luaUseCppFun",lua_LuaCppAdapter_LuaCppAdapter_luaUseCppFun);
        tolua_function(tolua_S,"exitGame",lua_LuaCppAdapter_LuaCppAdapter_exitGame);
        tolua_function(tolua_S,"addPlayer",lua_LuaCppAdapter_LuaCppAdapter_addPlayer);
        tolua_function(tolua_S,"getPathPos",lua_LuaCppAdapter_LuaCppAdapter_getPathPos);
        tolua_function(tolua_S,"callFish",lua_LuaCppAdapter_LuaCppAdapter_callFish);
        tolua_function(tolua_S,"bindUI",lua_LuaCppAdapter_LuaCppAdapter_bindUI);
        tolua_function(tolua_S,"callLuaFunc",lua_LuaCppAdapter_LuaCppAdapter_callLuaFunc);
        tolua_function(tolua_S,"setLuaNode",lua_LuaCppAdapter_LuaCppAdapter_setLuaNode);
        tolua_function(tolua_S,"getConfigData",lua_LuaCppAdapter_LuaCppAdapter_getConfigData);
        tolua_function(tolua_S,"loadDataBin",lua_LuaCppAdapter_LuaCppAdapter_loadDataBin);
        tolua_function(tolua_S,"setFishState",lua_LuaCppAdapter_LuaCppAdapter_setFishState);
        tolua_function(tolua_S,"getConfigDataByName",lua_LuaCppAdapter_LuaCppAdapter_getConfigDataByName);
        tolua_function(tolua_S,"removePlayerBullet",lua_LuaCppAdapter_LuaCppAdapter_removePlayerBullet);
        tolua_function(tolua_S,"getNBombKilledFishes",lua_LuaCppAdapter_LuaCppAdapter_getNBombKilledFishes);
        tolua_function(tolua_S,"getLuaValue",lua_LuaCppAdapter_LuaCppAdapter_getLuaValue);
        tolua_function(tolua_S,"startGame",lua_LuaCppAdapter_LuaCppAdapter_startGame);
        tolua_function(tolua_S,"getVIPResId",lua_LuaCppAdapter_LuaCppAdapter_getVIPResId);
        tolua_function(tolua_S,"getCurFrame",lua_LuaCppAdapter_LuaCppAdapter_getCurFrame);
        tolua_function(tolua_S,"addDebugLayer",lua_LuaCppAdapter_LuaCppAdapter_addDebugLayer);
        tolua_function(tolua_S,"setGunRate",lua_LuaCppAdapter_LuaCppAdapter_setGunRate);
        tolua_function(tolua_S,"otherPlayerBulletCollision",lua_LuaCppAdapter_LuaCppAdapter_otherPlayerBulletCollision);
        tolua_function(tolua_S,"updateNBombRate",lua_LuaCppAdapter_LuaCppAdapter_updateNBombRate);
        tolua_function(tolua_S,"startBullet",lua_LuaCppAdapter_LuaCppAdapter_startBullet);
        tolua_function(tolua_S,"bulletChangeTarget",lua_LuaCppAdapter_LuaCppAdapter_bulletChangeTarget);
        tolua_function(tolua_S,"fishAccelerateOut",lua_LuaCppAdapter_LuaCppAdapter_fishAccelerateOut);
        tolua_function(tolua_S,"frameRunning",lua_LuaCppAdapter_LuaCppAdapter_frameRunning);
        tolua_function(tolua_S,"updateThunderRate",lua_LuaCppAdapter_LuaCppAdapter_updateThunderRate);
        tolua_function(tolua_S,"debugLog",lua_LuaCppAdapter_LuaCppAdapter_debugLog);
        tolua_function(tolua_S,"fishTimeLineCome",lua_LuaCppAdapter_LuaCppAdapter_fishTimeLineCome);
        tolua_function(tolua_S,"fishSwithColor",lua_LuaCppAdapter_LuaCppAdapter_fishSwithColor);
        tolua_function(tolua_S,"syncFrame",lua_LuaCppAdapter_LuaCppAdapter_syncFrame);
        tolua_function(tolua_S,"fishGroupCome",lua_LuaCppAdapter_LuaCppAdapter_fishGroupCome);
        tolua_function(tolua_S,"getInstance", lua_LuaCppAdapter_LuaCppAdapter_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaCppAdapter).name();
    g_luaType[typeName] = "LuaCppAdapter";
    g_typeCast["LuaCppAdapter"] = "LuaCppAdapter";
    return 1;
}
TOLUA_API int register_all_LuaCppAdapter(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_LuaCppAdapter_LuaCppAdapter(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

