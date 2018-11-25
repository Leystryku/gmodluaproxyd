#pragma once

#include "Types.h"
#include "LuaBase.h"
#include "UserData.h"

struct lua_fState
{
	unsigned char _ignore_this_common_lua_header_[69];
	GarrysMod::Lua::ILuaBase *luabase;
};

#if defined _WIN32

	#define GMOD_DLL_EXPORT extern "C" __declspec(dllexport)

#else

	#define GMOD_DLL_EXPORT extern "C" __attribute__((visibility("default")))

#endif

#define GMOD_MODULE_OPEN( ) GMOD_DLL_EXPORT int gmod13_open( lua_fState *state )
#define GMOD_MODULE_CLOSE( ) GMOD_DLL_EXPORT int gmod13_close( lua_fState *state )

#define LUA state->luabase

#if !defined _MSC_VER || _MSC_VER >= 1900

#define LUA_FUNCTION( name ) int name( lua_fState *state ) noexcept

#elif defined _MSC_VER >= 1700

#include <yvals.h>
#define LUA_FUNCTION( name ) int name( lua_fState *state ) _NOEXCEPT

#else

#define LUA_FUNCTION( name ) int name( lua_fState *state )

#endif

#define LUA_FUNCTION_STATIC( name ) static LUA_FUNCTION( name )
