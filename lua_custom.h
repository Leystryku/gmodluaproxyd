#include <string.h>
#include <memory>



#define LUA state->luabase

typedef void* lua_Alloc;
typedef void* lua_CFunction;
typedef double lua_Number;
typedef ptrdiff_t lua_Integer;

typedef unsigned int size_t;
typedef void* lua_Reader;
typedef void* lua_Writer;

class CLuaInterface;
typedef void* lua_State;//size of lua_State is 48  on 32 bit

extern lua_State* ls_client_bak;
extern lua_State* ls_client;
extern lua_State* ls_our;


extern CLuaInterface *cli;
extern CLuaInterface *cour;

typedef int(__cdecl* fnluaL_newmetatable)(lua_State *L, const char *name);
/*
** open bullshits
*/

typedef void* (*fnluaL_openlib)(void *L, const char *libname, const void *l, int nup);
typedef void* (*fnluaL_openlibs)(void *L);
typedef void* (*fnluaopen_base)(void *L);
typedef void* (*fnluaopen_bit)(void *L);
typedef void* (*fnluaopen_debug)(void *L);
typedef void* (*fnluaopen_jit)(void *L);
typedef void* (*fnluaopen_math)(void *L);
typedef void* (*fnluaopen_package)(void *L);
typedef void* (*fnluaopen_string)(void *L);
typedef void* (*fnluaopen_table)(void *L);
typedef void* (*fnluaopen_os)(void *L);


/*
** state manipulation
*/







/*
** state manipulation
*/





//painfull hook pts start
typedef lua_State *(__cdecl* fnlua_newstate)(lua_Alloc f, void *ud);
typedef void*(__cdecl* fnlua_close)(lua_State *L);
typedef lua_State *(__cdecl* fnlua_newthread)(lua_State *L);

typedef lua_CFunction(__cdecl* fnlua_atpanic) (lua_State *L, lua_CFunction panicf);



/*
** basic stack manipulation
*/
typedef int(__cdecl* fnlua_gettop)(lua_State *L);
typedef void*(__cdecl* fnlua_settop)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_pushvalue)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_remove)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_insert)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_replace)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_copy)(lua_State *L, int fromidx, int toidx);
typedef int(__cdecl* fnlua_checkstack)(lua_State *L, int sz);

typedef void*(__cdecl* fnlua_xmove)(lua_State *from, lua_State *to, int n);


/*
** access functions (stack -> C)
*/

typedef int(__cdecl* fnlua_isnumber)(lua_State *L, int idx);
typedef int(__cdecl* fnlua_isstring)(lua_State *L, int idx);
typedef int(__cdecl* fnlua_iscfunction)(lua_State *L, int idx);
typedef int(__cdecl* fnlua_isuserdata)(lua_State *L, int idx);
typedef int(__cdecl* fnlua_type)(lua_State *L, int edx);
typedef const char     *(__cdecl* fnlua_typename)(lua_State *L, int tp, int tp2);

typedef lua_Number(__cdecl* fnlua_tonumberx) (lua_State *L, int idx);
typedef lua_Integer(__cdecl* fnlua_tointegerx) (lua_State *L, int idx);
typedef int(__cdecl* fnlua_toboolean)(lua_State *L, int idx);
typedef const char     *(__cdecl* fnlua_tolstring)(lua_State *L, int idx, size_t *len);
typedef size_t(__cdecl* fnlua_rawlen) (lua_State *L, int idx);
typedef lua_CFunction(__cdecl* fnlua_tocfunction) (lua_State *L, int idx);
typedef void           *(__cdecl* fnlua_touserdata)(lua_State *L, int idx);
typedef lua_State      *(__cdecl* fnlua_tothread)(lua_State *L, int idx);
typedef const void     *(__cdecl* fnlua_topointer)(lua_State *L, int idx);


/*
** Comparison and arithmetic functions
*/

typedef void*(__cdecl* fnlua_arith)(lua_State *L, int op);

