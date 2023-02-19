/*
#include<windows.h>
*/
void CAP_pause_spin(DWORD);



//function
void CAP_pause_spin(DWORD millisecond)
{
	DWORD start=GetTickCount();
	while(GetTickCount()-start< millisecond){}
}
