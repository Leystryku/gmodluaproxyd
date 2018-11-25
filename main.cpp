#include <windows.h>


#include "include/ILuaShared.h"
#include "include/ILuaInterface.h"
#include "include/GarrysMod//Lua/Interface.h"
#include "include/GarrysMod/Lua/Types.h"
#include "include/GarrysMod/Lua/LuaObject.h"

#include <stdio.h>
#include <string>
#include <stdint.h>

#include "sdk.h"

#include <stdint.h>
#include "hde/hde32.h"
#include "lua_custom.h"
#include "lua_axilaryfuncs.h"
#include "filelogger.h"
#include "oluaobject.h"


using namespace GarrysMod::Lua;

FileLogger *filelog = 0;
CLuaInterface *cour = 0;
CLuaInterface *cli = 0;
CLuaInterface *cli_bak = 0;

bool penis = false;

typedef void* lua_State;//size of lua_State is 48  on 32 bit

void *clientdll_luaptr = 0;

lua_State* ls_client = 0;
lua_State* ls_our = 0;
lua_State *ls_client_bak = 0;


ILuaShared *luashared = 0;

int lua_hookfuncref = 0;

char* gmod_gm = 0;
bool gmod_gm_hooked = false;

char* retnaddr_concommand = 0;
char* retnaddr_luainit = 0;

char username[0xFF];

int lua_refs[10];

