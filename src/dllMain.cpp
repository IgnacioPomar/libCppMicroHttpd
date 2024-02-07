﻿/*********************************************************************************************
*	Name		: dllmain.h
*	Description	: Entry point of the dll
********************************************************************************************/


#include <Windows.h>

#include "libCppMicroHttpd_pragmalib.h"



BOOL APIENTRY DllMain (HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
