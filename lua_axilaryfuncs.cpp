#include <stdint.h>

#include "include/ILuaShared.h"
#include "include/ILuaInterface.h"
#include "include/GarrysMod//Lua/Interface.h"
#include "include/GarrysMod/Lua/Types.h"
#include "include/GarrysMod/Lua/LuaObject.h"
#include "sdk.h"
#include "lua_custom.h"

extern int32_t coolbridgeon(void*L);
extern int32_t coolbridgeoff(void*L);
extern int32_t coolbridge_enabled(void*state);

extern char username[0xFF];

int32_t lua_SetName(lua_fState *state)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::STRING))
		return 1;

	if (!g_pEngine->IsInGame())
		return 1;

	INetChannel* chan = (INetChannel*)g_pEngine->GetNetChannelInfo();

	if (!chan)
		return 1;

	char sendbuf[1024];
	bf_write pck(sendbuf, 1024);
	pck.WriteUBitLong(5, 6);
	pck.WriteByte(1);
	pck.WriteString("name");
	pck.WriteString(LUA->GetString(1));

	return chan->SendData(pck);
}

int32_t lua_SetConVar(lua_fState *state)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::STRING) || !LUA->IsType(2, GarrysMod::Lua::Type::STRING))
		return 1;

	ConVar *var = g_pCVar->FindVar(LUA->GetString(1));

	if (var)
	{
		var->SetValue(LUA->GetString(2));
		return 1;
	}

	return 1;
}

int32_t lua_PureECC(lua_fState *state)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::STRING))
		return 1;

	g_pEngine->ClientCmd_Unrestricted(LUA->GetString(1));

	return 1;
}




bool RunLua(const char*filename, const char*content, void** err, bool clientstate = false)
{


	static int32_t(*luaL_loadbufferx)(void *L, const char *buf, size_t len, const char *name, void*nig) = (int32_t(*)(void*, const char*, size_t, const char*, void*))GetProcAddress(GetModuleHandleA("lua_shared.dll"), "luaL_loadbufferx");

	int32_t error = 0;

	lua_State *state = ls_our;

	if (clientstate)
	{
		state = ls_client_bak;
	}

	if (!clientstate)
	{

		coolbridgeon(0);
	}

	if (filename)
	{
		error = luaL_loadbufferx(state, content, strlen(content), filename, 0);
	}
	else {
		error = luaL_loadbufferx(state, content, strlen(content), "Startup", 0);

	}

	if (error)
	{
		const char*error = origlua_tolstring(state, -1, 0);

		if (err)
		{
			if (error)
			{
				printf("ERROR: %s\n", error);
			}
			*err = (void*)error;
		}

		origlua_settop(state, -(1) - 1);

		if (!clientstate)
			coolbridgeoff(0);

		return 1;
	}


	if (origlua_pcall(state, 0, -1, 0))
	{
		
		const char*error = origlua_tolstring(state, -1, 0);

		if (err)
		{

			if (error)
			{
				printf("ERROR: %s\n", error);
			}
			*err = (void*)error;
		}

		origlua_settop(state, -(1) - 1);

		if(!clientstate)
			coolbridgeoff(0);

		return 0;
	}

	if(!clientstate)
		coolbridgeoff(0);


	return 0;
}

int32_t lua_LuaRun(lua_fState *state)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::STRING))
		return 1;

	const char *content = LUA->GetString(1);
	const char *path = "[C]";
	
	if (LUA->IsType(2, GarrysMod::Lua::Type::STRING))
	{
		path = LUA->GetString(2);
	}

	char*err = 0;

	RunLua(path, content, (void**)&err, false);

	if (err)
	{
		LUA->PushString(err);
	}



	return 1;
}

int32_t lua_SetSpeed(lua_fState *state)
{
	return 1;
}


