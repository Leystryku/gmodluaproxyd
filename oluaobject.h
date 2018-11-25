#ifndef VMT_PROLOG_EPILOG_REPLACEMENT_HPP
#define VMT_PROLOG_EPILOG_REPLACEMENT_HPP

#include "include/ILuaShared.h"
#include "include/ILuaInterface.h"
#include "include/GarrysMod//Lua/Interface.h"
#include "include/GarrysMod/Lua/Types.h"
#include "include/GarrysMod/Lua/LuaObject.h"

using namespace GarrysMod::Lua;

#pragma optimize("", off)


extern int ourluaobject_funcaddr;
extern int oluaobject_funcaddr;

extern CLuaInterface *cour;
extern CLuaInterface *cli_bak;

extern bool cancerfags;

extern int iprintf;
extern int iGETEIP;
extern int iJMPBACK;


void*bak = 0;

char *printmask = 0;
int32_t grobset = 0;
int32_t ouraddr = 0;
int32_t curretnaddr = 0;

int32_t oldeax = 0;
int32_t oldecx = 0;

//stack for returns
int32_t jmpbacks[0xFFF] = { 0 }; // max calls into this shit = 0xFFF
int32_t jmpbacks_i = 0;


__declspec(naked) void JMPBACK()
{

	//HE RETURNED TO US; AFTER THIS PAINFUL JOURNEY
	__asm pushad;

	//custom global epilog here

	jmpbacks_i -= 1;
	curretnaddr = jmpbacks[jmpbacks_i];
	jmpbacks[jmpbacks_i] = 0;
	//printf("go beck\n");

	__asm {
		popad;
		push curretnaddr;
		retn; // AND THUS WE RETURN TO OUR LORD AND SAVIOR
	}

};

__declspec(naked) void GETEIP()
{
	__asm
	{
		pop eax;
		mov ouraddr, eax;
		jmp eax
	}

}

class OLuaObject
{
public:

	inline OLuaObject::OLuaObject(ILuaBase* lua)
	{

		*(char*)((char*)this + 4) = 0;
		*(int32_t*)((char*)this + 8) = -1;
		*(int32_t*)((char*)this + 12) = -1;
		*(int32_t**)((char*)this + 16) = (int32_t*)lua;

		if (oluaobject_funcaddr)
		{
			*(void**)this = (void*)oluaobject_funcaddr;
		}

		//printf("[WARN] OLuaObject default constructor called\n");
	}


	inline OLuaObject::~OLuaObject()
	{
		int m_iRef = -1; 
		ILuaBase*m_pLua = 0; 

		if ((int*)((char*)this + 12))
		{
			m_iRef = *(int*)((char*)this + 12);
		}
		if ((ILuaBase**)((char*)this + 16))
		{
			m_pLua = *(ILuaBase**)((char*)this + 16);
		}

		if (m_iRef != -1)
		{
			if (cancerfags)
			{
				cour->ReferenceFree(m_iRef);
			}
			else {
				m_pLua->ReferenceFree(m_iRef);
			}
		}

		*(char*)((char*)this + 4) = 0;
		*(int32_t*)((char*)this + 8) = -1;
		*(int32_t*)((char*)this + 12) = -1;

		//printf("[WARN] OLuaObject default deconstructor called\n");
	}


	virtual void* __cdecl copyfunc()
	{
		__asm
		{
			nop;
			int 3;
			nop;
			nop;
			mov eax, iGETEIP;
			call eax;

			push ebp;
			mov ebp, esp;

			mov oldecx, ecx;
			mov oldeax, eax;
					
			mov eax, ebp;
			mov eax, [eax + 4];
			mov curretnaddr, eax;

			mov eax, iJMPBACK;
			mov[ebp + 4], eax;

			mov esp, ebp;
			pop ebp;

			pushad;

		}


		jmpbacks[jmpbacks_i] = curretnaddr;
		jmpbacks_i += 1;

		while (*(char*)ouraddr != '\x90' || *(char*)(ouraddr+1) != '\x90')
			ouraddr--;

		//custom global prolog here
		

		if (!printmask)
			printmask = "==%d __ %x\n";

		//if (*(CLuaInterface**)((char*)oldecx + 16) != cour)
		{

		
		if (cancerfags)
		{

			if (*(CLuaInterface**)((char*)oldecx + 16) != cour)
			{
				*(CLuaInterface**)((char*)oldecx + 16) = cour;	
				/*
				__asm
				{
					mov eax, iprintf;
					push 1337;
					push grobset;
					push printmask;
					call eax;
					add ESP, 0x0C;
				}*/


			}
		}
		else {
			if (*(CLuaInterface**)((char*)oldecx + 16) != cli_bak)
			{
				*(CLuaInterface**)((char*)oldecx + 16) = cli_bak;
				/*
				__asm
				{
					mov eax, iprintf;
					push 1337;
					push grobset;
					push printmask;
					call eax;
					add ESP, 0x0C;
				}
				*/

			}
		}

		}


		for (grobset = 0; grobset < 85; grobset++)
			if (((void**)ourluaobject_funcaddr)[grobset] == (void*)ouraddr)
				break;


		/*
		__asm
		{
			mov eax, iprintf;
			push curretnaddr;
			push grobset;
			push printmask;
			call eax;
			add ESP, 0x0C;
		}*/

		grobset = grobset * 4;

		__asm
		{
			popad;

			mov eax, 0;
			add eax, grobset;
			add eax, oluaobject_funcaddr;
			mov eax, [eax];
			push eax;
			mov eax, oldeax;
			retn;

		}
	};


	char dick[0x500];
};

#pragma optimize("", on)
#endif