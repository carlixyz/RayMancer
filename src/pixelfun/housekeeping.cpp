#include "housekeeping.h"

#define WIN32

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>

void** hk_pointers=NULL;
int hk_numalloc=0;
int hk_numpointers=0;

void HKError(char* msg)
{
#ifdef WIN32
	MessageBox(NULL, msg, "Housekeeping error", MB_ICONEXCLAMATION);
#endif
	exit(0);
}

int HKGetNewPointer()
{
	if(hk_numpointers>=hk_numalloc)
	{
		hk_numalloc*=2;
		if(hk_numalloc==0)
			hk_numalloc=128;
		hk_pointers=(void**)realloc(hk_pointers, hk_numalloc*sizeof(void*));
	}
	return hk_numpointers++;
}

void HKPtr(void* ptr)
{
	if(ptr==NULL) HKError("HKPtr: NULL pointer");
	int newid=HKGetNewPointer();
	hk_pointers[newid]=ptr;
}

void HKFree()
{
	for(int i=0;i<hk_numpointers;i++)
		free(hk_pointers[i]);
	hk_numpointers=0;
}

