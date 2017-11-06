//
//  DownloadLuaBinding.cpp
//  MaJiang
//
//  Created by weile on 16/8/26.
//
//

#include <sstream>

#include "DownloadLuaBinding.h"
#include "Download.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

namespace download
{
    static inline void registry_setfield(lua_State* L, const std::string& key)
    {
        lua_setfield(L, LUA_REGISTRYINDEX, key.c_str());
    }
    
    static void inline registry_getfield(lua_State* L, const std::string& key)
    {
        lua_getfield(L, LUA_REGISTRYINDEX, key.c_str());
    }
    
    static inline void lua_push_uint64(lua_State* L, uint64_t val)
    {
        const char* p = reinterpret_cast<const char*>(&val);
        lua_pushlstring(L, p, sizeof(val));
    }
    
    static inline std::string s_keyFromId(uint64_t Id)
    {
        std::stringstream key;
        key << "network.download.id";
        key << Id;
        return key.str();
    }
    
    static void s_callback(lua_State* L, DownloadInfo& info)
    {
        int Id = info.getId();
        auto key = s_keyFromId(Id);
        registry_getfield(L, key);
        if (lua_isfunction(L, -1))
        {
            lua_newtable(L);
            lua_pushboolean(L, info.getResult());
            lua_setfield(L, -2, "result");
            
            auto& url = info.getUrl();
            if ( !url.empty())
            {
                lua_pushstring(L, url.c_str());
                lua_setfield(L, -2, "url");
            }
            
            auto& errorMsg = info.getErrorMsg();
            if ( !errorMsg.empty() )
            {
                lua_pushinteger(L, info.getCode());
                lua_setfield(L, -2, "code");
                
                lua_pushstring(L, errorMsg.c_str());
                lua_setfield(L, -2, "errorMsg");
            }
            
            auto data = info.getData();
            if (nullptr != data)
            {
                std::string str(data->begin(), data->end());
                lua_pushlstring(L, str.c_str(), str.length());
                lua_setfield(L, -2, "data");
                
                lua_pushnumber(L, data->size());
                lua_setfield(L, -2, "dataSize");
            }
            
            cocos2d::LuaStack* stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
            stack->executeFunction(1);
        }
        else
        {
            // 错误提示：Lua第二个参数必须提供，并且是个函数
            assert(false);
        }
        lua_pushnil(L);
        registry_setfield(L, key);
    }
    
    static int download_start(lua_State* L)
    {
        auto url = lua_tostring(L, 1);
        assert(url);
        luaL_checktype(L, 2, LUA_TFUNCTION);
        auto id = Download::getInstance().start(url,
                                                [=](DownloadInfo& info)
                                                {
                                                    s_callback(L, info);
                                                });
        lua_pushvalue(L, 2);
        registry_setfield(L, s_keyFromId(id));
        lua_push_uint64(L, id);
        
        return 1;
    }
    
    static int lua_open_http(lua_State* L)
    {
        luaL_Reg regs[] = {
            { "start",    download_start    },
        };
        
        lua_createtable(L, 0, sizeof(regs) / sizeof(regs[0]));
        for (auto& reg : regs)
        {
            lua_pushcfunction(L, reg.func);
            lua_setfield(L, -2, reg.name);
        }
        return 1;
    }
    
    int lua_open_download(lua_State* L)
    {
        lua_createtable(L, 0, 1);
        lua_open_http(L);
        lua_setfield(L, -2, "download");
        lua_setglobal(L, "network");
        
        return 1;
    }
}