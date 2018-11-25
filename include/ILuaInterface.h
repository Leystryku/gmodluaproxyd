#ifndef LUAINTERFACE_H
#define LUAINTERFACE_H

struct lua_fState;
class Color;
class GarrysMod__Lua__IThreadedCall;
class std__string;
class Bootil__Buffer;
typedef class CLuaCallback ILuaCallback;
class ILuaObject;
typedef class CLuaInterface ILuaInterface;

typedef int(*CLuaFunction) (lua_fState*);

typedef unsigned int uint;

#define vfunc(name)		(__thiscall *name)

class CLuaInterface
{
public:
	//you will have to fix the types yourself for the returns
	virtual void *Top(void);
	virtual void *Push(int);
	virtual void *Pop(int);
	virtual void *GetTable(int);
	virtual void *GetField(int, char  const*);
	virtual void *SetField(int, char  const*);
	virtual void *CreateTable(void);
	virtual void *SetTable(int);
	virtual void *SetMetaTable(int);
	virtual void *GetMetaTable(int);
	virtual void *Call(int, int);
	virtual void *PCall(int, int, int);
	virtual void *Equal(int, int);
	virtual void *RawEqual(int, int);
	virtual void *Insert(int);
	virtual void *Remove(int);
	virtual void *Next(int);
	virtual void *NewUserdata(uint);
	virtual void *ThrowError(char  const*);
	virtual void *CheckType(int, int);
	virtual void *ArgError(int, char  const*);
	virtual void *RawGet(int);
	virtual void *RawSet(int);
	virtual char *GetString(int);
	virtual float GetNumber(int);
	virtual void *GetBool(int);
	virtual void *GetCFunction(int);
	virtual void *GetUserdata(int);
	virtual void *PushNil(void);
	virtual void *PushString(char  const*, int idk=0);
	virtual void *PushNumber(double);
	virtual void *PushBool(bool);
	virtual void *PushCFunction(int(*)(lua_fState *));
	virtual void *PushCClosure(int(*)(lua_fState *), int);
	virtual void *PushUserdata(void *);
	virtual void *ReferenceCreate(void);
	virtual void *ReferenceFree(int);
	virtual void *ReferencePush(int);
	virtual void *PushSpecial(int);
	virtual void *IsType(int, int);
	virtual void *GetType(int);
	virtual void *GetTypeName(int);
	virtual void *CreateMetaTableType(char  const*, int);
	virtual void *CheckString(int);
	virtual void *CheckNumber(int);
	virtual void *ObjLen(int);
	virtual void *AddThreadedCall(GarrysMod__Lua__IThreadedCall *);
	virtual void *Init(ILuaCallback *, bool);
	virtual void *Shutdown(void);
	virtual void *Cycle(void);
	virtual void *GetLuaState(void);
	virtual void* Global(void);
	virtual void *GetObject(int);
	virtual void *DELETE_ME_5437(int);
	virtual void *_DELETE_ME2466(int);
	virtual void *PushLuaObject(void *);
	virtual void *PushLuaFunction(int(*)(lua_fState *));
	virtual void *LuaError(char  const*, int);
	virtual void *TypeError(char  const*, int);
	virtual void *CallInternal(int, int);
	virtual void *CallInternalNoReturns(int);
	virtual void *CallInternalGetBool(int);
	virtual void *CallInternalGetString(int);
	virtual void *CallInternalGet(int, void *);
	virtual void *_DELETE_ME(void *, void *);
	virtual void *NewGlobalTable(char  const*);
	virtual void *NewTemporaryObject(void);
	virtual void *isUserData(int);
	virtual void *GetMetaTableObject(char  const*, int);
	virtual void *GetMetaTableObject(int);
	virtual void *GetReturn(int);
	virtual void *IsServer(void);
	virtual void *IsClient(void);
	virtual void *IsDedicatedServer(void);
	virtual void *DestroyObject(void *);
	virtual void *CreateObject(void);
	virtual void *SetMember(void *, void *, void *);
	virtual void *GetNewTable(void);
	virtual void *SetMember(void *, float);
	virtual void *SetMember(void *, float, void *);
	virtual void *SetMember(void *, char  const*);
	virtual void *SetMember(void *, char  const*, void *);
	virtual void *SetIsServer(bool);
	virtual void *PushLong(long);
	virtual void *GetFlags(int);
	virtual void *FindOnObjectsMetaTable(int, int);
	virtual void *FindObjectOnTable(int, int);
	virtual void *SetMemberFast(void *, int, int);
	virtual void *RunString(char  const*, char  const*, char  const*, bool, bool);
	virtual void *IsEqual(void *, void *);
	virtual void *Error(char  const*);
	virtual void *GetStringOrError(int);
	virtual void *RunLuaModule(char  const*);
	virtual void *FindAndRunScript(char  const*, bool, bool, char  const*);
	virtual void *SetPathID(char  const*);
	virtual void *GetPathID(void);
	virtual void *ErrorNoHalt(char  const*, ...);
	virtual void *Msg(char  const*, ...);
	virtual void *PushPath(char  const*);
	virtual void *PopPath(void);
	virtual void *GetPath(void);
	virtual void *GetColor(int);
	virtual void *PushColor(int, int, int, int);
	virtual void *GetStack(int, void *);
	virtual void *GetInfo(char  const*, void *);
	virtual void *GetLocal(void *, int);
	virtual void *GetUpvalue(int, int);
	virtual void *RunStringEx(char  const*, char  const*, char  const*, bool, bool, bool);
	virtual void *DELETE_ME1(char  const*, int);
	virtual void *GetDataString(int, void **);
	virtual void *ErrorFromLua(char  const*, ...);
	virtual void *GetCurrentLocation(void);
	virtual void *MsgColour(Color  const&, char  const*, ...);
	virtual void *SetState(lua_fState *);
	virtual void *DELETE_ME2(void);
	virtual void *GetCurrentFile(std__string &);
	virtual void *CompileString(Bootil__Buffer &, std__string  const&);
	virtual void *ThreadLock(void);
	virtual void *ThreadUnlock(void);
	virtual void *CallFunctionProtected(int, int, bool);
	virtual void *Require(char  const*);
	virtual void *GetActualTypeName(int);
	virtual void *SetupInfiniteLoopProtection(void);
	virtual void SetGlobal(const char* namename, CLuaFunction f);

	lua_fState *L;
};

#endif