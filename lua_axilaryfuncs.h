#pragma once

#include <cstdint>

bool RunLua(const char*filename, const char*content, void** err, bool clientstate = false);

int32_t lua_GetTeam(lua_fState *state);
int32_t lua_HighestEntityIndex(lua_fState *state);
int32_t lua_LocalPlayer(lua_fState *state);
int32_t lua_Entity(lua_fState *state);
int32_t SSH_References(lua_fState *state);
int32_t SSH_hl2_shotmanip(lua_fState *state);
int32_t SSH_hl2_ucmd_getprediction(lua_fState *state);
int32_t lua_RunScripts(lua_fState *state);
int32_t lua_SettingWrite(lua_fState *state);
int32_t lua_SettingRead(lua_fState *state);
int32_t lua_WriteFile(lua_fState *state);
int32_t lua_ReadFile(lua_fState *state);
int32_t lua_SetSpeed(lua_fState *state);
int32_t lua_LuaRun(lua_fState *state);
int32_t lua_PureECC(lua_fState *state);
int32_t lua_SetConVar(lua_fState *state);
int32_t lua_SetName(lua_fState *state);