void createDirectoryRecursively(const std::string &directory) {
	static const std::string separators("\\/");

	// If the specified directory name doesn't exist, do our thing
	DWORD fileAttributes = ::GetFileAttributes(directory.c_str());
	if (fileAttributes == INVALID_FILE_ATTRIBUTES) {

		// Recursively do it all again for the parent directory, if any
		std::size_t slashIndex = directory.find_last_of(separators);
		if (slashIndex != std::string::npos) {
			createDirectoryRecursively(directory.substr(0, slashIndex));
		}

		// Create the last directory on the path (the recursive calls will have taken
		// care of the parent directories by now)
		BOOL result = ::CreateDirectory(directory.c_str(), nullptr);
		if (result == FALSE) {
			throw std::runtime_error("Could not create directory");
		}

	}
	else { // Specified directory name already exists as a file or directory

		bool isDirectoryOrJunction =
			((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
			((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

		if (!isDirectoryOrJunction) {
			throw std::runtime_error(
				"Could not create directory because a file with the same name exists"
			);
		}

	}
}


void recursivecreatedir(const char*dir)
{
	createDirectoryRecursively(std::string(dir));

}

void filelogger_recursivecreatedir(const char *dir)
{
	recursivecreatedir(dir);
}

intptr_t *LeyFindPattern(void* addr, size_t len, const char* mask)
{

	char*patchedmask = new char[strlen(mask)];//the patched mask WILL always be smaller than the mask inputted, because the inputted one isn't expressed as raw data but rather in ASCII
	uint32_t patchedmask_findbytes = 0;

	char lasthexchar = 0;

	for (size_t i = 0; i < strlen(mask); i++)
	{

		char curchar = toupper(mask[i]);

		if (curchar == '?')//add nulls
		{
			patchedmask[patchedmask_findbytes] = 0;
			patchedmask_findbytes += 1;
			continue;
		}

		if (curchar == ' ')//ignore spaces
		{
			continue;
		}

		if (!isalnum(curchar) || isalpha(curchar) && !(curchar > 64 && 71 > curchar) || isdigit(curchar) && !(curchar > 48 && 58 > curchar)) // not alpha numeric || not 'A' up to 'F'
		{
			printf("ER: NVH [%c __ %x]!\n", curchar, curchar);
			delete patchedmask;
			return 0;
		}

		if (!lasthexchar)
		{
			lasthexchar = curchar;
			continue;
		}

		char byte[3] = { 0 };
		byte[0] = lasthexchar;
		byte[1] = curchar;

		char databyte = strtoul(byte, 0, 16);

		patchedmask[patchedmask_findbytes] = databyte;
		patchedmask_findbytes += 1;

		lasthexchar = 0;

	}

	for (uint32_t i = 0; i < len; i++)
	{
		char *curptr = (char*)addr + i; // char* because when deref'd it's char and we only want to compare 1 byte at a time

		bool found = true;


		for (uint32_t sigptr = 0; sigptr < patchedmask_findbytes; sigptr++)
		{
			if (patchedmask[sigptr] == 0)
				continue;

			if (patchedmask[sigptr] != *(curptr + sigptr))
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			//delete patchedmask;
			return (intptr_t*)curptr;
		}
	}

	//delete patchedmask;
	return 0;
}



void *FindString(char *p, const char *string, uint32_t len)
{

	for (uint32_t i = 0; i < len; i++)
	{
		if (!(p + i))
			continue;


		if (!memcmp((char*)(p + i), string, strlen(string) + 1))
		{
			return (void*)(p + i);
		}

	}




	return 0;
}

void *FindAddrUse(char *p, void*addr, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
	{
		//if (p[i] == '\x68' || p[i] == '\xB8')
		{
			if (memcmp(&p[i + 1], &addr, 4))
				continue;


			return (char*)(p + i);

		}

		/*
		if (p[i] == '\xC7')
		{
			if (memcmp(&p[i + 2], &addr, 4))
				continue;


			return (char*)(p + i);

		}*/

	}

	return 0;
}


void *FindSubStart(void *address, uint32_t maxsearch)
{
	for (uint32_t i = 0; maxsearch>i; i++)
	{

		char*p = (char*)address - i;

		if (*p == 0x55)
		{
			return p;
			break;
		}
	}

	return 0;
}

void *FindSubNext(void *address, uint32_t maxsearch)
{
	for (uint32_t i = 0; maxsearch>i; i++)
	{

		char*p = (char*)address + i;

		if (*p == 0x55)
		{
			return p;
			break;
		}
	}

	return 0;
}

bool coolbridge_toggle = false;


int32_t coolbridge_enabled(void *state)
{

	if (!cli || !ls_our)
	{
		return 0;
	}

	if (!state)
		return coolbridge_toggle;


	if (state == ls_client_bak || state == ls_client || state == ls_our)
	{
		return coolbridge_toggle;
	}

	return coolbridge_toggle;
}

bool cancerfags = false;

int32_t count = 0;
int32_t coolbridge_top = 0;

int32_t coolbridgeon(void *L)
{
	if (penis)
		return 0;

	count++;

	//printf("|: %d\n", count);

	if (!coolbridge_toggle)
	{
		cancerfags = true;
		coolbridge_toggle = true;
	
		if (!ls_client_bak)
			ls_client_bak = ls_client;

		coolbridge_top = origlua_gettop(ls_client_bak);

		*((int32_t*)cli + 0x4) = (int32_t)ls_our;
		**(void***)clientdll_luaptr = (void*)cour;


	
	}

	return 0;
};


int32_t coolbridgeoff(void *L)
{
	if (penis)
		return 0;

	count--;

	//printf("|= %d\n", count);

	
	if (coolbridge_toggle&&!count)
	{
		cancerfags = false;
		coolbridge_toggle = false;


		*((int32_t*)cli + 0x4) = (int32_t)ls_client_bak;
		**(void***)clientdll_luaptr = cli_bak;

		while (origlua_gettop(ls_client_bak) > coolbridge_top)
		{
			origlua_settop(ls_client_bak, -1 - 1);
		}
	}

	return 0;
};


inline const char *lua_tostring(lua_State *L, int32_t stack)
{
	origlua_pushvalue(L, stack);
	origlua_pushlstring(L, "tostring", 8);
	origlua_gettable(L, -10002);
	origlua_pushvalue(L, -2);
	origlua_call(L, 1, 1);
	origlua_remove(L, -2);
	return origlua_tolstring(L, -1, 0);
}



#define LUA state->luabase
#define lua_Alloc void*
#define lua_CFunction void*
#define lua_Number void*
#define lua_Unsigned void*
#define lua_Integer void*

#define size_t uint32_t

#define lua_Reader void*
#define lua_Writer void*

typedef void* (__cdecl* fnluaL_loadbufferx)(void*state, const char *buff, size_t size, const char *name, const char *mode);
typedef void* (__fastcall* fnCreateLuaInterface)(void*ecx, int32_t edx, char state, bool newstate);
typedef void* (__fastcall* fnCloseLuaInterface)(void*ecx, int32_t edx, void*stateptr);
typedef void* (__fastcall* fnGetLuaInterface)(void*ecx, int32_t edx, unsigned char cstate);
typedef void* (__fastcall* fnGamemodeCallWithArgs)(void*ecx, int32_t edx, const char*name);
typedef void* (__fastcall* fnGamemodeCallWithArgs2)(void*ecx, int32_t edx, int32_t name);
typedef void* (__fastcall* fnGamemodeCall)(void*ecx, int32_t edx, const char*name);
typedef void* (__fastcall* fnGamemodeCall2)(void*ecx, int32_t edx, int32_t name);
typedef void* (__fastcall* fnGamemodeCallReturns)(void*ecx, int32_t edx, int32_t a2, int32_t a3);
typedef void* (__fastcall* fnGamemodeCallFinish)(void*ecx, int32_t edx, int32_t someint);
typedef char *(__fastcall* fnStartLua)(void *ecx, int32_t edx);
typedef char *(__fastcall* fnEntConstruct)(void *ecx, int32_t edx);

fnluaL_loadbufferx OrigLoadBufferx;
fnCreateLuaInterface OrigCreateLuaInterface;
fnCloseLuaInterface OrigCloseLuaInterface;
fnGetLuaInterface OrigGetLuaInterface;

fnGamemodeCallWithArgs OrigGamemodeCallWithArgs;
fnGamemodeCallWithArgs2 OrigGamemodeCallWithArgs2;
fnGamemodeCall OrigGamemodeCall;
fnGamemodeCall2 OrigGamemodeCall2;
fnGamemodeCallReturns OrigGamemodeCallReturns;
fnGamemodeCallFinish OrigGamemodeCallFinish;
fnStartLua OrigStartLua;
fnEntConstruct OrigEntConstruct;

void* panicfunc()
{
	if (ls_our)
	{

		lua_Debug entry;
		int32_t depth = 0;

		static int32_t(*lua_getstack)(void *L, int32_t depth, void*shit) = (int32_t(*)(void *L, int32_t depth, void* entry))GetProcAddress(GetModuleHandleA("lua_shared.dll"), "lua_getstack");
		static int32_t(*lua_getinfo)(void *L, const char*what, lua_Debug*ar) = (int32_t(*)(void *L, const char*what, lua_Debug*ar))GetProcAddress(GetModuleHandleA("lua_shared.dll"), "lua_getinfo");

		for (int32_t i = 0; i<100; i++)
		{
			if (!lua_getstack(ls_our, depth, &entry))
				continue;


			int32_t status = lua_getinfo(ls_our, "Sln", &entry);

			printf("%s(%d): %s\n", entry.short_src, entry.currentline, entry.name ? entry.name : "?");
			depth++;
		}

		for (int32_t i = -100; i < 5; i++)
		{
			printf("%d:::%s\n", i, origlua_tolstring(ls_our, i, 0));
		}



		const char *reason = origlua_tolstring(ls_our, -1, 0);

		MessageBoxA(NULL, reason, "OH SHIT", MB_OK);

		/*
		origlua_settop(ls_our, 0);

		origlua_getfield(ls_our, -10002, "debug");
		origlua_getfield(ls_our, -1, "traceback");
		//origlua_pushvalue(ls_our, 1);  //pass error message
		//origlua_pushinteger(ls_our, (void*)2); //skip this function and traceback
		origlua_call(ls_our, 0, 1);  // call debug.traceback

		MessageBoxA(NULL, origlua_tolstring(ls_our, -1, 0), "STACK", MB_OK);
		*/


		static int32_t(*luaL_traceback)(void *L, void* shit, int32_t shit2, int32_t shit3) = (int32_t(*)(void *L, void* shit, int32_t shit2, int32_t shit3))GetProcAddress(GetModuleHandleA("lua_shared.dll"), "luaL_traceback");
		luaL_traceback(ls_our, ls_our, 0, 1);

		reason = origlua_tolstring(ls_our, -1, 0);


		MessageBoxA(NULL, reason, "OH SHIT2", MB_OK);


	}
	else {
		MessageBoxA(NULL, "OH SHIT WADUP", "OH SHIT", MB_OK);
	}
	return 0;
}



void* LeyDetour(void* tohook, void*hookfunc, uint32_t safetycopy, bool pushret)
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


	uint32_t allocsize = 20 + safetycopy + 20;

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
	*(uint32_t*)(backup_jmpback + 1) = (uint32_t)tohook + safetycopy; // the addr after our overwrite


																	  //actual hookenz
	VirtualProtect(_tohook, 0x400, PAGE_EXECUTE_READWRITE, &oldprot);

	memset(tohook, 0x90, safetycopy);
	_tohook[0] = '\x68';//push
	*(uint32_t*)(_tohook + 1) = (uint32_t)hookfunc; // the addr after our overwrite
	_tohook[5] = '\xC3'; //ret


	VirtualProtect(_tohook, 0x400, oldprot, &trash);

	return backup;
}

int32_t coolprint(lua_State *L)
{

	printf(lua_tostring(ls_our, 1));
	origlua_settop(ls_our, -(1) - 1);

	return 0;
};

char*thebuf = 0;
int32_t thelen = 0;

int32_t newclientlua = 1;

typedef void*(__cdecl* clstatecallback_createluaobjtd)();
typedef void*(__stdcall* clstatecallback_destroyluaobjtd)(void*obj);

clstatecallback_createluaobjtd clstatecallback_createluaobj;
clstatecallback_destroyluaobjtd clstatecallback_destroyluaobj;


void* oluaobject_funcs[1024] = { 0 };
void* ourluaobject_funcs[1024] = { 0 };
int oluaobject_funcaddr = 0;
int ourluaobject_funcaddr = 0;

void**cluaobjectvmt = 0;
void**cluaobjectourvmt = 0;
int iGETEIP = 0;

class memestatecallbackclass : CLuaGameCallback
{
public:
	virtual CLuaObject* CreateLuaObject(void)
	{
		if (!oluaobject_funcs[0])
		{
			void*origobj = clstatecallback_createluaobj();
			cluaobjectvmt = *((void***)origobj);

			for (int i = 0; i < 1024; i++)
			{
				oluaobject_funcs[i] = cluaobjectvmt[i];

				if (!cluaobjectvmt[i]) break;
			}
			
			clstatecallback_destroyluaobj(origobj);

		}
		if (penis)
		{
			return (CLuaObject*)clstatecallback_createluaobj();
		}
		printf("NEW LUA OBJ\n");
		OLuaObject *luaobj = new OLuaObject((ILuaBase*)cour);




		if (!ourluaobject_funcs[0])
		{
			cluaobjectourvmt = *(void***)luaobj;
			void **newvmt = new void*[200];

			int beginning = 0;
			for (int i = 1; i < 0x100; i++)
			{
				if (*((char*)cluaobjectourvmt[0] + i) != '\x90' || *((char*)cluaobjectourvmt[0] + i + 1) != '\xCC')
					continue;

				beginning = i + 2; //strip the fucking prolog
				break;

			}
			printf("BEG: %d\n", beginning);

			for (int i = 0; i < 85; i++)
			{
				newvmt[i] = VirtualAlloc(0, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				memset(newvmt[i], 0x90, 1000);
				memcpy(newvmt[i], (char*)cluaobjectourvmt[0] + beginning, 1000);//fuck your size

			}

			*(void***)luaobj = newvmt;
			cluaobjectourvmt = newvmt;

			oluaobject_funcaddr = (int)oluaobject_funcs;
			ourluaobject_funcaddr = (int)ourluaobject_funcs;

			for (int i = 0; i < 85; i++)
			{
				ourluaobject_funcs[i] = cluaobjectourvmt[i];

				if (!cluaobjectourvmt[i]) break;
			}


			intptr_t replacevmts[30] = { 0 };
			//size_t replacevmts_l = 1;
			//replacevmts[0] = (intptr_t)cluaobjectvmt;


			size_t replacevmts_l = 0; // it'll find the first one by itself either way

			for (intptr_t i = (intptr_t)GetModuleHandleA("client.dll"); i < (intptr_t)GetModuleHandleA("client.dll") + (8388608-20); i++)
			{
				if (*(intptr_t*)i == (intptr_t)cluaobjectvmt[0] && *(intptr_t*)(i + 4) == (intptr_t)cluaobjectvmt[1]) 
				{

					if (i == (intptr_t)cluaobjectvmt)
					{
						printf("LOL: %d\n", replacevmts_l);
					}

					if (replacevmts_l ==1)
					{
						for (int a = 0; a < 85; a++)
						{

							intptr_t fn = *(intptr_t*)(i + (a*4));

							oluaobject_funcs[a] = (void*)fn;

							if (!oluaobject_funcs[a])
							{
								printf("END: %d\n", a);
								break;
							}

						}

						printf("peropero\n");
					}

					//printf("1FOUND VMT OUTSIDE .TEXT %d\n", replacevmts_l);

					replacevmts[replacevmts_l] = i; 
					replacevmts_l += 1;

				}
			}


			unsigned long shit = 0;
			unsigned long shit2 = 0;

			for (intptr_t i = (intptr_t)GetModuleHandleA("client.dll"); i < (intptr_t)GetModuleHandleA("client.dll") + 8388608; i++)
			{
				for (int j = 0; j < replacevmts_l; j++)
				{
					if (replacevmts[j]  && *(intptr_t*)i == (intptr_t)replacevmts[j])
					{

						//printf("2FOUND VMT\n");
						VirtualProtect((void*)(intptr_t*)i, 0x100, PAGE_EXECUTE_READWRITE, &shit);

						*(intptr_t*)i = (intptr_t)ourluaobject_funcs;
						VirtualProtect((void*)(intptr_t*)i, 0x100, shit, &shit2);

					}
				}

			}


			printf("done scanning\n");


		}
		printf("NEW LUA OBJ_2\n");

	//	coolbridgeoff(0);


		return (CLuaObject*)luaobj;

	};
	virtual void DestroyLuaObject(CLuaObject *obj)
	{
		if (penis)
		{
			clstatecallback_destroyluaobj(obj);
			return;
		}
		return;

		if (obj)
		{
			OLuaObject *robj = (OLuaObject*)obj;

			ILuaBase**m_pLua = (ILuaBase**)((char*)robj + 16);

			if(m_pLua)
				printf("END LUA OBJ: %x\n", *m_pLua);
			else
				printf("END LUA OBJ: %x\n", 1337);

			if (cancerfags||(CLuaInterface*)*m_pLua == cour)
			{
				printf("1\n");

				robj->~OLuaObject();

			//	delete robj;
			}
			else {
				printf("2 \n");
				clstatecallback_destroyluaobj(obj);
			}



			printf("KEND LUA OBJ\n");

		}


		//robj->UnReference();

		//robj->UnReference();
		//delete robj;

		//coolbridgeon(0);
		//void* ret = clstatecallback_destroyluaobj(obj);
		//coolbridgeoff(0);

		//coolbridgeoff(0);
		
		return ;

	};
	virtual void ErrorPrint(const char *err, bool)
	{
		printf("[(ERRORPRINT)] %s\n", err);
	};
	virtual void Msg(const char *msg, bool)
	{
		printf("[(Msg)] %s\n", msg);

	};
	virtual void MsgColour(const char *msg, Color  const&)
	{
		printf("[(MsgC)] %s\n", msg);
	};

	virtual void LuaError(CLuaError *err)
	{
		std::string* balls = (std::string*)err;

		printf("[(LUAERROR)] %s\n", balls->c_str());

	};

	virtual int32_t InterfaceCreated(CLuaInterface *face)
	{
		printf("[(INTERFACECREATED)]\n");
		return 1337;
	};
};

bool clua_grabstate = false;

void*memestatecallback = 0;
int iprintf = 0;
int iJMPBACK = 0;
/*
extern int localplayerref;
extern int entindexes[0xFFFF];
extern int entindexes_r[0xFFFF] ;
*/
void* __fastcall hooked_createluainterface(void* ecx, int32_t edx, char state, bool createnew)
{
	if (state == 0)
	{
		/*
		memset(entindexes, 0, sizeof(entindexes));
		memset(entindexes_r, 0, sizeof(entindexes_r));
		localplayerref = 0;
		*/

		ls_client = 0;
		ls_client_bak = 0;

		//penis = true;
		cancerfags = true;
		iprintf = (int)printf;
		iJMPBACK = (int)JMPBACK;
		iGETEIP = (int)GETEIP;

		cour = (CLuaInterface*)OrigCreateLuaInterface(ecx, edx, 4, createnew);
		void*old = **(CLuaInterface***)clientdll_luaptr;

		**(CLuaInterface***)clientdll_luaptr = cour;

		if (!memestatecallback)
		{
			memestatecallback = new memestatecallbackclass;
		}

		cour->Init((ILuaCallback*)memestatecallback, true);


		penis = false;

		ls_our = (lua_State*)origlua_newstate(l_alloc, 0);
		origlua_atpanic((lua_State*)ls_our, panicfunc);
		*(void**)((char*)ls_our + 0x48) = cour;
		origlua_settop(ls_our, 0);


		*((intptr_t*)(((intptr_t)cour + 0x4))) = (intptr_t)ls_our;

		//(*(void(__cdecl **)(CLuaInterface*, lua_State*))(*(char *)cour + 452))(cour, ls_our);
		//(*(void(__cdecl **)(CLuaInterface*))(*(char *)cour + 488))(cour);

		//
		//origluaL_openlibs(ls_our);


		cour->SetIsServer(false);
		cour->SetState((lua_fState*)ls_our);
		cour->SetPathID("LuaMenu");


		cancerfags = false;
		**(void***)clientdll_luaptr = old;

		clua_grabstate = true;
		void* ret = OrigCreateLuaInterface(ecx, edx, state, createnew);
		cli = (CLuaInterface*)ret;
		cli_bak = (CLuaInterface*)ret;
		//ls_client = *(lua_State**)((char*)cli + 0x4);


		newclientlua = 1;

		printf("Our State Addr: %x [%x]\n", (uint32_t)cour, (uint32_t)ls_our);

		return ret;
	}


	return OrigCreateLuaInterface(ecx, edx, state, createnew);
}

void* __fastcall hooked_closeluainterface(void* ecx, int32_t edx, void*state)
{
	__asm pushad
	__asm pushad
	//__asm pushad
	if (state == cli||state == cli_bak)
	{
		while (coolbridge_enabled(0))
			coolbridgeoff(0);

		memset(lua_refs, 0, sizeof(lua_refs));

		cli_bak = 0;
		cli = 0;
		cour = 0;
		ls_our = 0;
		ls_client = 0;
		ls_client_bak = 0;
		gmod_gm_hooked = false;
		gmod_gm = 0;

		//__asm popad
		//OrigCloseLuaInterface(ecx, edx, cour);

		__asm popad
		void* ret = OrigCloseLuaInterface(ecx, edx, state);

		__asm popad
		return ret;
	}

	__asm popad
	__asm popad
	//__asm popad
	return OrigCloseLuaInterface(ecx, edx, state);
}

void* __fastcall hooked_getluainterface(void*ecx, int32_t edx, unsigned char cstate)
{
	if (ls_our && cstate == 0 && coolbridge_enabled(0))
	{
		return cour;
	}

	return OrigGetLuaInterface(ecx, edx, cstate);
}

void CLuaObject_Hook(void*luaobj, void*luaptr)
{
	void *vmt = *(void***)luaobj;

}

void CLuaObject_Unhook(void*luaobj, void*luaptr)
{
	void *vmt = *(void***)luaobj;

}

typedef void*(__stdcall* fnCLuaObjectConstructor)();
typedef void*(__stdcall* fnCLuaObjectDeconstructor)();

fnCLuaObjectConstructor origCLuaObjectConstructor;
fnCLuaObjectDeconstructor origCLuaObjectDeconstructor;

const char *addrconcommandrun = 0;
int32_t should_disable = 0;


int vaginas = 0;

void* __stdcall hooked_CLuaObjectConstructor()
{

	__asm pushad;
	void*luaobj = 0;
	__asm mov luaobj, ecx;

	if (!luaobj)
	{
		printf("ffs\n");
		__asm popad;
		return origCLuaObjectConstructor();
	}

	if (penis)
	{
		__asm popad;
		return origCLuaObjectConstructor();
	}


	if (!cli || !cour || !ls_our)
	{
		__asm popad;
		return origCLuaObjectConstructor();
	}

	if (coolbridge_enabled(0))
	{
		//printf("2NEW LUA OBJ2\n");

		__asm popad;
		void* ret = origCLuaObjectConstructor();
		*(void**)luaobj = ourluaobject_funcs;

		return ret; 
	}


	char *vebp = 0; // current ebp
	__asm mov vebp, ebp;


	char *retnaddr = *(char**)(vebp + 0x4);


	if (!retnaddr_concommand)
	{
		if (retnaddr >= addrconcommandrun)
		{
			if (retnaddr < addrconcommandrun + 0x1F0)
			{
				retnaddr_concommand = retnaddr;
				printf("FOUND CONCMDRUN\n");
			}
		}
	}


	if (retnaddr == retnaddr_concommand)
	{

		char *debp = *(char**)vebp; // caller ebp
		char*cmd = (char*)(*(uintptr_t*)debp);

		cmd = (char*)*((uintptr_t*)cmd + 8);
		cmd = cmd + 8;

		if (1 > strlen(cmd))
		{
			__asm popad;
			return origCLuaObjectConstructor();;
		}

		size_t maxed = strlen(cmd);

		if (maxed > 1024)
			maxed = 1024;

		char *cmd_withoutargs = new char[maxed + 1];

		if (!cmd_withoutargs)
		{
			__asm popad;
			return origCLuaObjectConstructor();;
		}

		memset(cmd_withoutargs, 0, maxed + 1);




		uint32_t spaced = 0;
		for (spaced = 0; spaced < maxed; spaced++)
		{
			if (cmd[spaced] == ' ')
				break;
		}

		strncpy(cmd_withoutargs, cmd, spaced);



		printf("SOME NIGGA TRYNA RUN SOME CONCMD %s\n", cmd);

		coolbridgeon(0);
		cour->ReferencePush(lua_refs[1]);
		cour->PushString(cmd_withoutargs);
		cour->Call(1, 1);

		bool isvalid = cour->GetBool(-1);

		cour->Pop(1); // global tbl, concommand tbl, return value
		coolbridgeoff(0);
		printf("Cmd [%d] %s\n", isvalid, cmd_withoutargs);

		delete cmd_withoutargs;

		if (!isvalid)
		{
			__asm popad;
			return origCLuaObjectConstructor();
		}


		if (!coolbridge_enabled(0))
		{

			should_disable = (int32_t)luaobj;
			coolbridgeon(0);
			//vaginas++;
		}

		//*(CLuaInterface**)(ret + 16) = cour;

		__asm popad;
		return origCLuaObjectConstructor();
	}
	__asm popad;
	return origCLuaObjectConstructor();
}

void* __stdcall hooked_CLuaObjectDeconstructor()
{
	__asm pushad;
	void *luaobj = 0;
	__asm mov luaobj, ecx;

	if (!luaobj)
	{
		__asm popad;
		return origCLuaObjectDeconstructor();
	}

	if (should_disable && should_disable == (int32_t)luaobj)
	{
		coolbridgeoff(0);
		should_disable = 0;
		__asm popad;
		return origCLuaObjectDeconstructor();
	}

	if (coolbridge_enabled(0))
	{
		//printf("2NEW LUA OBJ2 KILL\n");

		OLuaObject *sret = (OLuaObject*)luaobj;
		sret->~OLuaObject();


		__asm popad;
		
		return 0;
		//return sret;
		return origCLuaObjectDeconstructor();
	}

	if (!should_disable)
	{
		__asm popad;
		return origCLuaObjectDeconstructor();
	}

	void *ret = origCLuaObjectDeconstructor();


	/*
	if (vaginas)
	{
		vaginas--;
		if (!vaginas)
		{
			coolbridgeoff(0);
		}
	}*/

	__asm popad;
	return ret;
}


struct gms // shity struct from UD
{
	unsigned char        obj1[0x14];    //0014 // gm
	unsigned char        obj2[0x14];                //0028
	unsigned char        obj3[0x14];    //003C // hook
	char                name[0x10];                //0050
	int32_t                    unk1;                    //0060
	int32_t                    unk2;                    //0064
};

void* __fastcall hooked_GamemodeCall(gms*ecx, int32_t edx, const char*name)
{

	cour->ReferencePush(lua_refs[0]);
	cour->PushString(name);
	cour->PushNil(); // gm


	coolbridgeon(0);
	int err = (int)cour->PCall(2, -1, 0);
	coolbridgeoff(0);

	if (err)
	{
		const char*error = origlua_tolstring(ls_our, -1, 0);

		if (error)
		{
			printf("ERROR: %s\n", error);
		}

		origlua_settop(ls_our, -(1) - 1);
	}


	void*ret = 0;

	if (!cour->IsType(-1, GarrysMod::Lua::Type::NIL))
	{
		ret = cour->GetType(-1);

		OrigGamemodeCall(ecx, edx, name);
	}
	else {
		ret = OrigGamemodeCall(ecx, edx, name);
	}
	return ret;

}

int cachedstrs[1000] = { 0 };

void* __fastcall hooked_GamemodeCall2(gms*ecx, int32_t edx, int32_t hookid)
{

	
	char *asstring = (*(char*(__stdcall **)(int32_t))(*(int32_t *)cli + 500))((int32_t)hookid);

	int rets = 0;
	cour->ReferencePush(lua_refs[0]);
	cour->PushString(asstring);
	cour->PushNil(); // gm

	coolbridgeon(0);

	int err = (int)cour->PCall(2, -1, 0);
	coolbridgeoff(0);

	if (err)
	{
		const char*error = origlua_tolstring(ls_our, -1, 0);

		if (error)
		{
			printf("ERROR: %s\n", error);
		}

		origlua_settop(ls_our, -(1) - 1);
	}


	void*ret = 0;

	if (!cour->IsType(-1, GarrysMod::Lua::Type::NIL))
	{
		ret = cour->GetType(-1);

		OrigGamemodeCall2(ecx, edx, hookid);
	}
	else {

		ret = OrigGamemodeCall2(ecx, edx, hookid);
	}
	return ret;

}


void* __fastcall hooked_GamemodeCallWithArgs(gms*ecx, int32_t edx, const char*name)
{
	//printf("C\n");
	GarrysMod::Lua::ILuaObject *gm1 = (GarrysMod::Lua::ILuaObject*)ecx->obj1;
	GarrysMod::Lua::ILuaObject *gm2 = (GarrysMod::Lua::ILuaObject*)ecx->obj2;
	GarrysMod::Lua::ILuaObject *gm3 = (GarrysMod::Lua::ILuaObject*)ecx->obj3;

	if (!ecx->obj1 || !ecx->obj3 || !ecx->obj2)
	{
		return OrigGamemodeCallWithArgs(ecx, edx, name);
	}


	/*
	coolbridgeon(0);

	void*ret = OrigGamemodeCallWithArgs(ecx, edx, name);

	coolbridgeoff(0);*/

	void*ret2 = OrigGamemodeCallWithArgs(ecx, edx, name);

	return ret2;
}

void* __fastcall hooked_GamemodeCallWithArgs2(gms*ecx, int32_t edx, int32_t name)
{
	//printf("D\n");
	GarrysMod::Lua::ILuaObject *gm1 = (GarrysMod::Lua::ILuaObject*)ecx->obj1;
	GarrysMod::Lua::ILuaObject *gm2 = (GarrysMod::Lua::ILuaObject*)ecx->obj2;
	GarrysMod::Lua::ILuaObject *gm3 = (GarrysMod::Lua::ILuaObject*)ecx->obj3;

	if (!ecx->obj1 || !ecx->obj3 || !ecx->obj2)
	{
		return OrigGamemodeCallWithArgs2(ecx, edx, name);
	}


	/*
	coolbridgeon(0);


	void*ret = OrigGamemodeCallWithArgs2(ecx, edx, name);

	coolbridgeoff(0);*/

	void*ret2 = OrigGamemodeCallWithArgs2(ecx, edx, name);

	return ret2;
}

void* __fastcall hooked_GamemodeCallFinish(gms*ecx, int32_t edx, int32_t someint)
{
	//printf("E\n");
	GarrysMod::Lua::ILuaObject *gm1 = (GarrysMod::Lua::ILuaObject*)ecx->obj1;
	GarrysMod::Lua::ILuaObject *gm2 = (GarrysMod::Lua::ILuaObject*)ecx->obj2;
	GarrysMod::Lua::ILuaObject *gm3 = (GarrysMod::Lua::ILuaObject*)ecx->obj3;

	if (!ecx->obj1 || !ecx->obj3 || !ecx->obj2)
	{
		return OrigGamemodeCallFinish(ecx, edx, someint);
	}


	CLuaInterface*oldluaptr1 = *(CLuaInterface**)(gm1 + 16);
	CLuaInterface*oldluaptr2 = *(CLuaInterface**)(gm2 + 16);
	CLuaInterface*oldluaptr3 = *(CLuaInterface**)(gm3 + 16);


	/*
	coolbridgeon(0);


	void*ret = OrigGamemodeCallFinish(ecx, edx, someint);

	coolbridgeoff(0);*/


	void*ret2 = OrigGamemodeCallFinish(ecx, edx, someint);

	return ret2;
}

void* __fastcall hooked_StartLua(void* ecx, int32_t edx)
{

	coolbridgeon(0);
	OrigStartLua(ecx, edx);
	coolbridgeoff(0);
	*(char *)((char*)ecx + 4) = 0;

	return 0;// OrigStartLua(ecx, edx);

}

void* __fastcall  hooked_EntConstruct(void *ecx, int32_t edx)
{
	coolbridgeon(0);
	void*ret = OrigEntConstruct(ecx, edx);
	coolbridgeoff(0);

	return ret;
}
void* __fastcall hooked_GamemodeCallReturns(gms*ecx, int32_t edx, int32_t a2, int32_t a3)
{
	printf("F\n");
	GarrysMod::Lua::ILuaObject *gm1 = (GarrysMod::Lua::ILuaObject*)ecx->obj1;
	GarrysMod::Lua::ILuaObject *gm2 = (GarrysMod::Lua::ILuaObject*)ecx->obj2;
	GarrysMod::Lua::ILuaObject *gm3 = (GarrysMod::Lua::ILuaObject*)ecx->obj3;

	if (!ecx->obj1 || !ecx->obj3 || !ecx->obj2)
	{
		return OrigGamemodeCallReturns(ecx, edx, a2, a3);
	}


	/*
	coolbridgeon(0);


	void*ret = OrigGamemodeCallReturns(ecx, edx, a2, a3);

	coolbridgeoff(0);*/

	void*ret2 = OrigGamemodeCallReturns(ecx, edx, a2, a3);

	return ret2;
}


bool rec = false;

void* __cdecl hooked_loadbufferx(void*state, const char *buff, size_t size, const char *name, const char *mode)
{

	if (!coolbridge_enabled(0) && ls_client_bak == state || ls_client == state)
	{
		filelog->LogFile(name, buff, size);
	}

	if (ls_our && ls_our == state || coolbridge_enabled(0))
	{
		printf("[LF][4]: %s\n", name);

		coolbridgeon(0);
		void*ret = OrigLoadBufferx(ls_our, buff, size, name, mode);
		coolbridgeoff(0);
		return ret;

	}else{
		if (!newclientlua)
		{
			if (ls_client_bak == state || ls_client == state)
			{
				//printf("[LF][0]: %s\n", name);
			}
			return OrigLoadBufferx(state, buff, size, name, mode);
		}

		if (!ls_client && !ls_client_bak || !strstr(name, "init.lua"))
		{
			return OrigLoadBufferx(state, buff, size, name, mode);
		}


		// time to become independent
		//MessageBoxA(NULL, (const char*)lua->GetPathID(), (const char*)cli->GetPathID(), MB_OK);
		//for getluastate the memestruct?


		printf("COUR: %x\n", cour);



		newclientlua = 0;

		ls_client_bak = ls_client;
		ls_client = 0;



		origlua_pushcclosure(ls_our, &coolprint, 0);
		origlua_setfield(ls_our, -10002, "cprint");

		FILE*memecode = fopen("C:/ss.lua", "rb");
		if (memecode)
		{
			memset(thebuf, 0, thelen);

			delete thebuf;
			thebuf = 0;
			thelen = 0;

			fseek(memecode, 0, SEEK_END);
			thelen = ftell(memecode);
			rewind(memecode);

			thebuf = new char[thelen + 5];

			fread(thebuf, thelen, sizeof(char), memecode);
			fclose(memecode);
			thebuf[thelen] = 0;
			thebuf[thelen + 1] = 0;
			thebuf[thelen + 2] = 0;


		}

		newclientlua = 0;
		printf("Loading...\n");

		cour->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);

		cour->PushCFunction(lua_GetTeam);
		cour->SetField(-2, "CLGetTeam");

		/*
		cour->PushCFunction(lua_Entity);
		cour->SetField(-2, "Entity");

		cour->PushCFunction(lua_LocalPlayer);
		cour->SetField(-2, "LocalPlayer");

		cour->PushCFunction(lua_HighestEntityIndex);
		cour->SetField(-2, "HighestEntityIndex");
		*/

		cour->CreateTable();


		cour->PushCFunction(lua_SetName);
		cour->SetField(-2, "SETNAME");

		cour->PushCFunction(lua_SetConVar);
		cour->SetField(-2, "SETCVAR");

		cour->PushCFunction(lua_PureECC);
		cour->SetField(-2, "PURECC");

		cour->PushCFunction(lua_LuaRun);
		cour->SetField(-2, "LUARUN");

		cour->PushCFunction(lua_SetSpeed);
		cour->SetField(-2, "SETSPEED");

		cour->PushCFunction(lua_ReadFile);
		cour->SetField(-2, "READFILE");

		cour->PushCFunction(lua_WriteFile);
		cour->SetField(-2, "WRITEFILE");

		cour->PushCFunction(lua_SettingRead);
		cour->SetField(-2, "SETTINGREAD");

		cour->PushCFunction(lua_SettingWrite);
		cour->SetField(-2, "SETTINGWRITE");

		cour->PushCFunction(lua_RunScripts);
		cour->SetField(-2, "RUNSCRIPTS");

		cour->PushCFunction(SSH_hl2_ucmd_getprediction);
		cour->SetField(-2, "hl2_ucmd_getprediction");

		cour->PushCFunction(SSH_hl2_shotmanip);
		cour->SetField(-2, "hl2_shotmanip");

		cour->PushCFunction(SSH_References);
		cour->SetField(-2, "REFERENCES");


		cour->PushLong(23);
		cour->SetField(-2, "MODVER");

		cour->SetField(-2, "SSH");


		cour->Pop(1);

		char*coolerr = 0;

		RunLua("meinkampf", thebuf, (void**)&coolerr);



	}




	return OrigLoadBufferx(state, buff, size, name, mode);


}

void CV_GlobalChange_Lua(IConVar *var, const char *oldvalue, float foldvalue)
{
	if (!var || !oldvalue || !cour || !ls_our || !lua_refs[2])
		return;

	coolbridgeon(0);
	cour->ReferencePush(lua_refs[2]);
	cour->PushString(var->GetName());
	cour->PushString(oldvalue);
	cour->PushString(g_pCVar->FindVar(var->GetName())->GetString());


	int err = (int)cour->PCall(3, -1, 0);

	if (err)
	{
		const char*error = origlua_tolstring(ls_our, -1, 0);

		if (error)
		{
			printf("ERROR: %s\n", error);
		}

		origlua_settop(ls_our, -(1) - 1);
	}


	coolbridgeoff(0);
}



int32_t HookShit()
{


	HMODULE lmod = GetModuleHandle("lua_shared.dll");
	if (lmod == NULL)
	{
		MessageBox(NULL, "Couldn't find lua_shared.dll", "Error", MB_OK);
		return 0;
	}

	void *loadbuf = GetProcAddress(lmod, "luaL_loadbufferx");

	if (!loadbuf)
	{
		MessageBox(NULL, "missing loadbuf", "k", MB_OK);
		return 0;
	}

	HMODULE clmod = GetModuleHandle("client.dll");
	if (clmod == NULL)
	{
		MessageBox(NULL, "Couldn't find client.dll", "Error", MB_OK);
		return 0;
	}

	AllocConsole();
	SetConsoleTitleA("SH");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	typedef void* (__cdecl* CreateInterfaceFn)(const char* name, int32_t* retcode);
	CreateInterfaceFn func = (CreateInterfaceFn)GetProcAddress(lmod, "CreateInterface");

	if (func == NULL)
	{
		MessageBox(NULL, "Couldn't find CreateInterface", "Error", MB_OK);
		return 0;
	}

	luashared = (ILuaShared*)func("LUASHARED003", NULL);

	if (luashared == NULL)
	{
		MessageBox(NULL, "Couldn't find ILuaShared", "Error", MB_OK);
		return 0;
	}
	
	InitializeSdk();

	lua_hookshit();
	g_pCVar->InstallGlobalChangeCallback(CV_GlobalChange_Lua);

	unsigned long gay = 255;
	GetUserNameA(username, &gay);

	char basedir[256] = { 0 };
	char settingsfolder[256] = { 0 };
	char lualogfolder[256] = { 0 };
	char serverlualogfolder[256] = { 0 };

	snprintf(basedir, 256, "C:/Users/%s/Documents/SH", username);
	snprintf(settingsfolder, 256, "%s/%s",basedir, "settings");
	snprintf(lualogfolder, 256, "%s/%s", basedir, "luadump");
	snprintf(serverlualogfolder, 256, "%s/%s", lualogfolder, "127.0.0.1");

	recursivecreatedir(settingsfolder);

	filelog = new FileLogger;
	filelog->Start(serverlualogfolder);
	
	//char*luastartupstr = (char*)FindString((char*)clmod, "clientside lua startup!\n", 7096832);
	//char*firstluastartupstr = (char*)FindAddrUse((char*)clmod, luastartupstr, 7096832);
	//	luastartmanagerptr = FindSubStart(firstluastartupstr, 7096832);

	void **vluashared = *(void***)luashared;
	void *createluainterfaceptr = vluashared[4];
	void* closeluainterfaceptr  = vluashared[5];
	void* getluainterfaceptr = vluashared[6];



	printf("1\n");

	void *ptrCLuaObjectConstructor = LeyFindPattern(clmod, 7096832, "8B C1 83 C9 FF C7");
	printf("2\n");

	addrconcommandrun = (char*)LeyFindPattern(clmod, 7096832, "55 8B EC 83 EC 3C 83 3D ? ? ? ? ?");


	void*ptrCLuaObjectDeconstructor = (char*)ptrCLuaObjectConstructor - 0x50;

	char*luastartupstr = (char*)FindString((char*)clmod, "clientside lua startup!\n", 7096832);
	char*luainitstr = (char*)FindString((char*)clmod, "New gGM when old one exists!\n", 7096832);
	char*firstgmcall = (char*)FindString((char*)clmod, "[GM:Call - !ThreadInMainThread] %s\n", 7096832);
	char*firstgmcallwithargs = (char*)FindString((char*)clmod, "[GM:CallWithArgs - !ThreadInMainThread] %s\n", 7096832);
	char*firstgmfinish = (char*)FindString((char*)clmod, "CLuaGamemode::CallFinish", 7096832);
	char*firststartlua = (char*)FindString((char*)clmod, "CScriptedEntity with invalid m_ParentEntity->GetLuaEntity()", 7096832);
	char*firstentconstruct = (char*)FindString((char*)clmod, "C_BaseEntity::m_iv_angRotation", 8388608);
	//char*firstgmreturn = (char*)FindString((char*)clmod, "CLuaGamemode::CallFinish", 7096832)

	char*firstluastartupstr = (char*)FindAddrUse((char*)clmod, luastartupstr, 7096832);
	char*firstluainitstr = (char*)FindAddrUse((char*)clmod, luainitstr, 7096832);
	char*firstgmcallstr = (char*)FindAddrUse((char*)clmod, firstgmcall, 7096832);
	char*firstgmcallwithargsstr = (char*)FindAddrUse((char*)clmod, firstgmcallwithargs, 7096832);
	char*firstgmfinishstr = (char*)FindAddrUse((char*)clmod, firstgmfinish, 7096832);
	char*firststartluastr = (char*)FindAddrUse((char*)clmod, firststartlua, 7096832);
	char*firstentconstructstr = (char*)FindAddrUse((char*)clmod, firstentconstruct, 8388608);

	//char*firstgmreturnstr = (char*)FindAddrUse((char*)clmod, firstgmreturn, 7096832);

	void*ptrgmcall = (char*)FindSubStart(firstgmcallstr, 7096832);
	void*ptrgmcall2 = (char*)FindSubNext((char*)ptrgmcall + 2, 7096832);
	void*ptrgmcallwithargs = (char*)FindSubStart(firstgmcallwithargsstr, 7096832);
	void*ptrgmcallwithargs2 = (char*)FindSubNext((char*)ptrgmcallwithargs + 2, 7096832);

	void*ptrgmfinish = (char*)FindSubStart(firstgmfinishstr, 7096832);
	void*ptrstartlua = (char*)FindSubStart(firststartluastr, 7096832);
	void*ptrentconstruct = (char*)FindSubStart(firstentconstructstr, 7096832);

	//void*ptrgmreturn = (char*)FindSubStart(firstgmreturnstr, 7096832);

	clientdll_luaptr = firstluastartupstr + 0x6C + 0x2; // it's some mov ecx, gluaptr

	printf("LUA PTR ADDR: %x\n", clientdll_luaptr);

	*(void**)&OrigLoadBufferx = LeyDetour(loadbuf, hooked_loadbufferx, 0, false);
	*(void**)&OrigCreateLuaInterface = LeyDetour(createluainterfaceptr, hooked_createluainterface, 0, false);
	*(void**)&OrigCloseLuaInterface = LeyDetour(closeluainterfaceptr, hooked_closeluainterface, 0, false);
	*(void**)&OrigGetLuaInterface = LeyDetour(getluainterfaceptr, hooked_getluainterface, 0, true);

	*(void**)&origCLuaObjectConstructor = LeyDetour(ptrCLuaObjectConstructor, hooked_CLuaObjectConstructor, 0, true);
	*(void**)&origCLuaObjectDeconstructor = LeyDetour(ptrCLuaObjectDeconstructor, hooked_CLuaObjectDeconstructor, 0, true);

	char*metastr = (char*)FindString((char*)clmod, "MetaBaseClass", 7096832);
	char*firstmetastr = (char*)FindAddrUse((char*)clmod, metastr, 7096832);
	char*secondmetastr = (char*)FindAddrUse(firstmetastr + 0x10, metastr, 7096832);

	*(void**)&clstatecallback_createluaobj = (char*)secondmetastr + 0xEC;
	*(void**)&clstatecallback_destroyluaobj = (char*)secondmetastr + 0xEC + 0xA0;

	*(void**)&OrigGamemodeCall = LeyDetour(ptrgmcall, hooked_GamemodeCall, 0, true);
	*(void**)&OrigGamemodeCall2 = LeyDetour(ptrgmcall2, hooked_GamemodeCall2, 0, true);
	
	*(void**)&OrigGamemodeCallWithArgs = LeyDetour(ptrgmcallwithargs, hooked_GamemodeCallWithArgs, 0, true);
	*(void**)&OrigGamemodeCallWithArgs2 = LeyDetour(ptrgmcallwithargs2, hooked_GamemodeCallWithArgs2, 0, true);

	*(void**)&OrigGamemodeCallFinish = LeyDetour(ptrgmfinish, hooked_GamemodeCallFinish, 0, true);
	
//	*(void**)&OrigStartLua = LeyDetour(ptrstartlua, hooked_StartLua, 0, true);
//	*(void**)&OrigEntConstruct = LeyDetour(ptrentconstruct, hooked_EntConstruct, 0, true);

	//*(void**)&OrigGamemodeCallReturns = LeyDetour(ptrgmreturn, hooked_GamemodeCallReturns, 0, true);*/ // a weirdo

	//int32_t __cdecl CLuaObject::CLuaObject(int32_t a1)
	// hook it, walk around the stack to check the caller functions argument nr.1, if it's lua_State* or ILuaInterface* then only return to original function
	// if it's not, and  return to original function


	printf("CC: %x _ GG: %x\n", addrconcommandrun, ptrCLuaObjectConstructor);
	//gmod_gm = (char*)(firstluainitstr - 0x8 + 0x2);


	return 1;

}

BOOL WINAPI DllMain(HINSTANCE dllinstance, DWORD     reason, LPVOID    reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		HookShit();
	}

	return 1;

}