int32_t lua_ReadFile(lua_fState *state)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::STRING))
		return 1;

	char safestring[0xFF];
	strcpy(safestring, LUA->GetString(1));

	for (size_t i = 0; i < strlen(safestring); i++)
	{
		if (i > 1 && safestring[i] == '.' && safestring[i - 1] == '.')
		{
			safestring[i] = '_';
		}

		if (safestring[i] == '<' || safestring[i] == '>' || safestring[i] == ':' || safestring[i] == '"' || safestring[i] == '|' || safestring[i] == '?' || safestring[i] == '*')
		{
			safestring[i] = '_';
		}
		if (32 > safestring[i])
		{
			safestring[i] = '_';
		}


		if (safestring[i] == '\\')
		{
			safestring[i] = '/';
		}

	}

	char namaewa[0xFF];

	snprintf(namaewa, 0xFF, "C:/Users/%s/Documents/SH/%s", username, safestring);

	FILE*filehandle = fopen(namaewa, "rb");
	if (filehandle)
	{
		fseek(filehandle, 0, SEEK_END);
		size_t ourlen = ftell(filehandle);
		rewind(filehandle);

		char*fbuf = new char[ourlen + 5];

		fread(fbuf, ourlen, sizeof(char), filehandle);
		fclose(filehandle);
		fbuf[ourlen] = 0;
		fbuf[ourlen + 1] = 0;
		fbuf[ourlen + 2] = 0;
		LUA->PushString(fbuf);
		free(fbuf);


	}
	else {
		LUA->PushNil();
	}

	return 1;
}
int32_t lua_WriteFile(lua_fState *state)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::STRING))
		return 1;

	if (!LUA->IsType(2, GarrysMod::Lua::Type::STRING))
		return 1;

	char safestring[0xFF];
	strcpy(safestring, LUA->GetString(1));

	for (size_t i = 0; i < strlen(safestring); i++)
	{
		if (i > 1 && safestring[i] == '.' && safestring[i - 1] == '.')
		{
			safestring[i] = '_';
		}

		if (safestring[i] == '<' || safestring[i] == '>' || safestring[i] == ':' || safestring[i] == '"' || safestring[i] == '|' || safestring[i] == '?' || safestring[i] == '*')
		{
			safestring[i] = '_';
		}
		if (32 > safestring[i])
		{
			safestring[i] = '_';
		}


		if (safestring[i] == '\\')
		{
			safestring[i] = '/';
		}

	}

	char namaewa[0xFF];

	snprintf(namaewa, 0xFF, "C:/Users/%s/Documents/SH/%s", username, safestring);

	FILE*filehandle = fopen(namaewa, "wb");
	if (filehandle)
	{
		fwrite(LUA->GetString(2), sizeof(char), strlen(LUA->GetString(2)), filehandle);
		fclose(filehandle);


	}

	return 1;
}
int32_t lua_SettingRead(lua_fState *state)
{

	if (!LUA->IsType(1, GarrysMod::Lua::Type::STRING))
		return 1;


	char safestring[0xFF];
	strcpy(safestring, LUA->GetString(1));

	for (size_t i = 0; i < strlen(safestring); i++)
	{
		if (i > 1 && safestring[i] == '.' && safestring[i - 1] == '.')
		{
			safestring[i] = '_';
		}

		if (safestring[i] == '<' || safestring[i] == '>' || safestring[i] == ':' || safestring[i] == '"' || safestring[i] == '|' || safestring[i] == '?' || safestring[i] == '*')
		{
			safestring[i] = '_';
		}
		if (32 > safestring[i])
		{
			safestring[i] = '_';
		}


		if (safestring[i] == '\\')
		{
			safestring[i] = '/';
		}

	}

	char namaewa[0xFF];

	snprintf(namaewa, 0xFF, "C:/Users/%s/Documents/SH/settings/%s", username, safestring);

	FILE*filehandle = fopen(namaewa, "rb");
	if (filehandle)
	{
		fseek(filehandle, 0, SEEK_END);
		size_t ourlen = ftell(filehandle);
		rewind(filehandle);

		char*fbuf = new char[ourlen + 5];

		fread(fbuf, ourlen, sizeof(char), filehandle);
		fclose(filehandle);
		fbuf[ourlen] = 0;
		fbuf[ourlen + 1] = 0;
		fbuf[ourlen + 2] = 0;
		LUA->PushString(fbuf);
		free(fbuf);


	}
	else {
		LUA->PushNil();
	}


	return 1;
}
int32_t lua_SettingWrite(lua_fState *state)
{

	if (!LUA->IsType(1, GarrysMod::Lua::Type::STRING))
		return 1;

	if (!LUA->IsType(2, GarrysMod::Lua::Type::STRING))
		return 1;

	char safestring[0xFF];
	strcpy(safestring, LUA->GetString(1));

	for (size_t i = 0; i < strlen(safestring); i++)
	{
		if (i > 1 && safestring[i] == '.' && safestring[i - 1] == '.')
		{
			safestring[i] = '_';
		}

		if (safestring[i] == '<' || safestring[i] == '>' || safestring[i] == ':' || safestring[i] == '"' || safestring[i] == '|' || safestring[i] == '?' || safestring[i] == '*')
		{
			safestring[i] = '_';
		}
		if (32 > safestring[i])
		{
			safestring[i] = '_';
		}


		if (safestring[i] == '\\')
		{
			safestring[i] = '/';
		}

	}

	char namaewa[0xFF];

	snprintf(namaewa, 0xFF, "C:/Users/%s/Documents/SH/settings/%s", username, safestring);

	FILE*filehandle = fopen(namaewa, "wb");
	if (filehandle)
	{
		fwrite(LUA->GetString(2), sizeof(char), strlen(LUA->GetString(2)), filehandle);
		fclose(filehandle);

	}

	return 1;
}
int32_t lua_RunScripts(lua_fState *state)
{

	return 1;
}

