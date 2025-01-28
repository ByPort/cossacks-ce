// IntExplorer.cpp : Defines the entry point for the DLL application.
//

#include "windows.h"
#include <stdio.h>
void EraseSXP();
extern bool LOGMODE;

// TODO: replace with STD init/deinit functions so we can make SO/DLL
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	FILE* F=fopen("dolog.dat","r");
	if(F){
		LOGMODE=1;
		fclose(F);
	};
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			EraseSXP();
			break;
	};
    return TRUE;
};

