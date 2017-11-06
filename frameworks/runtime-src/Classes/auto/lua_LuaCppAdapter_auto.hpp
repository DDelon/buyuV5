#include "base/ccConfig.h"
#ifndef __LuaCppAdapter_h__
#define __LuaCppAdapter_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_all_LuaCppAdapter(lua_State* tolua_S);









































#endif // __LuaCppAdapter_h__