typedef int(__cdecl* fnlua_rawequal)(lua_State *L, int idx1, int idx2);
typedef int(__cdecl* fnlua_compare)(lua_State *L, int idx1, int idx2, int op);


/*
** push functions (C -> stack)
*/
typedef void*(__cdecl* fnlua_pushnil)(lua_State *L);
typedef void*(__cdecl* fnlua_pushnumber)(lua_State *L, lua_Number n);
typedef void*(__cdecl* fnlua_pushinteger)(lua_State *L, lua_Integer n);
typedef const char *(__cdecl* fnlua_pushlstring)(lua_State *L, const char *s, size_t l);
typedef const char *(__cdecl* fnlua_pushstring)(lua_State *L, const char *s);
typedef const char *(__cdecl* fnlua_pushvfstring)(lua_State *L, const char *fmt, va_list argp);
typedef const char *(__cdecl* fnlua_pushfstring)(lua_State *L, const char *fmt, ...);
typedef void*(__cdecl* fnlua_pushcclosure)(lua_State *L, lua_CFunction fn, int n);
typedef void*(__cdecl* fnlua_pushboolean)(lua_State *L, int b);
typedef void*(__cdecl* fnlua_pushlightuserdata)(lua_State *L, void *p);
typedef int(__cdecl* fnlua_pushthread)(lua_State *L);


/*
** get functions (Lua -> stack)
*/
typedef void*(__cdecl* fnlua_getglobal)(lua_State *L, const char *var);
typedef void*(__cdecl* fnlua_gettable)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_getfield)(lua_State *L, int idx, const char *k);
typedef void*(__cdecl* fnlua_rawget)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_rawgeti)(lua_State *L, int idx, int n);
typedef void*(__cdecl* fnlua_rawgetp)(lua_State *L, int idx, const void *p);
typedef void*(__cdecl* fnlua_createtable)(lua_State *L, int narr, int nrec);
typedef void *(__cdecl* fnlua_newuserdata)(lua_State *L, size_t sz);
typedef int(__cdecl* fnlua_getmetatable)(lua_State *L, int objindex);


/*
** set functions (stack -> Lua)
*/
typedef void*(__cdecl* fnlua_settable)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_setfield)(lua_State *L, int idx, const char *k);
typedef void*(__cdecl* fnlua_rawset)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_rawseti)(lua_State *L, int idx, int n);
typedef int(__cdecl* fnlua_setmetatable)(lua_State *L, int objindex);


/*
** 'load' and 'call' functions (load and run Lua code)
*/
typedef void*(__cdecl* fnlua_call)(lua_State *L, int nargs, int nresults);
//#define lua_call(L,n,r)         lua_call(L, (n), (r), 0, NULL)

typedef int(__cdecl* fnlua_pcall)(lua_State *L, int nargs, int nresults, int errfunc);
//#define lua_pcall(L,n,r,f)      lua_pcall(L, (n), (r), (f), 0, NULL)


/*
** garbage-collection function and options
*/


typedef int(__cdecl* fnlua_gc)(lua_State *L, int what, int data);


/*
** miscellaneous functions
*/

typedef int(__cdecl* fnlua_next)(lua_State *L, int idx);

typedef void*(__cdecl* fnlua_concat)(lua_State *L, int n);
typedef void*(__cdecl* fnlua_len)(lua_State *L, int idx);

/*
** state manipulation
*/

typedef struct lua_Debug {
	int event;
	const char *name;           /* (n) */
	const char *namewhat;       /* (n) */
	const char *what;           /* (S) */
	const char *source;         /* (S) */
	int currentline;            /* (l) */
	int nups;                   /* (u) number of upvalues */
	int linedefined;            /* (S) */
	int lastlinedefined;        /* (S) */
	char short_src[60]; /* (S) */

} lua_Debug;

