#include "lua_custom.h"
#include <Windows.h>

//#define sheet (void)0;
//#define sheet if (cour && ls_our) { *(void**)((char*)cour +0x4) = (void*)ls_our; }
//#define sheet(num) if(L == clws) { lognum(num); };
//#define sheet(num) if(L != ms && L != ss && L != ws && !clws) { clws = L; }

extern bool rec;

//#define sheet(num) if(rec) {  lognum(num); }; 	dicks();
#define sheet(num) if(rec) { lognum(num); };// dicks();
FILE*logger = 0;
void lognum(int num)
{

	char convnum[20] = { 0 };
	sprintf(convnum, "%d\n", num);

	if (!logger)
		logger = fopen("C:/Users/Leystryku/Desktop/dick.txt", "a+b");

	fwrite(convnum, sizeof(char), strlen(convnum), logger);
	fclose(logger);
	logger = 0;

}

extern int32_t coolbridgeon(void*L);
extern int32_t coolbridgeoff(void*L);
extern int32_t coolbridge_enabled(void*state);
class CLuaInterface;
extern bool clua_grabstate;

#define clwsb ls_client_bak
#define clws ls_client
#define ws ls_our

/*int ourrefs[0xFFF] = { 0 };
int hisrefs[0xFFF] = { 0 };
int ref_count = 0;*/

fnluaL_openlibs origluaL_openlibs;
fnluaL_openlib origluaL_openlib;
fnluaopen_base origluaopen_base;
fnluaopen_bit origluaopen_bit;
fnluaopen_debug origluaopen_debug;
fnluaopen_jit origluaopen_jit;
fnluaopen_math origluaopen_math;
fnluaopen_package origluaopen_package;
fnluaopen_string origluaopen_string;
fnluaopen_table origluaopen_table;
fnluaopen_os origluaopen_os;

fnluaL_newmetatable origluaL_newmetatable;

fnlua_newstate origlua_newstate;
fnlua_close origlua_close;
fnlua_newthread origlua_newthread;
fnlua_atpanic origlua_atpanic;
fnlua_gettop origlua_gettop;
fnlua_settop origlua_settop;
fnlua_pushvalue origlua_pushvalue;
fnlua_remove origlua_remove;
fnlua_insert origlua_insert;
fnlua_replace origlua_replace;
fnlua_copy origlua_copy;
fnlua_checkstack origlua_checkstack;
fnlua_xmove origlua_xmove;
fnlua_isnumber origlua_isnumber;
fnlua_isstring origlua_isstring;
fnlua_iscfunction origlua_iscfunction;
fnlua_isuserdata origlua_isuserdata;
fnlua_type origlua_type;
fnlua_typename origlua_typename;
fnlua_tonumberx origlua_tonumberx;
fnlua_tointegerx origlua_tointegerx;
fnlua_toboolean origlua_toboolean;
fnlua_tolstring origlua_tolstring;
fnlua_rawlen origlua_rawlen;
fnlua_tocfunction origlua_tocfunction;
fnlua_touserdata origlua_touserdata;
fnlua_tothread origlua_tothread;
fnlua_topointer origlua_topointer;
fnlua_arith origlua_arith;
fnlua_rawequal origlua_rawequal;
fnlua_compare origlua_compare;
fnlua_pushnil origlua_pushnil;
fnlua_pushnumber origlua_pushnumber;
fnlua_pushinteger origlua_pushinteger;
fnlua_pushlstring origlua_pushlstring;
fnlua_pushstring origlua_pushstring;
fnlua_pushvfstring origlua_pushvfstring;
fnlua_pushfstring origlua_pushfstring;
fnlua_pushcclosure origlua_pushcclosure;
fnlua_pushboolean origlua_pushboolean;
fnlua_pushlightuserdata origlua_pushlightuserdata;
fnlua_pushthread origlua_pushthread;
fnlua_gettable origlua_gettable;
fnlua_getfield origlua_getfield;
fnlua_rawget origlua_rawget;
fnlua_rawgeti origlua_rawgeti;
fnlua_rawgetp origlua_rawgetp;
fnlua_createtable origlua_createtable;
fnlua_newuserdata origlua_newuserdata;
fnlua_getmetatable origlua_getmetatable;
fnlua_settable origlua_settable;
fnlua_setfield origlua_setfield;
fnlua_rawset origlua_rawset;
fnlua_rawseti origlua_rawseti;
fnlua_setmetatable origlua_setmetatable;
fnlua_call origlua_call;
fnlua_pcall origlua_pcall;
fnlua_gc origlua_gc;
fnlua_next origlua_next;
fnlua_concat origlua_concat;
fnlua_len origlua_len;

fnluaL_buffinit origluaL_buffinit;
fnluaL_optnumber origluaL_optnumber;
fnlua_lessthan origlua_lessthan;
fnlua_equal origlua_equal;
fnlua_error origlua_error;
fnlua_getfenv origlua_getfenv;
fnlua_cpcall origlua_cpcall;
fnluaL_optlstring origluaL_optlstring;
fnlua_getupvalue origlua_getupvalue;
fnluaL_checkudata origluaL_checkudata;
fnlua_objlen origlua_objlen;
fnluaL_ref origluaL_ref;
fnlua_gethook origlua_gethook;
fnluaL_checktype origluaL_checktype;
fnlua_status origlua_status;
fnluaL_unref origluaL_unref;
fnlua_sethook origlua_sethook;
fnlua_setupvalue origlua_setupvalue;
fnluaL_where origluaL_where;
fnluaL_execresult origluaL_execresult;
fnlua_setallocf origlua_setallocf;
fnlua_gethookmask origlua_gethookmask;
fnlua_getstack origlua_getstack;
fnlua_resume_real origlua_resume_real;
fnluaL_checknumber origluaL_checknumber;
fnluaL_traceback origluaL_traceback;
fnluaL_gsub origluaL_gsub;
fnluaL_typerror origluaL_typerror;
fnlua_yield origlua_yield;
fnlua_upvalueid origlua_upvalueid;
fnluaL_optinteger origluaL_optinteger;
fnluaJIT_setmode origluaJIT_setmode;
fnluaL_checkinteger origluaL_checkinteger;
fnluaL_checkstack origluaL_checkstack;
fnlua_setlocal origlua_setlocal;
fnluaL_checklstring origluaL_checklstring;
fnlua_dump origlua_dump;
fnlua_upvaluejoin origlua_upvaluejoin;
fnluaL_checkany origluaL_checkany;
fnluaL_findtable origluaL_findtable;
fnlua_gethookcount origlua_gethookcount;
fnluaL_callmeta origluaL_callmeta;
fnluaL_fileresult origluaL_fileresult;
fnluaL_getmetafield origluaL_getmetafield;
fnluaL_checkoption origluaL_checkoption;
fnluaL_newmetatable_type origluaL_newmetatable_type;
fnluaL_error origluaL_error;
fnlua_setfenv origlua_setfenv;
fnluaL_register origluaL_register;
fnluaL_argerror origluaL_argerror;
fnlua_getinfo origlua_getinfo;
fnlua_getallocf origlua_getallocf;

struct UserData
{
	void *data;
	unsigned char type;
};

void* userdata_our[0xFFFFF] = { 0 };
void* userdata_his[0xFFFFF] = { 0 };
size_t userdata_sizes[0xFFFFF] = { 0 };
size_t userdata_lifetime[0xFFFFF] = { 0 };

int userdata_num = 0;

void dicks()
{
	return;

	if (!userdata_num)
		return;

	if (!clws && !ws && !clwsb)
	{
		memset(userdata_our, 0, sizeof(userdata_our));
		memset(userdata_his, 0, sizeof(userdata_his));
		memset(userdata_sizes, 0, sizeof(userdata_sizes));
		memset(userdata_lifetime, 0, sizeof(userdata_lifetime));

		userdata_num = 0;
		return;
	}

	for (int i = 0; i<userdata_num; i++)
	{
		userdata_lifetime[i] -= 1;
		memcpy(userdata_our[i], userdata_his[i], userdata_sizes[i]); // synch that shit up

		if (!userdata_lifetime[i])
		{
			userdata_our[i] = userdata_our[userdata_num];
			userdata_his[i] = userdata_his[userdata_num];
			userdata_sizes[i] = userdata_sizes[userdata_num];
			userdata_lifetime[i] = userdata_lifetime[userdata_num];

			userdata_our[userdata_num] = 0;
			userdata_his[userdata_num] = 0;
			userdata_lifetime[userdata_num] = 0;
			userdata_sizes[userdata_num] = 0;
			userdata_num -= 1;

		}
	}
}

static void transferTable(lua_State *L1, lua_State *L2)
{
	origlua_pushnil(L1);
	while (origlua_next(L1, -2) != 0)
	{
		if (origlua_isnumber(L1, -1))
		{
			size_t len = 0;
			origlua_pushstring(L2, origlua_tolstring(L1, -2, &len));
			origlua_pushnumber(L2, origlua_tonumberx(L1, -1));
			origlua_settable(L2, -3);
		}
		else if (origlua_isstring(L1, -1))
		{
			size_t len = 0;
			origlua_pushstring(L2, origlua_tolstring(L1, -2, &len));
			origlua_pushstring(L2, origlua_tolstring(L1, -2, &len));
			origlua_settable(L2, -3);
		}
		else if (origlua_type(L1, -1) == 5)//istale
		{
			size_t len = 0;
			origlua_pushstring(L2, origlua_tolstring(L1, -2, &len));
			origlua_createtable(L2, 0, 0);
			transferTable(L1, L2);
			origlua_settable(L2, -3);
		}
		origlua_settop(L1, 1);
	}
}

void lua_steal(lua_State *from, lua_State *to)
{
	int idx = origlua_gettop(from);
	int tostealtype = origlua_type(from, idx);

	int ret = 0;

	switch (tostealtype)
	{
	case -1:
		break;
	case 0:
	{
		origlua_pushnil(to);
	}
	case 1:
	{
		origlua_pushboolean(to, origlua_toboolean(from, idx));
	}
		break;
	case 2:
	{

		origlua_pushlightuserdata(to, origlua_touserdata(from, idx));

	}
	case 3:
	{

		origlua_pushnumber(to, origlua_tonumberx(from, idx));
		break;

	}
	case 4:
	{

		size_t size = 0;
		const char *topush = origlua_tolstring(from, idx, &size);
		origlua_pushlstring(to, topush, size);
		break;
	}

	case 5:
	{
		transferTable(from, to);

		break;
	}
	case 6:
	{

		lua_CFunction func = origlua_tocfunction(from, idx);

		if (!func)
		{
			origlua_pushnil(to);
		}
		else {
			origlua_pushcclosure(to, func, 0);
		}

		break;
}
	case 7:
	{
		UserData* oldud = (UserData *)origlua_touserdata(from, idx);
		UserData*newud = (UserData *)origlua_newuserdata(to, 8);
		newud->data = oldud->data;
		newud->type = oldud->type;

		if (origlua_getmetatable(from, idx) != 0)
		{
			origluaL_newmetatable_type(to, (void*)origlua_typename(from, 7, idx), oldud->type);
			origlua_setmetatable(to, -2);
		}

		break;
		}

	default:
	{
		origlua_pushnil(to);
		break;
		}
	}


	origlua_settop(from, -1 - 1);

}

