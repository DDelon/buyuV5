#ifndef LJMSG_H
#define LJMSG_H
#include <string>
struct lua_State;
int JMsgRegisterLua(lua_State* L);
void JMsgSetLogPath(std::string path);
#endif