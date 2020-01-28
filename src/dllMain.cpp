/*********************************************************************************************
*	Name		: dllmain.h
*	Description	: Entry point of the dll
********************************************************************************************/


#include <Windows.h>

#pragma comment (lib, "Wtsapi32.lib")
#ifdef __has_include
#  if __has_include(<cryptopp/aes.h>) // crypto++ exists
#pragma comment (lib, "cryptopp.lib")
#  endif
#endif


BOOL APIENTRY DllMain (HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
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