//new
typedef void*(__cdecl* fnluaL_buffinit)(lua_State *L, void *luabuffB);
typedef void*(__cdecl* fnluaL_optnumber)(lua_State *L, int narg, lua_Number d);
typedef void*(__cdecl* fnlua_lessthan)(lua_State *L, int index1, int index2);
typedef void*(__cdecl* fnlua_equal)(lua_State *L, int index1, int index2);
typedef void*(__cdecl* fnlua_error)(lua_State *L);
typedef void*(__cdecl* fnlua_getfenv)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_cpcall)(lua_State *L, lua_CFunction func, void* ud);
typedef void*(__cdecl* fnluaL_optlstring)(lua_State *L, int narg, const char*d, size_t*l);
typedef void*(__cdecl* fnlua_getupvalue)(lua_State *L, int funcindex, int n);
typedef void*(__cdecl* fnluaL_checkudata)(lua_State *L, int narg, const char *tname);
typedef void*(__cdecl* fnlua_objlen)(lua_State *L, int idx);
typedef void*(__cdecl* fnluaL_ref)(lua_State *L, int idx);
typedef void*(__cdecl* fnlua_gethook)(lua_State *L);
typedef void*(__cdecl* fnluaL_checktype)(lua_State *L, int narg, int t);
typedef void*(__cdecl* fnlua_status)(lua_State *L);
typedef void*(__cdecl* fnluaL_unref)(lua_State *L, int t, int ref);
typedef void*(__cdecl* fnlua_sethook)(lua_State *L, int f, int mask, int count);
typedef void*(__cdecl* fnlua_setupvalue)(lua_State *L, int index, int n);
typedef void*(__cdecl* fnluaL_where)(lua_State *L, int lv);
typedef void*(__cdecl* fnluaL_execresult)(lua_State *L, int stat);
typedef void*(__cdecl* fnlua_setallocf)(lua_State *L, lua_Alloc f, void* ud);
typedef void*(__cdecl* fnlua_gethookmask)(lua_State *L);
typedef void*(__cdecl* fnlua_getstack)(lua_State *L, int lv, lua_Debug* ar);
typedef void*(__cdecl* fnlua_resume_real)(lua_State *L, int idx);
typedef void*(__cdecl* fnluaL_checknumber)(lua_State *L, int narg);
typedef void*(__cdecl* fnluaL_traceback)(lua_State *L, int a2, int a3, int a4);
typedef void*(__cdecl* fnluaL_gsub)(lua_State *L, const char *s, const char *p, const char *r);
typedef void*(__cdecl* fnluaL_typerror)(lua_State *L, int narg, const char *tname);
typedef void*(__cdecl* fnlua_yield)(lua_State *L, int res);
typedef void*(__cdecl* fnlua_upvalueid)(lua_State *L, int idx, int n);
typedef void*(__cdecl* fnluaL_optinteger)(lua_State *L, int narg, lua_Integer d);
typedef void*(__cdecl* fnluaJIT_setmode)(lua_State *L, int idx, int mode);
typedef void*(__cdecl* fnluaL_checkinteger)(lua_State *L, int narg);
typedef void*(__cdecl* fnluaL_checkstack)(lua_State *L, int sz, const char *msg);
typedef void*(__cdecl* fnlua_setlocal)(lua_State *L, const lua_Debug *ar, int n);
typedef void*(__cdecl* fnluaL_checklstring)(lua_State *L, int narg, size_t *l);
typedef void*(__cdecl* fnlua_dump)(lua_State *L, lua_Writer writer, void *data);
typedef void*(__cdecl* fnlua_upvaluejoin)(lua_State *L, int a2, int a3, int a4, int a5);
typedef void*(__cdecl* fnluaL_checkany)(lua_State *L, int narg);
typedef void*(__cdecl* fnluaL_findtable)(lua_State *L, int a2, char *a3, int a4);
typedef void*(__cdecl* fnlua_gethookcount)(lua_State *L);
typedef void*(__cdecl* fnluaL_callmeta)(lua_State *L, int obj, const char *e);
typedef void*(__cdecl* fnluaL_fileresult)(lua_State *L, int a2, int a3);
typedef void*(__cdecl* fnluaL_getmetafield)(lua_State *L, int obj, const char *e);
typedef void*(__cdecl* fnluaL_checkoption)(lua_State *L, int narg, const char *def, const char *const lst[]);
typedef void*(__cdecl* fnluaL_newmetatable_type)(lua_State *L, void *a2, int a3);
typedef void*(__cdecl* fnluaL_error)(lua_State *L, const char* fmt, ...);
typedef void*(__cdecl* fnlua_setfenv)(lua_State *L, int idx);
typedef void*(__cdecl* fnluaL_register)(lua_State *L, const char*lib, const void* l);
typedef void*(__cdecl* fnluaL_argerror)(lua_State *L, int narg, const char*extramsg);
typedef void*(__cdecl* fnlua_getinfo)(lua_State *L, const char*what, lua_Debug *ar);
typedef void*(__cdecl* fnlua_getallocf)(lua_State *L, void*ud);