//the actual hooks
int __cdecl hooked_luaL_newmetatable(lua_State *L, const char *name)
{
	sheet(1);

	if (coolbridge_enabled(L))
	{
		return origluaL_newmetatable(ws, name);
	}

	if (ws && clws == L)
	{
		void*bak = clws;
		clws = 0;
		int ret = origluaL_newmetatable(L, name);
		origluaL_newmetatable(ws, name);
		clws = (lua_State*)bak;
		return ret;
	}


	return  origluaL_newmetatable(L, name);
}

void* openfix = 0;

void* __cdecl hooked_luaL_openlibs(lua_State *L)
{
	sheet(2);

	if (coolbridge_enabled(L))
	{

		return origluaL_openlibs(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaL_openlibs(L);

	if (openfix)
	{
		origluaL_openlibs(ws);
		clws = (lua_State*)openfix;
		openfix = 0;
	}

	return ret;

}

void*shit = 0;
void* __cdecl hooked_luaL_openlib(lua_State *L, const char *libname, const void *l, int nup)
{
	sheet(3);

	if (coolbridge_enabled(L))
	{

		return origluaL_openlib(ws, libname, l, nup);
	}

	if (ws && clws == L)
	{

		origluaL_openlib(ws, libname, l, nup);
	}

	shit = clws;
	clws = 0;


	void* ret = origluaL_openlib(L, libname, l, nup);


	clws = (lua_State*)shit;
	shit = 0;

	return ret;

}

void* __cdecl hooked_luaopen_base(lua_State *L)
{
	sheet(4);

	if (coolbridge_enabled(L))
	{
		return origluaopen_base(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaopen_base(L);

	if (openfix)
	{
		origluaopen_base(ws);
		clws = (lua_State*)openfix;
	}

	return ret;
}

void* __cdecl hooked_luaopen_bit(lua_State *L)
{

	sheet(5);

	if (coolbridge_enabled(L))
	{

		return origluaopen_bit(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaopen_bit(L);

	if (openfix)
	{
		origluaopen_bit(ws);
		clws = (lua_State*)openfix;
	}

	return ret;

}

void* __cdecl hooked_luaopen_debug(lua_State *L)
{
	sheet(6);

	if (coolbridge_enabled(L))
	{
		return origluaopen_debug(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaopen_debug(L);

	if (openfix)
	{
		origluaopen_debug(ws);
		clws = (lua_State*)openfix;
	}

	return ret;

}

void* __cdecl hooked_luaopen_jit(lua_State *L)
{
	sheet(7);

	if (coolbridge_enabled(L))
	{
		return origluaopen_jit(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void*ret = origluaopen_jit(L);

	if (openfix)
	{
		origluaopen_jit(ws);
		clws = (lua_State*)openfix;
	}

	return ret;

}

void* __cdecl hooked_luaopen_math(lua_State *L)
{
	sheet(8);

	if (coolbridge_enabled(L))
	{
		return origluaopen_math(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaopen_math(L);

	if (openfix)
	{
		origluaopen_math(ws);
		clws = (lua_State*)openfix;
	}

	return ret;
}

void* __cdecl hooked_luaopen_package(lua_State *L)
{
	sheet(9);

	if (coolbridge_enabled(L))
	{
		return origluaopen_package(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaopen_package(L);

	if (openfix)
	{
		origluaopen_package(ws);
		clws = (lua_State*)openfix;
	}

	return ret;
}

void* __cdecl hooked_luaopen_string(lua_State *L)
{
	sheet(10);

	if (coolbridge_enabled(L))
	{
		return origluaopen_string(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaopen_string(L);

	if (openfix)
	{
		origluaopen_string(ws);
		clws = (lua_State*)openfix;
	}

	return ret;
}

void* __cdecl hooked_luaopen_table(lua_State *L)
{
	sheet(11);

	if (coolbridge_enabled(L))
	{
		return origluaopen_table(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaopen_table(L);

	if (openfix)
	{
		origluaopen_table(ws);
		clws = (lua_State*)openfix;
	}

	return ret;
}

void* __cdecl hooked_luaopen_os(lua_State *L)
{
	sheet(12);

	if (coolbridge_enabled(L))
	{
		return origluaopen_os(ws);
	}

	if (ws && clws == L)
	{
		openfix = clws;
		clws = 0;
	}


	void* ret = origluaopen_os(L);

	if (openfix)
	{
		origluaopen_os(ws);
		clws = (lua_State*)openfix;
	}

	return ret;

}


/*
** state manipulation
*/

lua_State *__cdecl  hooked_lua_newstate(lua_Alloc L, void *ud)
{

	sheet(13);

	if (clua_grabstate)
	{
		memset(userdata_our, sizeof(userdata_our), 0);
		memset(userdata_his, sizeof(userdata_his), 0);
		memset(userdata_sizes, sizeof(userdata_sizes), 0);
		memset(userdata_lifetime, 0, sizeof(userdata_lifetime));
		userdata_num = 0;


		ls_client = origlua_newstate(L, ud);
		clua_grabstate = false;


		printf("1: %d __ %d\n", origlua_gettop(ls_client), origlua_gettop(ws));

		return ls_client;
	}



	return origlua_newstate(L, ud);
}

void __cdecl  hooked_lua_close(lua_State *L)
{


	sheet(14);

	origlua_close(L);




}

lua_State *__cdecl  hooked_lua_newthread(lua_State *L)
{

	sheet(15);

	return origlua_newthread(L);
}


lua_CFunction __cdecl  hooked_lua_atpanic(lua_State *L, lua_CFunction panicf)
{

	sheet(16);

	return origlua_atpanic(L, panicf);
}




/*
** basic stack manipulation
*/
int __cdecl  hooked_lua_gettop(lua_State *L)
{
	sheet(17);

	if (coolbridge_enabled(L))
	{
		return origlua_gettop(ws);
	}

	if (ws && clws == L)
	{
		origlua_gettop(ws);
	}

	return origlua_gettop(L);
}

void* __cdecl  hooked_lua_settop(lua_State *L, int idx)
{
	sheet(18);


	if (coolbridge_enabled(L))
	{
		return origlua_settop(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_settop(ws, idx);
	}
	return origlua_settop(L, idx);
}

void* __cdecl  hooked_lua_pushvalue(lua_State *L, int idx)
{
	sheet(19);


	if (coolbridge_enabled(L))
	{
		return origlua_pushvalue(ws, idx);
	}

	if (ws && clws == L)
	{

		origlua_pushvalue(ws, idx);
	}
	return origlua_pushvalue(L, idx);
}

void* __cdecl  hooked_lua_remove(lua_State *L, int idx)
{

	sheet(20);


	if (coolbridge_enabled(L))
	{

		return origlua_remove(ws, idx);
	}

	if (ws && clws == L)
	{

		origlua_remove(ws, idx);
	}


	return origlua_remove(L, idx);
}

void* __cdecl  hooked_lua_insert(lua_State *L, int idx)
{

	sheet(21);


	if (coolbridge_enabled(L))
	{

		return origlua_insert(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_insert(ws, idx);
	}

	return origlua_insert(L, idx);
}

void* __cdecl  hooked_lua_replace(lua_State *L, int idx)
{

	sheet(22);


	if (coolbridge_enabled(L))
	{

		return origlua_replace(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_replace(ws, idx);
	}

	return origlua_replace(L, idx);
}

void* __cdecl  hooked_lua_copy(lua_State *L, int fromidx, int toidx)
{

	sheet(23);


	if (coolbridge_enabled(L))
	{

		return origlua_copy(ws, fromidx, toidx);
	}

	if (ws && clws == L)
	{
		origlua_copy(ws, fromidx, toidx);
	}

	return origlua_copy(L, fromidx, toidx);
}

int __cdecl  hooked_lua_checkstack(lua_State *L, int sz)
{

	sheet(24);


	if (coolbridge_enabled(L))
	{

		return origlua_checkstack(ws, sz);
	}

	if (ws && clws == L)
	{
		origlua_checkstack(ws, sz);
	}

	return origlua_checkstack(L, sz);
}


void __cdecl  hooked_lua_xmove(lua_State *L, lua_State *to, int n)
{

	sheet(25);

	origlua_xmove(L, to, n);
}



/*
** access functions (stack -> C)
*/

int __cdecl  hooked_lua_isnumber(lua_State *L, int idx)
{

	sheet(26);


	if (coolbridge_enabled(L))
	{

		return origlua_isnumber(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_isnumber(ws, idx);
	}

	return origlua_isnumber(L, idx);
}

int __cdecl  hooked_lua_isstring(lua_State *L, int idx)
{

	sheet(27);


	if (coolbridge_enabled(L))
	{

		return origlua_isstring(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_isstring(ws, idx);
	}

	return origlua_isstring(L, idx);
}

int __cdecl  hooked_lua_iscfunction(lua_State *L, int idx)
{

	sheet(28);


	if (coolbridge_enabled(L))
	{

		return origlua_iscfunction(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_iscfunction(ws, idx);
	}

	return origlua_iscfunction(L, idx);
}

int __cdecl  hooked_lua_isuserdata(lua_State *L, int idx)
{

	sheet(29);


	if (coolbridge_enabled(L))
	{

		return origlua_isuserdata(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_isuserdata(ws, idx);
	}

	return origlua_isuserdata(L, idx);
}

int __cdecl  hooked_lua_type(lua_State *L, int idx)
{
	sheet(30);


	if (coolbridge_enabled(L))
	{
		return origlua_type(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_type(ws, idx);
	}

	if (coolbridge_enabled(L) )
	{

		return origlua_type(ws, idx);
		int ourtype = origlua_type(ws, idx);
		if (ourtype != -1 && ourtype != 0)
		{
			return ourtype;
		}

		int histype = origlua_type(clwsb, idx);
		if (histype != -1 && histype != 0)//not nil in this state
		{
			return histype;
		}

		return ourtype;
	}

	return origlua_type(L, idx);
}

const char *__cdecl  hooked_lua_typename(lua_State *L, int tp, int tp2)
{
	sheet(31);


	if (coolbridge_enabled(L))
	{
		return origlua_typename(ws, tp, tp2);
	}

	if (ws && clws == L)
	{
		origlua_typename(ws, tp, tp2);
	}

	if (coolbridge_enabled(L) )
	{
		return origlua_typename(ws, tp, tp2);

		const char* ourtype = origlua_typename(ws, tp, tp2);
		if (!strstr(ourtype, "UserData"))
		{
			return ourtype;
		}

		const char* histype = origlua_typename(clwsb, tp, tp2);
		if (!strstr(histype, "UserData"))
		{
			printf("KK: %s\n", histype);
			return histype;
		}

		return ourtype;
	}


	return origlua_typename(L, tp, tp2);
}


lua_Number __cdecl  hooked_lua_tonumberx(lua_State *L, int idx)
{
	sheet(32);


	if (coolbridge_enabled(L))
	{
		return origlua_tonumberx(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_tonumberx(ws, idx);
	}
	return origlua_tonumberx(L, idx);
}

lua_Integer __cdecl  hooked_lua_tointegerx(lua_State *L, int idx)
{
	sheet(33);


	if (coolbridge_enabled(L))
	{
		lua_Integer ret = (lua_Integer)origlua_tointegerx(ws, idx);

		return (lua_Integer)ret;

	}

	if (ws && clws == L)
	{
		origlua_tointegerx(ws, idx);
	}


	lua_Integer ret = (lua_Integer)origlua_tointegerx(L, idx);

	return (lua_Integer)ret;
}

int __cdecl  hooked_lua_toboolean(lua_State *L, int idx)
{
	sheet(35);


	if (coolbridge_enabled(L))
	{
		return origlua_toboolean(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_toboolean(ws, idx);
	}
	return origlua_toboolean(L, idx);
}

const char *__cdecl  hooked_lua_tolstring(lua_State *L, int idx, size_t *len)
{
	sheet(36);


	if (coolbridge_enabled(L))
	{
		return origlua_tolstring(ws, idx, len);
	}

	if (ws && clws == L)
	{
		origlua_tolstring(ws, idx, len);
	}
	return origlua_tolstring(L, idx, len);
}

size_t __cdecl  hooked_lua_rawlen(lua_State *L, int idx)
{

	sheet(37);


	if (coolbridge_enabled(L))
	{

		return origlua_rawlen(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_rawlen(ws, idx);
	}

	return origlua_rawlen(L, idx);
}

lua_CFunction __cdecl  hooked_lua_tocfunction(lua_State *L, int idx)
{
	sheet(38);


	if (coolbridge_enabled(L))
	{
		return origlua_tocfunction(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_tocfunction(ws, idx);
	}
	return origlua_tocfunction(L, idx);
}

void *__cdecl  hooked_lua_touserdata(lua_State *L, int idx)
{
	sheet(39);


	if (coolbridge_enabled(L))
	{
		return origlua_touserdata(ws, idx);
	}

	if (ws && clws == L)
	{
		void*bak = clws;
		clws = 0;
		void*ret = origlua_touserdata(L, idx);
		origlua_touserdata(ws, idx);

		clws = (lua_State*)bak;
		return ret;
	}
	return origlua_touserdata(L, idx);
}

lua_State *__cdecl  hooked_lua_tothread(lua_State *L, int idx)
{
	sheet(40);


	if (coolbridge_enabled(L))
	{
		return origlua_tothread(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_tothread(ws, idx);
	}
	return origlua_tothread(L, idx);
}

const void *__cdecl  hooked_lua_topointer(lua_State *L, int idx)
{
	sheet(41);


	if (coolbridge_enabled(L))
	{
		return origlua_topointer(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_topointer(ws, idx);
	}
	return origlua_topointer(L, idx);
}



/*
** Comparison and arithmetic functions
*/

void* __cdecl  hooked_lua_arith(lua_State *L, int op)
{

	sheet(42);


	if (coolbridge_enabled(L))
	{

		return origlua_arith(ws, op);
	}

	if (ws && clws == L)
	{
		origlua_arith(ws, op);
	}

	return origlua_arith(L, op);
}


int __cdecl  hooked_lua_rawequal(lua_State *L, int idx1, int idx2)
{

	sheet(43);


	if (coolbridge_enabled(L))
	{

		return origlua_rawequal(ws, idx1, idx2);
	}

	if (ws && clws == L)
	{
		origlua_rawequal(ws, idx1, idx2);
	}

	return origlua_rawequal(L, idx1, idx2);
}

int __cdecl  hooked_lua_compare(lua_State *L, int idx1, int idx2, int op)
{

	sheet(44);


	if (coolbridge_enabled(L))
	{

		return origlua_compare(ws, idx1, idx2, op);
	}

	if (ws && clws == L)
	{
		origlua_compare(ws, idx1, idx2, op);
	}

	return origlua_compare(L, idx1, idx2, op);
}



/*
** push functions (C -> stack)
*/
void* __cdecl  hooked_lua_pushnil(lua_State *L)
{
	sheet(45);


	if (coolbridge_enabled(L))
	{
		return origlua_pushnil(ws);
	}

	if (ws && clws == L)
	{
		origlua_pushnil(ws);
	}
	return origlua_pushnil(L);
}

void* __cdecl  hooked_lua_pushnumber(lua_State *L, lua_Number n)
{
	sheet(46);


	if (coolbridge_enabled(L))
	{
		return origlua_pushnumber(ws, n);
	}

	if (ws && clws == L)
	{
		origlua_pushnumber(ws, n);
	}
	return origlua_pushnumber(L, n);
}

void* __cdecl  hooked_lua_pushinteger(lua_State *L, lua_Integer n)
{
	sheet(47);


	if (coolbridge_enabled(L))
	{
		return origlua_pushinteger(ws, n);
	}

	if (ws && clws == L)
	{
		origlua_pushinteger(ws, n);
	}
	return origlua_pushinteger(L, n);
}


const char *__cdecl  hooked_lua_pushlstring(lua_State *L, const char *s, size_t l)
{
	sheet(49);


	if (coolbridge_enabled(L))
	{
		return origlua_pushlstring(ws, s, l);
	}

	if (ws && clws == L)
	{
		origlua_pushlstring(ws, s, l);
	}
	return origlua_pushlstring(L, s, l);
}

const char *__cdecl  hooked_lua_pushstring(lua_State *L, const char *s)
{
	sheet(50);

	if (coolbridge_enabled(L))
	{
		return origlua_pushstring(ws, s);
	}

	if (ws && clws == L)
	{
		origlua_pushstring(ws, s);
	}
	return origlua_pushstring(L, s);
}

const char *__cdecl  hooked_lua_pushvfstring(lua_State *L, const char *fmt,
	va_list argp)
{
	sheet(51);

	if (coolbridge_enabled(L))
	{
		return origlua_pushvfstring(ws, fmt, argp);
	}

	if (ws && clws == L)
	{
		origlua_pushvfstring(ws, fmt, argp);
	}
	return origlua_pushvfstring(L, fmt, argp);
}

const char *__cdecl  hooked_lua_pushfstring(lua_State *L, const char *fmt, ...)
{

	char buffer[2048] = { 0 };

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	perror(buffer);
	va_end(args);

	sheet(52);

	if (coolbridge_enabled(L))
	{
		return origlua_pushfstring(ws, buffer);
	}

	if (ws && clws == L)
	{
		origlua_pushfstring(ws, buffer);
	}

	const char* ret = origlua_pushfstring(L, buffer);

	return ret;

}

void* __cdecl  hooked_lua_pushcclosure(lua_State *L, lua_CFunction hooked_, int n)
{
	sheet(53);

	if (coolbridge_enabled(L))
	{
		return origlua_pushcclosure(ws, hooked_, n);
	}

	if (ws && clws == L)
	{
		origlua_pushcclosure(ws, hooked_, n);
	}
	return origlua_pushcclosure(L, hooked_, n);
}

void* __cdecl  hooked_lua_pushboolean(lua_State *L, int b)
{
	sheet(54);

	if (coolbridge_enabled(L))
	{
		return origlua_pushboolean(ws, b);
	}

	if (ws && clws == L)
	{
		origlua_pushboolean(ws, b);
	}
	return origlua_pushboolean(L, b);
}

void* __cdecl  hooked_lua_pushlightuserdata(lua_State *L, void *p)
{
	sheet(55);

	if (coolbridge_enabled(L))
	{
		return origlua_pushlightuserdata(ws, p);
	}

	if (ws && clws == L)
	{
		origlua_pushlightuserdata(ws, p);
	}
	return origlua_pushlightuserdata(L, p);
}

int __cdecl  hooked_lua_pushthread(lua_State *L)
{
	sheet(56);

	if (coolbridge_enabled(L))
	{
		return origlua_pushthread(ws);
	}

	if (ws && clws == L)
	{
		origlua_pushthread(ws);
	}
	return origlua_pushthread(L);
}



/*
** get functions (Lua -> stack)
*/
void* __cdecl  hooked_lua_gettable(lua_State *L, int idx)
{
	sheet(57);

	if (coolbridge_enabled(L))
	{
		return origlua_gettable(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_gettable(ws, idx);
	}
	return origlua_gettable(L, idx);
}

void* __cdecl  hooked_lua_getfield(lua_State *L, int idx, const char *k)
{

	sheet(58);

	if (coolbridge_enabled(L))
	{

		return origlua_getfield(ws, idx, k);
	}

	if (ws && clws == L)
	{
		origlua_getfield(ws, idx, k);
	}

	return origlua_getfield(L, idx, k);
}

void* __cdecl  hooked_lua_rawget(lua_State *L, int idx)
{
	sheet(59);

	if (coolbridge_enabled(L))
	{
		return origlua_rawget(ws, idx);
	}

	if (ws && clws == L)
	{
		void *bak = clws;
		clws = 0;
		origlua_rawget(ws, idx);
		void* ret = origlua_rawget(L, idx);
		clws = (lua_State*)bak;
		return ret;

	}
	return origlua_rawget(L, idx);
}


void* __cdecl  hooked_lua_rawgeti(lua_State *L, int idx, int r)
{
	sheet(60);

	if (ws && clws == L)
	{

		void *bak = clws;
		clws = 0;
		origlua_rawgeti(ws, idx, r);
		void* ret = origlua_rawgeti(L, idx, r);
		clws = (lua_State*)bak;
		return ret;
	}

	if (coolbridge_enabled(L)||L == ws && clwsb && !clws)
	{
		
		void*ret = origlua_rawgeti(ws, idx, r);
		return ret;
		/*
		int ourtype = origlua_type(ws, -2);
		if (ourtype!= -1&& ourtype != 0)
		{
			return ret;
		}



		origlua_rawgeti(clwsb, idx, r);

		int histype = origlua_type(clwsb, -2);
		if (histype == -1 || histype == 0)//nil in this state too
		{
			origlua_settop(clwsb, -(1) - 1);//get rid of the nil
			return ret;
		}


		printf("iz easy: %d __ %d\n", origlua_gettop(L), origlua_gettop(clwsb));


		origlua_settop(ws, -(1) - 1);//get rid of the nil

		origlua_xmove(clwsb, ws, 1);

		//origlua_pushnil(ws);

		//lua_steal(clwsb, ws);//move whats on top of the clwsb stack to ws

		printf("iz easy2: %d __ %d\n", origlua_gettop(L), origlua_gettop(clwsb));

		return ret;*/
	}


	return origlua_rawgeti(L, idx, r);
}

void* __cdecl  hooked_lua_rawgetp(lua_State *L, int idx, const void *p)
{

	sheet(61);

	if (coolbridge_enabled(L))
	{

		return origlua_rawgetp(ws, idx, p);
	}

	if (ws && clws == L)
	{
		void *bak = clws;
		clws = 0;
		origlua_rawgetp(ws, idx, p);
		void* ret = origlua_rawgetp(L, idx, p);
		clws = (lua_State*)bak;
		return ret;
	}

	return origlua_rawgetp(L, idx, p);
}

void* __cdecl  hooked_lua_createtable(lua_State *L, int narr, int nrec)
{

	sheet(62);

	if (coolbridge_enabled(L))
	{

		return origlua_createtable(ws, narr, nrec);
	}

	if (ws && clws == L)
	{
		origlua_createtable(ws, narr, nrec);
	}

	return origlua_createtable(L, narr, nrec);
}

void *__cdecl  hooked_lua_newuserdata(lua_State *L, size_t sz)
{
	sheet(63);

	if (coolbridge_enabled(L))
	{
		return origlua_newuserdata(ws, sz);
	}

	if (ws && clws == L)
	{
		printf("NEU\n");

		void* bak = clws;
		clws = 0;

		void*ourud = origlua_newuserdata(ws, sz);
		void*ret = origlua_newuserdata(L, sz);

		/*
		userdata_our[userdata_num] = ourud;
		userdata_his[userdata_num] = ret;
		userdata_sizes[userdata_num] = sz;
		userdata_lifetime[userdata_num] = 10;
		userdata_num += 1;*/

		clws = (lua_State*)bak;

		return ret;
	}
	return origlua_newuserdata(L, sz);
}

int __cdecl  hooked_lua_getmetatable(lua_State *L, int objindex)
{

	sheet(64);

	if (coolbridge_enabled(L))
	{

		return origlua_getmetatable(ws, objindex);
	}

	if (ws && clws == L)
	{
		origlua_getmetatable(ws, objindex);
	}

	return origlua_getmetatable(L, objindex);
}



/*
** set functions (stack -> Lua)
*/
void* __cdecl  hooked_lua_settable(lua_State *L, int idx)
{

	sheet(65);

	if (coolbridge_enabled(L))
	{

		return origlua_settable(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_settable(ws, idx);
	}

	return origlua_settable(L, idx);
}

void* __cdecl  hooked_lua_setfield(lua_State *L, int idx, const char *k)
{
	sheet(66);

	if (coolbridge_enabled(L))
	{
		return origlua_setfield(ws, idx, k);
	}

	if (ws && clws == L)
	{
		void*bak = clws;
		clws = 0;
		void*ret = origlua_setfield(L, idx, k);
		origlua_setfield(ws, idx, k);
		clws = (lua_State*)bak;
		return ret;
	}
	return origlua_setfield(L, idx, k);
}

void* __cdecl  hooked_lua_rawset(lua_State *L, int idx)
{
	sheet(67);

	if (coolbridge_enabled(L))
	{
		return origlua_rawset(ws, idx);
	}

	if (ws && clws == L)
	{
		void*bak = clws;
		clws = 0;
		origlua_rawset(ws, idx);
		clws = (lua_State*)bak;

		void *ret = origlua_rawset(L, idx);

		return ret;
	}
	return origlua_rawset(L, idx);
}

void* __cdecl  hooked_lua_rawseti(lua_State *L, int idx, int n)
{
	sheet(68);

	if (coolbridge_enabled(L))
	{
		return origlua_rawseti(ws, idx, n);
	}

	if (ws && clws == L)
	{
		void *bak = clws;
		clws = 0;

		origlua_rawseti(ws, idx, n); // its a nil ref or sth
		void* ret = origlua_rawseti(L, idx, n);
		clws = (lua_State*)bak;

		return ret;
	}

	return origlua_rawseti(L, idx, n);
}

int __cdecl  hooked_lua_setmetatable(lua_State *L, int objindex)
{
	sheet(69);

	if (coolbridge_enabled(L))
	{
		return origlua_setmetatable(ws, objindex);
	}

	if (ws && clws == L)
	{



		void* bak = clws;
		clws = 0;
		int ret = origlua_setmetatable(L, objindex);
		origlua_setmetatable(ws, objindex);
		clws = (lua_State*)bak;
		return ret;
	}
	return origlua_setmetatable(L, objindex);
}



/*
** 'load' and 'call' functions (load and run Lua code)
*/
void* __cdecl  hooked_lua_call(lua_State *L, int nargs, int nresults)
{
	sheet(70);

	if (coolbridge_enabled(L) || (ws && ws == L && clwsb))
	{
		void* ret = origlua_call(L, nargs, nresults);

		return ret;
	}

	if (ws && clws == L)
	{
		void*bak = clws;
		clws = 0;

		origlua_call(ws, nargs, nresults);
		clws = (lua_State*)bak;
	}
	return origlua_call(L, nargs, nresults);
}

//#define lua_call(L,n,r)  lua_call(L, (n), (r), 0, NULL)

int __cdecl  hooked_lua_pcall(lua_State *L, int nargs, int nresults, int errfunc)
{
	sheet(71);

	if (coolbridge_enabled(L) || (ws && ws == L && clwsb))
	{

		int ret = origlua_pcall(ws, nargs, nresults, errfunc);


		return ret;
	}

	if (ws && clws == L)
	{
		void*bak = clws;
		clws = 0;
		origlua_pcall(ws, nargs, nresults, errfunc);
		clws = (lua_State*)bak;
	}
	return origlua_pcall(L, nargs, nresults, errfunc);
}

/*
** garbage-collection function and options
*/


int __cdecl  hooked_lua_gc(lua_State *L, int what, int data)
{

	sheet(72);

	if (coolbridge_enabled(L))
	{

		return origlua_gc(ws, what, data);
	}

	if (ws && clws == L)
	{
		origlua_gc(ws, what, data);
	}

	return origlua_gc(L, what, data);
}



/*
** miscellaneous functions
*/


int __cdecl  hooked_lua_next(lua_State *L, int idx)
{

	sheet(73);

	if (coolbridge_enabled(L))
	{

		return origlua_next(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_next(ws, idx);
	}

	return origlua_next(L, idx);
}


void* __cdecl  hooked_lua_concat(lua_State *L, int n)
{

	sheet(74);

	if (coolbridge_enabled(L))
	{

		return origlua_concat(ws, n);
	}

	if (ws && clws == L)
	{
		origlua_concat(ws, n);
	}

	return origlua_concat(L, n);
}

void* __cdecl  hooked_lua_len(lua_State *L, int idx)
{

	sheet(75);

	if (coolbridge_enabled(L))
	{

		return origlua_len(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_len(ws, idx);
	}

	return origlua_len(L, idx);
}

void *__cdecl hooked_luaL_buffinit(lua_State *L, void *luabuffB)
{

	sheet(76);


	if (coolbridge_enabled(L))
	{

		return origluaL_buffinit(ws, luabuffB);
	}

	if (ws && clws == L)
	{
		origluaL_buffinit(ws, luabuffB);
	}

	return origluaL_buffinit(L, luabuffB);
}

void *__cdecl hooked_luaL_optnumber(lua_State *L, int narg, lua_Number d)
{

	sheet(77);


	if (coolbridge_enabled(L))
	{

		return origluaL_optnumber(ws, narg, d);
	}

	if (ws && clws == L)
	{
		origluaL_optnumber(ws, narg, d);
	}

	return origluaL_optnumber(L, narg, d);
}

void *__cdecl hooked_lua_lessthan(lua_State *L, int idx1, int idx2)
{

	sheet(78);


	if (coolbridge_enabled(L))
	{

		return origlua_lessthan(ws, idx1, idx2);
	}

	if (ws && clws == L)
	{
		origlua_lessthan(ws, idx1, idx2);
	}

	return origlua_lessthan(L, idx1, idx2);
}


void *__cdecl hooked_lua_equal(lua_State *L, int idx, int idx2)
{

	sheet(79);


	if (coolbridge_enabled(L))
	{

		return origlua_equal(ws, idx, idx2);
	}

	if (ws && clws == L)
	{
		origlua_equal(ws, idx, idx2);
	}

	return origlua_equal(L, idx, idx2);
}

void *__cdecl hooked_lua_error(lua_State *L)
{

	sheet(80);


	if (coolbridge_enabled(L))
	{

		return origlua_error(ws);
	}

	if (ws && clws == L)
	{
		origlua_error(ws);
	}

	return origlua_error(L);
}

void *__cdecl hooked_lua_getfenv(lua_State *L, int idx)
{

	sheet(81);


	if (coolbridge_enabled(L))
	{

		return origlua_getfenv(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_getfenv(ws, idx);
	}

	return origlua_getfenv(L, idx);
}

void *__cdecl hooked_lua_cpcall(lua_State *L, lua_CFunction func, void* ud)
{

	sheet(82);


	if (coolbridge_enabled(L))
	{

		return origlua_cpcall(ws, func, ud);
	}

	if (ws && clws == L)
	{
		origlua_cpcall(ws, func, ud);
	}

	return origlua_cpcall(L, func, ud);
}

void *__cdecl hooked_luaL_optlstring(lua_State *L, int narg, const char*d, size_t*l)
{

	sheet(83);


	if (coolbridge_enabled(L))
	{

		return origluaL_optlstring(ws, narg, d, l);
	}

	if (ws && clws == L)
	{
		origluaL_optlstring(ws, narg, d, l);
	}

	return origluaL_optlstring(L, narg, d, l);
}

void *__cdecl hooked_lua_getupvalue(lua_State *L, int idx, int n)
{

	sheet(84);


	if (coolbridge_enabled(L))
	{

		return origlua_getupvalue(ws, idx, n);
	}

	if (ws && clws == L)
	{
		origlua_getupvalue(ws, idx, n);
	}

	return origlua_getupvalue(L, idx, n);
}

void *__cdecl hooked_luaL_checkudata(lua_State *L, int idx, const char*tname)
{

	sheet(85);


	if (coolbridge_enabled(L))
	{

		return origluaL_checkudata(ws, idx, tname);
	}

	if (ws && clws == L)
	{
		origluaL_checkudata(ws, idx, tname);
	}

	return origluaL_checkudata(L, idx, tname);
}


void *__cdecl hooked_lua_gethook(lua_State *L)
{

	sheet(86);


	if (coolbridge_enabled(L))
	{

		return origlua_gethook(ws);
	}

	if (ws && clws == L)
	{
		origlua_gethook(ws);
	}

	return origlua_gethook(L);
}

void *__cdecl hooked_luaL_checktype(lua_State *L, int idx, int t)
{

	sheet(87);


	if (coolbridge_enabled(L))
	{

		return origluaL_checktype(ws, idx, t);
	}

	if (ws && clws == L)
	{
		origluaL_checktype(ws, idx, t);
	}

	return origluaL_checktype(L, idx, t);
}

void *__cdecl hooked_lua_status(lua_State *L)
{

	sheet(88);


	if (coolbridge_enabled(L))
	{

		return origlua_status(ws);
	}

	if (ws && clws == L)
	{
		origlua_status(ws);
	}

	return origlua_status(L);
}

void *__cdecl hooked_lua_sethook(lua_State *L, int idx, int mask, int count)
{

	sheet(89);


	if (coolbridge_enabled(L))
	{

		return origlua_sethook(ws, idx, mask, count);
	}

	if (ws && clws == L)
	{
		origlua_sethook(ws, idx, mask, count);
	}

	return origlua_sethook(L, idx, mask, count);
}

void *__cdecl hooked_lua_setupvalue(lua_State *L, int idx, int n)
{

	sheet(90);


	if (coolbridge_enabled(L))
	{

		return origlua_setupvalue(ws, idx, n);
	}

	if (ws && clws == L)
	{
		origlua_setupvalue(ws, idx, n);
	}

	return origlua_setupvalue(L, idx, n);
}

void *__cdecl hooked_luaL_where(lua_State *L, int idx)
{

	sheet(91);


	if (coolbridge_enabled(L))
	{

		return origluaL_where(ws, idx);
	}

	if (ws && clws == L)
	{
		origluaL_where(ws, idx);
	}

	return origluaL_where(L, idx);
}

void *__cdecl hooked_luaL_execresult(lua_State *L, int idx)
{

	sheet(92);


	if (coolbridge_enabled(L))
	{

		return origluaL_execresult(ws, idx);
	}

	if (ws && clws == L)
	{
		origluaL_execresult(ws, idx);
	}

	return origluaL_execresult(L, idx);
}

void *__cdecl hooked_lua_setallocf(lua_State *L, lua_Alloc *f, void*ud)
{

	sheet(93);


	if (coolbridge_enabled(L))
	{

		return origlua_setallocf(ws, f, ud);
	}

	if (ws && clws == L)
	{
		//	origlua_setallocf(ws, f, ud);
	}

	return origlua_setallocf(L, f, ud);
}

void *__cdecl hooked_lua_gethookmask(lua_State *L)
{

	sheet(94);


	if (coolbridge_enabled(L))
	{

		return origlua_gethookmask(ws);
	}

	if (ws && clws == L)
	{
		origlua_gethookmask(ws);
	}

	return origlua_gethookmask(L);
}

void *__cdecl hooked_lua_getstack(lua_State *L, int idx, lua_Debug *ar)
{

	sheet(95);


	if (coolbridge_enabled(L))
	{

		return origlua_getstack(ws, idx, ar);
	}

	if (ws && clws == L)
	{
		origlua_getstack(ws, idx, ar);
	}

	return origlua_getstack(L, idx, ar);
}

void *__cdecl hooked_lua_resume_real(lua_State *L, int idx)
{

	sheet(96);


	if (coolbridge_enabled(L))
	{

		return origlua_resume_real(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_resume_real(ws, idx);
	}

	return origlua_resume_real(L, idx);
}

void *__cdecl hooked_luaL_checknumber(lua_State *L, int idx)
{

	sheet(97);


	if (coolbridge_enabled(L))
	{

		return origluaL_checknumber(ws, idx);
	}

	if (ws && clws == L)
	{
		origluaL_checknumber(ws, idx);
	}

	return origluaL_checknumber(L, idx);
}

void *__cdecl hooked_luaL_traceback(lua_State *L, int a2, int a3, int a4)
{

	sheet(98);


	if (coolbridge_enabled(L))
	{

		return origluaL_traceback(ws, a2, a3, a4);
	}

	if (ws && clws == L)
	{
		origluaL_traceback(ws, a2, a3, a4);
	}

	return origluaL_traceback(L, a2, a3, a4);
}

void *__cdecl hooked_luaL_gsub(lua_State *L, const char *s, const char *p, const char *r)
{

	sheet(99);


	if (coolbridge_enabled(L))
	{

		return origluaL_gsub(ws, s, p, r);
	}

	if (ws && clws == L)
	{
		origluaL_gsub(ws, s, p, r);
	}

	return origluaL_gsub(L, s, p, r);
}

void *__cdecl hooked_luaL_typerror(lua_State *L, int idx, const char *tname)
{

	sheet(100);


	if (coolbridge_enabled(L))
	{

		return origluaL_typerror(ws, idx, tname);
	}

	if (ws && clws == L)
	{
		origluaL_typerror(ws, idx, tname);
	}

	return origluaL_typerror(L, idx, tname);
}

void *__cdecl hooked_lua_yield(lua_State *L, int idx)
{

	sheet(101);


	if (coolbridge_enabled(L))
	{

		return origlua_yield(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_yield(ws, idx);
	}

	return origlua_yield(L, idx);
}

void *__cdecl hooked_lua_upvalueid(lua_State *L, int idx, int n)
{

	sheet(102);


	if (coolbridge_enabled(L))
	{

		return origlua_upvalueid(ws, idx, n);
	}

	if (ws && clws == L)
	{
		origlua_upvalueid(ws, idx, n);
	}

	return origlua_upvalueid(L, idx, n);
}

void *__cdecl hooked_luaL_optinteger(lua_State *L, int idx, lua_Integer d)
{

	sheet(103);


	if (coolbridge_enabled(L))
	{

		return origluaL_optinteger(ws, idx, d);
	}

	if (ws && clws == L)
	{
		origluaL_optinteger(ws, idx, d);
	}

	return origluaL_optinteger(L, idx, d);
}


void *__cdecl hooked_luaJIT_setmode(lua_State *L, int idx, int mode)
{

	sheet(104);


	if (coolbridge_enabled(L))
	{

		return origluaJIT_setmode(ws, idx, mode);
	}

	if (ws && clws == L)
	{
		origluaJIT_setmode(ws, idx, mode);
	}

	return origluaJIT_setmode(L, idx, mode);
}

void *__cdecl hooked_luaL_checkinteger(lua_State *L, int idx)
{

	sheet(105);


	if (coolbridge_enabled(L))
	{

		return origluaL_checkinteger(ws, idx);
	}

	if (ws && clws == L)
	{
		origluaL_checkinteger(ws, idx);
	}

	return origluaL_checkinteger(L, idx);
}

void *__cdecl hooked_luaL_checkstack(lua_State *L, int idx, const char *msg)
{

	sheet(106);


	if (coolbridge_enabled(L))
	{

		return origluaL_checkstack(ws, idx, msg);
	}

	if (ws && clws == L)
	{
		origluaL_checkstack(ws, idx, msg);
	}

	return origluaL_checkstack(L, idx, msg);
}

void *__cdecl hooked_lua_setlocal(lua_State *L, const lua_Debug *ar, int n)
{

	sheet(107);


	if (coolbridge_enabled(L))
	{

		return origlua_setlocal(ws, ar, n);
	}

	if (ws && clws == L)
	{
		origlua_setlocal(ws, ar, n);
	}

	return origlua_setlocal(L, ar, n);
}

void *__cdecl hooked_luaL_checklstring(lua_State *L, int idx, size_t *l)
{

	sheet(108);


	if (coolbridge_enabled(L))
	{

		return origluaL_checklstring(ws, idx, l);
	}

	if (ws && clws == L)
	{
		origluaL_checklstring(ws, idx, l);
	}

	return origluaL_checklstring(L, idx, l);
}

void *__cdecl hooked_lua_dump(lua_State *L, lua_Writer *idx, void*data)
{

	sheet(109);


	if (coolbridge_enabled(L))
	{

		return origlua_dump(ws, idx, data);
	}

	if (ws && clws == L)
	{
		origlua_dump(ws, idx, data);
	}

	return origlua_dump(L, idx, data);
}



void *__cdecl hooked_lua_upvaluejoin(lua_State *L, int a2, int a3, int a4, int a5)
{

	sheet(110);


	if (coolbridge_enabled(L))
	{

		return origlua_upvaluejoin(ws, a2, a3, a4, a5);
	}

	if (ws && clws == L)
	{
		origlua_upvaluejoin(ws, a2, a3, a4, a5);
	}

	return origlua_upvaluejoin(L, a2, a3, a4, a5);
}

void *__cdecl hooked_luaL_checkany(lua_State *L, int idx)
{

	sheet(111);


	if (coolbridge_enabled(L))
	{

		return origluaL_checkany(ws, idx);
	}

	if (ws && clws == L)
	{
		origluaL_checkany(ws, idx);
	}

	return origluaL_checkany(L, idx);
}

void *__cdecl hooked_luaL_findtable(lua_State *L, int a2, char *a3, int a4)
{

	sheet(112);


	if (coolbridge_enabled(L))
	{

		return origluaL_findtable(ws, a2, a3, a4);
	}

	if (ws && clws == L)
	{
		origluaL_findtable(ws, a2, a3, a4);
	}

	return origluaL_findtable(L, a2, a3, a4);
}

void *__cdecl hooked_lua_gethookcount(lua_State *L)
{

	sheet(113);


	if (coolbridge_enabled(L))
	{

		return origlua_gethookcount(ws);
	}

	if (ws && clws == L)
	{
		origlua_gethookcount(ws);
	}

	return origlua_gethookcount(L);
}


void *__cdecl hooked_luaL_callmeta(lua_State *L, int obj, const char *e)
{

	sheet(114);


	if (coolbridge_enabled(L))
	{

		return origluaL_callmeta(ws, obj, e);
	}

	if (ws && clws == L)
	{
		origluaL_callmeta(ws, obj, e);
	}

	return origluaL_callmeta(L, obj, e);
}

void *__cdecl hooked_luaL_fileresult(lua_State *L, int a2, int a3)
{

	sheet(115);


	if (coolbridge_enabled(L))
	{

		return origluaL_fileresult(ws, a2, a3);
	}

	if (ws && clws == L)
	{
		origluaL_fileresult(ws, a2, a3);
	}

	return origluaL_fileresult(L, a2, a3);
}

void *__cdecl hooked_luaL_getmetafield(lua_State *L, int idx, const char *e)
{

	sheet(116);


	if (coolbridge_enabled(L))
	{

		return origluaL_getmetafield(ws, idx, e);
	}

	if (ws && clws == L)
	{
		origluaL_getmetafield(ws, idx, e);
	}

	return origluaL_getmetafield(L, idx, e);
}



void *__cdecl hooked_luaL_checkoption(lua_State *L, int narg, const char *def, const char *const lst[])
{

	sheet(117);


	if (coolbridge_enabled(L))
	{

		return origluaL_checkoption(ws, narg, def, lst);
	}

	if (ws && clws == L)
	{
		origluaL_checkoption(ws, narg, def, lst);
	}

	return origluaL_checkoption(L, narg, def, lst);
}

void *__cdecl hooked_luaL_newmetatable_type(lua_State *L, char* name, int a3)
{
	sheet(118);


	if (coolbridge_enabled(L))
	{
		return origluaL_newmetatable_type(ws, name, a3);
	}

	if (ws && clws == L)
	{
		void*bak = clws;
		clws = 0;

		void* ourret = origluaL_newmetatable_type(ws, name, a3);
		void* ret = origluaL_newmetatable_type(L, name, a3);
		printf("NAME: %s[%d] _ [%d _ %d] __ %d = %d\n", name, a3, ret, ourret, *(double *)*(intptr_t *)((intptr_t)L + 20), *(double *)*(intptr_t *)((intptr_t)ws + 20));
		*(double *)*(intptr_t *)((intptr_t)ws + 20) = *(double *)*(intptr_t *)((intptr_t)L + 20);

		clws = (lua_State*)bak;
		return ret;
	}
	return origluaL_newmetatable_type(L, name, a3);
}


void *__cdecl hooked_luaL_error(lua_State *L, const char* fmt, ...)
{
	char buffer[2048] = { 0 };

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	perror(buffer);
	va_end(args);


	sheet(119);

	if (coolbridge_enabled(L))
	{

		return origluaL_error(ws, buffer);
	}

	if (ws && clws == L)
	{
		origluaL_error(ws, buffer);
	}


	void* ret = (void*)origluaL_error(L, buffer);

	return (void*)ret;
}

void *__cdecl hooked_lua_setfenv(lua_State *L, int idx)
{

	sheet(120);


	if (coolbridge_enabled(L))
	{

		return origlua_setfenv(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_setfenv(ws, idx);
	}

	return origlua_setfenv(L, idx);
}

void *__cdecl hooked_luaL_register(lua_State *L, const char*lib, const void* l)
{

	sheet(121);


	if (coolbridge_enabled(L))
	{

		return origluaL_register(ws, lib, l);
	}

	if (ws && clws == L)
	{
		origluaL_register(ws, lib, l);
	}

	return origluaL_register(L, lib, l);
}

void *__cdecl hooked_luaL_argerror(lua_State *L, int narg, const char* extramsg)
{

	sheet(122);


	if (coolbridge_enabled(L))
	{

		return origluaL_argerror(ws, narg, extramsg);
	}

	if (ws && clws == L)
	{
		origluaL_argerror(ws, narg, extramsg);
	}

	return origluaL_argerror(L, narg, extramsg);
}

void *__cdecl hooked_lua_getinfo(lua_State *L, const char* what, lua_Debug *ar)
{

	sheet(123);


	if (coolbridge_enabled(L))
	{

		return origlua_getinfo(ws, what, ar);
	}

	if (ws && clws == L)
	{
		origlua_getinfo(ws, what, ar);
	}

	return origlua_getinfo(L, what, ar);
}

void *__cdecl hooked_lua_getallocf(lua_State *L, void* ud)
{

	sheet(124);


	if (coolbridge_enabled(L))
	{

		return origlua_getallocf(ws, ud);
	}

	if (ws && clws == L)
	{
		origlua_getallocf(ws, ud);
	}

	return origlua_getallocf(L, ud);
}

void *__cdecl hooked_lua_objlen(lua_State *L, int idx)
{

	sheet(125);


	if (coolbridge_enabled(L))
	{

		return origlua_objlen(ws, idx);
	}

	if (ws && clws == L)
	{
		origlua_objlen(ws, idx);
	}

	return origlua_objlen(L, idx);
}


void *__cdecl hooked_luaL_ref(lua_State *L, int idx)
{
	sheet(126);


	if (coolbridge_enabled(L))
	{
		int ret = (int)origluaL_ref(ws, idx);


		return (void*)ret;
	}

	if (ws && clws == L)
	{
		//printf("A_1: %d __ %d \n", origlua_gettop(L), origlua_gettop(ws));
		if (origlua_gettop(L) != origlua_gettop(ws))
		{
			MessageBoxA(NULL, "rip", "k", MB_OK);
		}
		void* bak = clws;
		clws = 0;
		int hisref = (int)origluaL_ref(L, idx);
		int ourref = (int)origluaL_ref(ws, idx);


		clws = (lua_State*)bak;
		if (origlua_gettop(L) != origlua_gettop(ws))
		{

			printf("A: %d __ %d == %d __ %d\n", origlua_gettop(L), origlua_gettop(ws), hisref, ourref);
			MessageBoxA(NULL, "rip", "k", MB_OK);
		}
		if (hisref != ourref)
		{

			printf("A: %d __ %d == %d __ %d\n", origlua_gettop(L), origlua_gettop(ws), hisref, ourref);
			MessageBoxA(NULL, "rip", "k", MB_OK);
		}
		return (void*)hisref;

		/*
		void*ourref = origluaL_ref(ws, idx);
		void*hisref = origluaL_ref(L, idx);

		ourrefs[ref_count] = (int)ourref;
		hisrefs[ref_count] = (int)hisref;
		ref_count += 1;

		return hisref;*/


	}

	int ret = (int)origluaL_ref(L, idx);;
	return (void*)ret;
}


void *__cdecl hooked_luaL_unref(lua_State *L, int idx, int ref)
{
	sheet(127);


	if (coolbridge_enabled(L))
	{
		return origluaL_unref(ws, idx, ref);
	}

	if (ws && clws == L)
	{
		origluaL_unref(ws, idx, ref);
		void* bak = clws;
		clws = 0;
		void * ret = origluaL_unref(L, idx, ref);
		clws = (lua_State*)bak;
		return ret;

	}

	return origluaL_unref(L, idx, ref);
}

//hooks end

#include "hde\hde32.h"

void* MyDetour(void* tohook, void*hookfunc, unsigned int safetycopy, bool pushret)
{

#ifdef _HDE32_H_

	if (safetycopy == 0)
	{
		uint32_t hooklen = 6;
		if (!pushret)
			hooklen += 1;

		hde32s disasm;

		while (safetycopy < hooklen)
		{
			void* curaddr = (void*)((uint32_t)tohook + safetycopy);
			safetycopy += hde32_disasm(curaddr, &disasm);
		}
	}

#endif

	unsigned long oldprot = 0;
	unsigned long trash = 0;

	char *_tohook = (char*)tohook;
	char *_hookfunc = (char*)hookfunc;


	unsigned int allocsize = 20 + safetycopy + 20;

	char*backup = (char*)VirtualAlloc(0, allocsize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	memset(backup, 0x90, allocsize);
	memcpy(backup + 20, tohook, safetycopy);

	char *backup_jmpback = (backup + 20 + safetycopy);

	if (pushret)
	{

		backup_jmpback[0] = '\x68';
		backup_jmpback[4 + 1] = '\xC3';
	}
	else {
		backup_jmpback[0] = '\xB8';
		backup_jmpback[4 + 1] = '\xFF';
		backup_jmpback[4 + 1 + 1] = '\xE0';
	}
	*(unsigned int*)(backup_jmpback + 1) = (unsigned int)tohook + safetycopy; // the addr after our overwrite


																			  //actual hookenz
	VirtualProtect(_tohook, 0x400, PAGE_EXECUTE_READWRITE, &oldprot);

	memset(tohook, 0x90, safetycopy);
	_tohook[0] = '\x68';//push
	*(unsigned int*)(_tohook + 1) = (unsigned int)hookfunc; // the addr after our overwrite
	_tohook[5] = '\xC3'; //ret


	VirtualProtect(_tohook, 0x400, oldprot, &trash);

	return backup;
}

void lua_hookshit()
{
	HMODULE mod = GetModuleHandle("lua_shared.dll");

	void* ptrlua_newstate = GetProcAddress(mod, "lua_newstate");
	void* ptrlua_close = GetProcAddress(mod, "lua_close");
	void* ptrlua_newthread = GetProcAddress(mod, "lua_newthread");
	void* ptrlua_atpanic = GetProcAddress(mod, "lua_atpanic");
	void* ptrlua_gettop = GetProcAddress(mod, "lua_gettop");
	void* ptrlua_settop = GetProcAddress(mod, "lua_settop");
	void* ptrlua_pushvalue = GetProcAddress(mod, "lua_pushvalue");
	void* ptrlua_remove = GetProcAddress(mod, "lua_remove");
	void* ptrlua_insert = GetProcAddress(mod, "lua_insert");
	void* ptrlua_replace = GetProcAddress(mod, "lua_replace");
	void* ptrlua_checkstack = GetProcAddress(mod, "lua_checkstack");
	void* ptrlua_xmove = GetProcAddress(mod, "lua_xmove");
	void* ptrlua_isnumber = GetProcAddress(mod, "lua_isnumber");
	void* ptrlua_isstring = GetProcAddress(mod, "lua_isstring");
	void* ptrlua_iscfunction = GetProcAddress(mod, "lua_iscfunction");
	void* ptrlua_isuserdata = GetProcAddress(mod, "lua_isuserdata");
	void* ptrlua_type = GetProcAddress(mod, "lua_type");
	void* ptrlua_typename = GetProcAddress(mod, "lua_typename");
	void* ptrlua_tonumberx = GetProcAddress(mod, "lua_tonumber");
	void* ptrlua_tointegerx = GetProcAddress(mod, "lua_tointeger");
	void* ptrlua_toboolean = GetProcAddress(mod, "lua_toboolean");
	void* ptrlua_tolstring = GetProcAddress(mod, "lua_tolstring");
	void* ptrlua_tocfunction = GetProcAddress(mod, "lua_tocfunction");
	void* ptrlua_touserdata = GetProcAddress(mod, "lua_touserdata");
	void* ptrlua_tothread = GetProcAddress(mod, "lua_tothread");
	void* ptrlua_topointer = GetProcAddress(mod, "lua_topointer");
	void* ptrlua_rawequal = GetProcAddress(mod, "lua_rawequal");
	void* ptrlua_pushnil = GetProcAddress(mod, "lua_pushnil");
	void* ptrlua_pushnumber = GetProcAddress(mod, "lua_pushnumber");
	void* ptrlua_pushinteger = GetProcAddress(mod, "lua_pushinteger");
	void* ptrlua_pushlstring = GetProcAddress(mod, "lua_pushlstring");
	void* ptrlua_pushstring = GetProcAddress(mod, "lua_pushstring");
	void* ptrlua_pushvfstring = GetProcAddress(mod, "lua_pushvfstring");
	void* ptrlua_pushfstring = GetProcAddress(mod, "lua_pushfstring");
	void* ptrlua_pushcclosure = GetProcAddress(mod, "lua_pushcclosure");
	void* ptrlua_pushboolean = GetProcAddress(mod, "lua_pushboolean");
	void* ptrlua_pushlightuserdata = GetProcAddress(mod, "lua_pushlightuserdata");
	void* ptrlua_pushthread = GetProcAddress(mod, "lua_pushthread");
	void* ptrlua_gettable = GetProcAddress(mod, "lua_gettable");
	void* ptrlua_getfield = GetProcAddress(mod, "lua_getfield");
	void* ptrlua_rawget = GetProcAddress(mod, "lua_rawget");
	void* ptrlua_rawgeti = GetProcAddress(mod, "lua_rawgeti");
	void* ptrlua_createtable = GetProcAddress(mod, "lua_createtable");
	void* ptrlua_newuserdata = GetProcAddress(mod, "lua_newuserdata");
	void* ptrlua_getmetatable = GetProcAddress(mod, "lua_getmetatable");
	void* ptrlua_settable = GetProcAddress(mod, "lua_settable");
	void* ptrlua_setfield = GetProcAddress(mod, "lua_setfield");
	void* ptrlua_rawset = GetProcAddress(mod, "lua_rawset");
	void* ptrlua_rawseti = GetProcAddress(mod, "lua_rawseti");
	void* ptrlua_setmetatable = GetProcAddress(mod, "lua_setmetatable");
	void* ptrlua_call = GetProcAddress(mod, "lua_call");
	void* ptrlua_pcall = GetProcAddress(mod, "lua_pcall");
	void* ptrlua_gc = GetProcAddress(mod, "lua_gc");
	void* ptrlua_next = GetProcAddress(mod, "lua_next");
	void* ptrlua_concat = GetProcAddress(mod, "lua_concat");
	void* ptrlua_len = GetProcAddress(mod, "lua_objlen");

	void* ptrluaL_openlibs = GetProcAddress(mod, "luaL_openlibs");
	void* ptrluaL_openlib = GetProcAddress(mod, "luaL_openlib");
	void* ptrluaopen_base = GetProcAddress(mod, "luaopen_base");
	void* ptrluaopen_bit = GetProcAddress(mod, "luaopen_bit");
	void* ptrluaopen_debug = GetProcAddress(mod, "luaopen_debug");
	void* ptrluaopen_jit = GetProcAddress(mod, "luaopen_jit");
	void* ptrluaopen_math = GetProcAddress(mod, "luaopen_math");
	void* ptrluaopen_package = GetProcAddress(mod, "luaopen_package");
	void* ptrluaopen_string = GetProcAddress(mod, "luaopen_string");
	void* ptrluaopen_table = GetProcAddress(mod, "luaopen_table");
	void* ptrluaopen_os = GetProcAddress(mod, "luaopen_os");

	void *ptrluaL_newmetatable = GetProcAddress(mod, "luaL_newmetatable");

	void* ptrluaL_buffinit = GetProcAddress(mod, "luaL_buffinit");
	void* ptrluaL_optnumber = GetProcAddress(mod, "luaL_optnumber");
	void* ptrluaJIT_version_2_0_3 = GetProcAddress(mod, "luaJIT_version_2_0_3");
	void* ptrlua_lessthan = GetProcAddress(mod, "lua_lessthan");
	void* ptrluaL_addstring = GetProcAddress(mod, "luaL_addstring");
	void* ptrluaL_prepbuffer = GetProcAddress(mod, "luaL_prepbuffer");
	void* ptrlua_equal = GetProcAddress(mod, "lua_equal");
	void* ptrlua_error = GetProcAddress(mod, "lua_error");
	void* ptrlua_getfenv = GetProcAddress(mod, "lua_getfenv");
	void* ptrlua_cpcall = GetProcAddress(mod, "lua_cpcall");
	void* ptrluaL_optlstring = GetProcAddress(mod, "luaL_optlstring");
	void* ptrlua_getupvalue = GetProcAddress(mod, "lua_getupvalue");
	void* ptrluaL_checkudata = GetProcAddress(mod, "luaL_checkudata");
	void* ptrlua_getlocal = GetProcAddress(mod, "lua_getlocal");
	void* ptrlua_objlen = GetProcAddress(mod, "lua_objlen");
	void* ptrluaL_ref = GetProcAddress(mod, "luaL_ref");
	void* ptrlua_gethook = GetProcAddress(mod, "lua_gethook");
	void* ptrluaL_checktype = GetProcAddress(mod, "luaL_checktype");
	void* ptrlua_status = GetProcAddress(mod, "lua_status");
	void* ptrluaL_unref = GetProcAddress(mod, "luaL_unref");
	void* ptrlua_sethook = GetProcAddress(mod, "lua_sethook");
	void* ptrlua_setupvalue = GetProcAddress(mod, "lua_setupvalue");
	void* ptrluaL_where = GetProcAddress(mod, "luaL_where");
	void* ptrluaL_execresult = GetProcAddress(mod, "luaL_execresult");
	void* ptrlua_setallocf = GetProcAddress(mod, "lua_setallocf");
	void* ptrlua_gethookmask = GetProcAddress(mod, "lua_gethookmask");
	void* ptrlua_getstack = GetProcAddress(mod, "lua_getstack");
	void* ptrlua_resume_real = GetProcAddress(mod, "lua_resume_real");
	void* ptrluaL_checknumber = GetProcAddress(mod, "luaL_checknumber");
	void* ptrluaL_pushresult = GetProcAddress(mod, "luaL_pushresult");
	void* ptrluaL_traceback = GetProcAddress(mod, "luaL_traceback");
	void* ptrluaL_gsub = GetProcAddress(mod, "luaL_gsub");
	void* ptrluaL_typerror = GetProcAddress(mod, "luaL_typerror");
	void* ptrlua_yield = GetProcAddress(mod, "lua_yield");
	void* ptrlua_upvalueid = GetProcAddress(mod, "lua_upvalueid");
	void* ptrluaL_optinteger = GetProcAddress(mod, "luaL_optinteger");
	void* ptrluaL_addvalue = GetProcAddress(mod, "luaL_addvalue");
	void* ptrluaJIT_setmode = GetProcAddress(mod, "luaJIT_setmode");
	void* ptrluaL_checkinteger = GetProcAddress(mod, "luaL_checkinteger");
	void* ptrluaL_checkstack = GetProcAddress(mod, "luaL_checkstack");
	void* ptrlua_setlocal = GetProcAddress(mod, "lua_setlocal");
	void* ptrluaL_checklstring = GetProcAddress(mod, "luaL_checklstring");
	void* ptrlua_dump = GetProcAddress(mod, "lua_dump");
	void* ptrlua_loadx = GetProcAddress(mod, "lua_loadx");
	void* ptrlua_load = GetProcAddress(mod, "lua_load");
	void* ptrlua_upvaluejoin = GetProcAddress(mod, "lua_upvaluejoin");
	void* ptrluaL_checkany = GetProcAddress(mod, "luaL_checkany");
	void* ptrluaL_findtable = GetProcAddress(mod, "luaL_findtable");
	void* ptrlua_gethookcount = GetProcAddress(mod, "lua_gethookcount");
	void* ptrluaL_newstate = GetProcAddress(mod, "luaL_newstate");
	void* ptrluaL_callmeta = GetProcAddress(mod, "luaL_callmeta");
	void* ptrluaL_fileresult = GetProcAddress(mod, "luaL_fileresult");
	void* ptrluaL_getmetafield = GetProcAddress(mod, "luaL_getmetafield");
	void* ptrluaL_addlstring = GetProcAddress(mod, "luaL_addlstring");
	void* ptrluaL_loadstring = GetProcAddress(mod, "luaL_loadstring");
	void* ptrluaL_checkoption = GetProcAddress(mod, "luaL_checkoption");
	void* ptrluaL_newmetatable_type = GetProcAddress(mod, "luaL_newmetatable_type");
	void* ptrluaL_loadbufferx = GetProcAddress(mod, "luaL_loadbufferx");
	void* ptrluaL_error = GetProcAddress(mod, "luaL_error");
	void* ptrluaL_loadfilex = GetProcAddress(mod, "luaL_loadfilex");
	void* ptrlua_setfenv = GetProcAddress(mod, "lua_setfenv");
	void* ptrluaL_register = GetProcAddress(mod, "luaL_register");
	void* ptrluaL_argerror = GetProcAddress(mod, "luaL_argerror");
	void* ptrlua_getinfo = GetProcAddress(mod, "lua_getinfo");
	void* ptrlua_getallocf = GetProcAddress(mod, "lua_getallocf");


	//hook funcs

	*(void**)&origlua_newstate = MyDetour(ptrlua_newstate, hooked_lua_newstate, 0, true);
	*(void**)&origlua_close = MyDetour(ptrlua_close, hooked_lua_close, 0, true);
	//*(void**)&origlua_newthread = MyDetour(ptrlua_newthread, hooked_lua_newthread, 0, true);
	*(void**)&origlua_atpanic = MyDetour(ptrlua_atpanic, hooked_lua_atpanic, 0, true);
	*(void**)&origlua_gettop = MyDetour(ptrlua_gettop, hooked_lua_gettop, 0, true);
	*(void**)&origlua_settop = MyDetour(ptrlua_settop, hooked_lua_settop, 0, true);
	*(void**)&origlua_pushvalue = MyDetour(ptrlua_pushvalue, hooked_lua_pushvalue, 0, true);
	*(void**)&origlua_remove = MyDetour(ptrlua_remove, hooked_lua_remove, 0, true);
	*(void**)&origlua_insert = MyDetour(ptrlua_insert, hooked_lua_insert, 0, true);
	*(void**)&origlua_replace = MyDetour(ptrlua_replace, hooked_lua_replace, 0, true);
	*(void**)&origlua_checkstack = MyDetour(ptrlua_checkstack, hooked_lua_checkstack, 0, true);
	*(void**)&origlua_xmove = MyDetour(ptrlua_xmove, hooked_lua_xmove, 0, true);
	*(void**)&origlua_isnumber = MyDetour(ptrlua_isnumber, hooked_lua_isnumber, 0, true);
	*(void**)&origlua_isstring = MyDetour(ptrlua_isstring, hooked_lua_isstring, 0, true);
	*(void**)&origlua_iscfunction = MyDetour(ptrlua_iscfunction, hooked_lua_iscfunction, 0, true);
	*(void**)&origlua_isuserdata = MyDetour(ptrlua_isuserdata, hooked_lua_isuserdata, 0, true);
	*(void**)&origlua_type = MyDetour(ptrlua_type, hooked_lua_type, 0, true);
	*(void**)&origlua_typename = MyDetour(ptrlua_typename, hooked_lua_typename, 0, true);
	*(void**)&origlua_tonumberx = MyDetour(ptrlua_tonumberx, hooked_lua_tonumberx, 0, true);
	*(void**)&origlua_tointegerx = MyDetour(ptrlua_tointegerx, hooked_lua_tointegerx, 0, true);
	*(void**)&origlua_toboolean = MyDetour(ptrlua_toboolean, hooked_lua_toboolean, 0, true);
	*(void**)&origlua_tolstring = MyDetour(ptrlua_tolstring, hooked_lua_tolstring, 0, true);
	*(void**)&origlua_tocfunction = MyDetour(ptrlua_tocfunction, hooked_lua_tocfunction, 0, true);
	*(void**)&origlua_touserdata = MyDetour(ptrlua_touserdata, hooked_lua_touserdata, 0, true);
	*(void**)&origlua_tothread = MyDetour(ptrlua_tothread, hooked_lua_tothread, 0, true);
	*(void**)&origlua_topointer = MyDetour(ptrlua_topointer, hooked_lua_topointer, 0, true);
	*(void**)&origlua_rawequal = MyDetour(ptrlua_rawequal, hooked_lua_rawequal, 0, true);
	*(void**)&origlua_pushnil = MyDetour(ptrlua_pushnil, hooked_lua_pushnil, 0, true);
	*(void**)&origlua_pushnumber = MyDetour(ptrlua_pushnumber, hooked_lua_pushnumber, 0, true);
	*(void**)&origlua_pushinteger = MyDetour(ptrlua_pushinteger, hooked_lua_pushinteger, 0, true);
	*(void**)&origlua_pushlstring = MyDetour(ptrlua_pushlstring, hooked_lua_pushlstring, 0, true);
	*(void**)&origlua_pushstring = MyDetour(ptrlua_pushstring, hooked_lua_pushstring, 0, true);
	*(void**)&origlua_pushvfstring = MyDetour(ptrlua_pushvfstring, hooked_lua_pushvfstring, 0, true);
	*(void**)&origlua_pushfstring = MyDetour(ptrlua_pushfstring, hooked_lua_pushfstring, 0, true);
	*(void**)&origlua_pushcclosure = MyDetour(ptrlua_pushcclosure, hooked_lua_pushcclosure, 0, true);
	*(void**)&origlua_pushboolean = MyDetour(ptrlua_pushboolean, hooked_lua_pushboolean, 0, true);
	*(void**)&origlua_pushlightuserdata = MyDetour(ptrlua_pushlightuserdata, hooked_lua_pushlightuserdata, 0, true);
	*(void**)&origlua_pushthread = MyDetour(ptrlua_pushthread, hooked_lua_pushthread, 0, true);
	*(void**)&origlua_gettable = MyDetour(ptrlua_gettable, hooked_lua_gettable, 0, true);
	*(void**)&origlua_getfield = MyDetour(ptrlua_getfield, hooked_lua_getfield, 0, true);
	*(void**)&origlua_rawget = MyDetour(ptrlua_rawget, hooked_lua_rawget, 0, true);
	*(void**)&origlua_rawgeti = MyDetour(ptrlua_rawgeti, hooked_lua_rawgeti, 0, true);
	*(void**)&origlua_createtable = MyDetour(ptrlua_createtable, hooked_lua_createtable, 0, true);
	*(void**)&origlua_newuserdata = MyDetour(ptrlua_newuserdata, hooked_lua_newuserdata, 0, true);
	*(void**)&origlua_getmetatable = MyDetour(ptrlua_getmetatable, hooked_lua_getmetatable, 0, true);
	*(void**)&origlua_settable = MyDetour(ptrlua_settable, hooked_lua_settable, 0, true);
	*(void**)&origlua_setfield = MyDetour(ptrlua_setfield, hooked_lua_setfield, 0, true);
	*(void**)&origlua_rawset = MyDetour(ptrlua_rawset, hooked_lua_rawset, 0, true);
	*(void**)&origlua_rawseti = MyDetour(ptrlua_rawseti, hooked_lua_rawseti, 0, true);
	*(void**)&origlua_setmetatable = MyDetour(ptrlua_setmetatable, hooked_lua_setmetatable, 0, true);
	*(void**)&origlua_call = MyDetour(ptrlua_call, hooked_lua_call, 0, true);
	*(void**)&origlua_pcall = MyDetour(ptrlua_pcall, hooked_lua_pcall, 0, true);
	*(void**)&origlua_gc = MyDetour(ptrlua_gc, hooked_lua_gc, 0, true);
	*(void**)&origlua_next = MyDetour(ptrlua_next, hooked_lua_next, 0, true);
	*(void**)&origlua_concat = MyDetour(ptrlua_concat, hooked_lua_concat, 0, true);
	*(void**)&origlua_len = MyDetour(ptrlua_len, hooked_lua_len, 0, true);




	*(void**)&origluaL_openlibs = MyDetour(ptrluaL_openlibs, hooked_luaL_openlibs, 0, true);
	//*(void**)&origluaL_openlib = MyDetour(ptrluaL_openlib, hooked_luaL_openlib, 0, true);
	/**(void**)&origluaopen_base = MyDetour(ptrluaopen_base, hooked_luaopen_base, 0, true);
	*(void**)&origluaopen_bit = MyDetour(ptrluaopen_bit, hooked_luaopen_bit, 0, true);
	*(void**)&origluaopen_debug = MyDetour(ptrluaopen_debug, hooked_luaopen_debug, 0, true);
	*(void**)&origluaopen_jit = MyDetour(ptrluaopen_jit, hooked_luaopen_jit, 0, true);
	*(void**)&origluaopen_math = MyDetour(ptrluaopen_math, hooked_luaopen_math, 0, true);
	*(void**)&origluaopen_package = MyDetour(ptrluaopen_package, hooked_luaopen_package, 0, true);
	*(void**)&origluaopen_string = MyDetour(ptrluaopen_string, hooked_luaopen_string, 0, true);
	*(void**)&origluaopen_table = MyDetour(ptrluaopen_table, hooked_luaopen_table, 0, true);
	*(void**)&origluaopen_os = MyDetour(ptrluaopen_os, hooked_luaopen_os, 0, true);*/

	//*(void**)&origluaL_newmetatable = MyDetour(ptrluaL_newmetatable, hooked_luaL_newmetatable, 0, true);



	*(void**)&origluaL_buffinit = MyDetour(ptrluaL_buffinit, hooked_luaL_buffinit, 0, true);
	*(void**)&origluaL_optnumber = MyDetour(ptrluaL_optnumber, hooked_luaL_optnumber, 0, true);
	*(void**)&origlua_lessthan = MyDetour(ptrlua_lessthan, hooked_lua_lessthan, 0, true);
	*(void**)&origlua_equal = MyDetour(ptrlua_equal, hooked_lua_equal, 0, true);
	*(void**)&origlua_error = MyDetour(ptrlua_error, hooked_lua_error, 0, true);
	*(void**)&origlua_getfenv = MyDetour(ptrlua_getfenv, hooked_lua_getfenv, 0, true);
	*(void**)&origlua_cpcall = MyDetour(ptrlua_cpcall, hooked_lua_cpcall, 0, true);
	*(void**)&origluaL_optlstring = MyDetour(ptrluaL_optlstring, hooked_luaL_optlstring, 0, true);
	*(void**)&origlua_getupvalue = MyDetour(ptrlua_getupvalue, hooked_lua_getupvalue, 0, true);
	*(void**)&origluaL_checkudata = MyDetour(ptrluaL_checkudata, hooked_luaL_checkudata, 0, true);
	*(void**)&origlua_sethook = MyDetour(ptrlua_sethook, hooked_lua_sethook, 0, true);
	*(void**)&origlua_setupvalue = MyDetour(ptrlua_setupvalue, hooked_lua_setupvalue, 0, true);
	*(void**)&origluaL_where = MyDetour(ptrluaL_where, hooked_luaL_where, 0, true);
	*(void**)&origluaL_execresult = MyDetour(ptrluaL_execresult, hooked_luaL_execresult, 0, true);
	*(void**)&origlua_gethookmask = MyDetour(ptrlua_gethookmask, hooked_lua_gethookmask, 0, true);
	*(void**)&origlua_getstack = MyDetour(ptrlua_getstack, hooked_lua_getstack, 0, true);
	*(void**)&origlua_resume_real = MyDetour(ptrlua_resume_real, hooked_lua_resume_real, 0, true);
	*(void**)&origluaL_checknumber = MyDetour(ptrluaL_checknumber, hooked_luaL_checknumber, 0, true);
	*(void**)&origluaL_traceback = MyDetour(ptrluaL_traceback, hooked_luaL_traceback, 0, true);
	*(void**)&origluaL_gsub = MyDetour(ptrluaL_gsub, hooked_luaL_gsub, 0, true);
	*(void**)&origluaL_typerror = MyDetour(ptrluaL_typerror, hooked_luaL_typerror, 0, true);
	*(void**)&origlua_yield = MyDetour(ptrlua_yield, hooked_lua_yield, 0, true);
	*(void**)&origlua_upvalueid = MyDetour(ptrlua_upvalueid, hooked_lua_upvalueid, 0, true);
	*(void**)&origluaL_optinteger = MyDetour(ptrluaL_optinteger, hooked_luaL_optinteger, 0, true);
	*(void**)&origluaJIT_setmode = MyDetour(ptrluaJIT_setmode, hooked_luaJIT_setmode, 0, true);
	*(void**)&origluaL_checkinteger = MyDetour(ptrluaL_checkinteger, hooked_luaL_checkinteger, 0, true);
	*(void**)&origluaL_checkstack = MyDetour(ptrluaL_checkstack, hooked_luaL_checkstack, 0, true);
	*(void**)&origlua_setlocal = MyDetour(ptrlua_setlocal, hooked_lua_setlocal, 0, true);
	*(void**)&origluaL_checklstring = MyDetour(ptrluaL_checklstring, hooked_luaL_checklstring, 0, true);
	*(void**)&origlua_dump = MyDetour(ptrlua_dump, hooked_lua_dump, 0, true);
	*(void**)&origlua_upvaluejoin = MyDetour(ptrlua_upvaluejoin, hooked_lua_upvaluejoin, 0, true);
	*(void**)&origluaL_checkany = MyDetour(ptrluaL_checkany, hooked_luaL_checkany, 0, true);
	*(void**)&origluaL_findtable = MyDetour(ptrluaL_findtable, hooked_luaL_findtable, 0, true);
	*(void**)&origlua_gethookcount = MyDetour(ptrlua_gethookcount, hooked_lua_gethookcount, 0, true);
	*(void**)&origluaL_callmeta = MyDetour(ptrluaL_callmeta, hooked_luaL_callmeta, 0, true);
	*(void**)&origluaL_fileresult = MyDetour(ptrluaL_fileresult, hooked_luaL_fileresult, 0, true);
	//*(void**)&origluaL_getmetafield = MyDetour(ptrluaL_getmetafield, hooked_luaL_getmetafield, 0, true);
	*(void**)&origluaL_checkoption = MyDetour(ptrluaL_checkoption, hooked_luaL_checkoption, 0, true);
	*(void**)&origluaL_newmetatable_type = MyDetour(ptrluaL_newmetatable_type, hooked_luaL_newmetatable_type, 0, true);
	*(void**)&origluaL_error = MyDetour(ptrluaL_error, hooked_luaL_error, 0, true);
	*(void**)&origlua_setfenv = MyDetour(ptrlua_setfenv, hooked_lua_setfenv, 0, true);
	*(void**)&origluaL_register = MyDetour(ptrluaL_register, hooked_luaL_register, 0, true);
	*(void**)&origluaL_argerror = MyDetour(ptrluaL_argerror, hooked_luaL_argerror, 0, true);
	*(void**)&origlua_getinfo = MyDetour(ptrlua_getinfo, hooked_lua_getinfo, 0, true);
	*(void**)&origlua_setallocf = MyDetour(ptrlua_setallocf, hooked_lua_setallocf, 0, true);
	*(void**)&origlua_getallocf = MyDetour(ptrlua_getallocf, hooked_lua_getallocf, 0, true);

	*(void**)&origlua_status = MyDetour(ptrlua_status, hooked_lua_status, 0, true);
	//*(void**)&origluaL_checktype = MyDetour(ptrluaL_checktype, hooked_luaL_checktype, 0, true);
	*(void**)&origlua_gethook = MyDetour(ptrlua_gethook, hooked_lua_gethook, 0, true);


	//these 3 mofos are funky
	*(void**)&origlua_objlen = MyDetour(ptrlua_objlen, hooked_lua_objlen, 0, true);
	*(void**)&origluaL_ref = MyDetour(ptrluaL_ref, hooked_luaL_ref, 0, true);//calling _ref  for my state = crash
	*(void**)&origluaL_unref = MyDetour(ptrluaL_unref, hooked_luaL_unref, 0, true);//calling _unref for my state =prolly crash2

}