int32_t SSH_hl2_ucmd_getprediction(lua_fState *state)
{
	return 1;
}
int32_t SSH_hl2_shotmanip(lua_fState *state)
{
	return 1;
}

extern int lua_refs[10];

int32_t SSH_References(lua_fState *state)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::FUNCTION) || !LUA->IsType(2, GarrysMod::Lua::Type::FUNCTION) )
	{
		return 1;
	}

	printf("Installed cheeseburgers!\n");

	LUA->Push(1);
	lua_refs[0] = LUA->ReferenceCreate();
	LUA->Push(2);
	lua_refs[1] = LUA->ReferenceCreate();
	LUA->Push(3);
	lua_refs[2] = LUA->ReferenceCreate();

	return 1;
}

int32_t lua_GetTeam(lua_fState *state)
{

	return 1;
}
/*
int entindexes[0xFFFF] = { 0 };
int entindexes_r[0xFFFF] = { 0 };

int32_t lua_Entity(lua_fState *state)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::NUMBER))
	{
		return 1;
	}

	IEntity* pEnt = (IEntity*)g_pEntList->GetClientEntity(LUA->GetNumber(1));

	if (!pEnt || !pEnt->GetClientClass())
	{
		LUA->PushNil();
		return 1;
	}
	

	player_info_s plInfo;
	g_pEngine->GetPlayerInfo(pEnt->entindex(), &plInfo);

	if (!entindexes[pEnt->entindex()])
	{
		GarrysMod::Lua::UserData* ud = (GarrysMod::Lua::UserData*)LUA->NewUserdata(sizeof(GarrysMod::Lua::UserData));
		ud->type = GarrysMod::Lua::Type::ENTITY;
		entindexes[pEnt->entindex()] = (int)ud;
		entindexes_r[pEnt->entindex()] = LUA->ReferenceCreate();
	}

	auto ud = (GarrysMod::Lua::UserData*)entindexes[pEnt->entindex()];

	const CBaseHandle &ebola = pEnt->GetRefEHandle();
	ud->data = new CBaseHandle(ebola);

	LUA->ReferencePush(entindexes_r[pEnt->entindex()]);

	if (pEnt->entindex()>g_pGlobals->maxClients|| !pEnt->entindex()|| !plInfo.name)
	{
		LUA->CreateMetaTableType("Entity", GarrysMod::Lua::Type::ENTITY);
		LUA->SetMetaTable(-2);
		return 1;
	}

	LUA->CreateMetaTableType("Player", GarrysMod::Lua::Type::ENTITY);
	LUA->SetMetaTable(-2);
	return 1;
}

int localplayerref = 0;

int32_t lua_LocalPlayer(lua_fState *state)
{

	IEntity* pEnt = (IEntity*)g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!pEnt || !pEnt->GetClientClass())
	{
		localplayerref = 0;
		LUA->PushNil();
		return 1;
	}

	if (!localplayerref)
	{
		GarrysMod::Lua::UserData* ud = (GarrysMod::Lua::UserData*)LUA->NewUserdata(sizeof(GarrysMod::Lua::UserData));
		ud->type = GarrysMod::Lua::Type::ENTITY;

		localplayerref = LUA->ReferenceCreate();

		const CBaseHandle &ebola = pEnt->GetRefEHandle();
		ud->data = new CBaseHandle(ebola);
	}
	LUA->ReferencePush(localplayerref);


	LUA->CreateMetaTableType("Player", GarrysMod::Lua::Type::ENTITY);
	LUA->SetMetaTable(-2);

	return 1;
}

int32_t lua_HighestEntityIndex(lua_fState *state)
{
	LUA->PushNumber(g_pEntList->GetHighestEntityIndex());
	return 1;
}
*/