extern fnluaL_openlibs origluaL_openlibs;
extern fnluaL_openlib origluaL_openlib;
extern fnluaopen_base origluaopen_base;
extern fnluaopen_bit origluaopen_bit;
extern fnluaopen_debug origluaopen_debug;
extern fnluaopen_jit origluaopen_jit;
extern fnluaopen_math origluaopen_math;
extern fnluaopen_package origluaopen_package;
extern fnluaopen_string origluaopen_string;
extern fnluaopen_table origluaopen_table;
extern fnluaopen_os origluaopen_os;

extern fnluaL_newmetatable origluaL_newmetatable;

extern fnlua_newstate origlua_newstate;
extern fnlua_close origlua_close;
extern fnlua_newthread origlua_newthread;
extern fnlua_atpanic origlua_atpanic;
extern fnlua_gettop origlua_gettop;
extern fnlua_settop origlua_settop;
extern fnlua_pushvalue origlua_pushvalue;
extern fnlua_remove origlua_remove;
extern fnlua_insert origlua_insert;
extern fnlua_replace origlua_replace;
extern fnlua_copy origlua_copy;
extern fnlua_checkstack origlua_checkstack;
extern fnlua_xmove origlua_xmove;
extern fnlua_isnumber origlua_isnumber;
extern fnlua_isstring origlua_isstring;
extern fnlua_iscfunction origlua_iscfunction;
extern fnlua_isuserdata origlua_isuserdata;
extern fnlua_type origlua_type;
extern fnlua_typename origlua_typename;
extern fnlua_tonumberx origlua_tonumberx;
extern fnlua_tointegerx origlua_tointegerx;
extern fnlua_toboolean origlua_toboolean;
extern fnlua_tolstring origlua_tolstring;
extern fnlua_rawlen origlua_rawlen;
extern fnlua_tocfunction origlua_tocfunction;
extern fnlua_touserdata origlua_touserdata;
extern fnlua_tothread origlua_tothread;
extern fnlua_topointer origlua_topointer;
extern fnlua_arith origlua_arith;
extern fnlua_rawequal origlua_rawequal;
extern fnlua_compare origlua_compare;
extern fnlua_pushnil origlua_pushnil;
extern fnlua_pushnumber origlua_pushnumber;
extern fnlua_pushinteger origlua_pushinteger;
extern fnlua_pushlstring origlua_pushlstring;
extern fnlua_pushstring origlua_pushstring;
extern fnlua_pushvfstring origlua_pushvfstring;
extern fnlua_pushfstring origlua_pushfstring;
extern fnlua_pushcclosure origlua_pushcclosure;
extern fnlua_pushboolean origlua_pushboolean;
extern fnlua_pushlightuserdata origlua_pushlightuserdata;
extern fnlua_pushthread origlua_pushthread;
extern fnlua_gettable origlua_gettable;
extern fnlua_getfield origlua_getfield;
extern fnlua_rawget origlua_rawget;
extern fnlua_rawgeti origlua_rawgeti;
extern fnlua_rawgetp origlua_rawgetp;
extern fnlua_createtable origlua_createtable;
extern fnlua_newuserdata origlua_newuserdata;
extern fnlua_getmetatable origlua_getmetatable;
extern fnlua_settable origlua_settable;
extern fnlua_setfield origlua_setfield;
extern fnlua_rawset origlua_rawset;
extern fnlua_rawseti origlua_rawseti;
extern fnlua_setmetatable origlua_setmetatable;
extern fnlua_call origlua_call;
extern fnlua_pcall origlua_pcall;
extern fnlua_gc origlua_gc;
extern fnlua_next origlua_next;
extern fnlua_concat origlua_concat;
extern fnlua_len origlua_len;


