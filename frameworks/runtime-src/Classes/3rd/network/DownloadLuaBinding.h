//
//  DownloadLuaBinding.hpp
//  MaJiang
//
//  Created by weile on 16/8/26.
//
//

#ifndef DownloadLuaBinding_hpp
#define DownloadLuaBinding_hpp

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace download
{
    /**
     * Lua端调用方法：
        c_network.download.start(url, function(info)
            -- dosomething
        end)
     * 参数：info类型table
        info.data：  返回的数据，字符串
        info.result：返回结果，true成功，false失败
        info.url：   请求的url
        info.code:   如果请求失败，失败的错误码
        info.errorMsg：请求失败的错误提示信息
     */
    int lua_open_download(lua_State* L);
}

#endif /* DownloadLuaBinding_hpp */