extern fnluaL_buffinit origluaL_buffinit;
extern fnluaL_optnumber origluaL_optnumber;
extern fnlua_lessthan origlua_lessthan;
extern fnlua_equal origlua_equal;
extern fnlua_error origlua_error;
extern fnlua_getfenv origlua_getfenv;
extern fnlua_cpcall origlua_cpcall;
extern fnluaL_optlstring origluaL_optlstring;
extern fnlua_getupvalue origlua_getupvalue;
extern fnluaL_checkudata origluaL_checkudata;
extern fnluaL_ref origluaL_ref;
extern fnlua_gethook origlua_gethook;
extern fnluaL_checktype origluaL_checktype;
extern fnlua_status origlua_status;
extern fnluaL_unref origluaL_unref;
extern fnlua_sethook origlua_sethook;
extern fnlua_setupvalue origlua_setupvalue;
extern fnluaL_where origluaL_where;
extern fnluaL_execresult origluaL_execresult;
extern fnlua_setallocf origlua_setallocf;
extern fnlua_gethookmask origlua_gethookmask;
extern fnlua_getstack origlua_getstack;
extern fnlua_resume_real origlua_resume_real;
extern fnluaL_checknumber origluaL_checknumber;
extern fnluaL_traceback origluaL_traceback;
extern fnluaL_gsub origluaL_gsub;
extern fnluaL_typerror origluaL_typerror;
extern fnlua_yield origlua_yield;
extern fnlua_upvalueid origlua_upvalueid;
extern fnluaL_optinteger origluaL_optinteger;
extern fnluaJIT_setmode origluaJIT_setmode;
extern fnluaL_checkinteger origluaL_checkinteger;
extern fnluaL_checkstack origluaL_checkstack;
extern fnlua_setlocal origlua_setlocal;
extern fnluaL_checklstring origluaL_checklstring;
extern fnlua_dump origlua_dump;
extern fnlua_upvaluejoin origlua_upvaluejoin;
extern fnluaL_checkany origluaL_checkany;
extern fnluaL_findtable origluaL_findtable;
extern fnlua_gethookcount origlua_gethookcount;
extern fnluaL_callmeta origluaL_callmeta;
extern fnluaL_fileresult origluaL_fileresult;
extern fnluaL_getmetafield origluaL_getmetafield;
extern fnluaL_checkoption origluaL_checkoption;
extern fnluaL_newmetatable_type origluaL_newmetatable_type;
extern fnluaL_error origluaL_error;
extern fnlua_setfenv origlua_setfenv;
extern fnluaL_register origluaL_register;
extern fnluaL_argerror origluaL_argerror;
extern fnlua_getinfo origlua_getinfo;
extern fnlua_getallocf origlua_getallocf;

extern void lua_hookshit();

static void *l_alloc(void *ud, void *ptr, size_t osize,
	size_t nsize) {
	(void)ud;  (void)osize;  /* not used */
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else
		return realloc(ptr, nsize);
}





//garry only stuff

class CLuaError;
class CLuaObject;
class Color;
class CLuaInterface;

class CLuaGameCallback
{
public:
	virtual CLuaObject* CreateLuaObject(void) = 0;
	virtual void DestroyLuaObject(CLuaObject *) = 0;
	virtual void ErrorPrint(char  const*, bool) = 0;
	virtual void Msg(char  const*, bool) = 0;
	virtual void MsgColour(char  const*, Color  const&) = 0;
	virtual void LuaError(CLuaError *) = 0;
	virtual int InterfaceCreated(CLuaInterface *) = 0;
};