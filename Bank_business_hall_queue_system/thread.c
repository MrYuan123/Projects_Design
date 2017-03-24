#include <stdio.h>

#include <conio.h>

#include <windows.h>

#include <process.h>

#include <time.h>

#include "Format.h"

CRITICAL_SECTION Lock;


extern unsigned int __stdcall REFRE(void *pPM){
	
	thread_flag = 1;
	vip_flag = 1;
	EnterCriticalSection(&Lock);
	signal = CreateEvent(NULL,FALSE,FALSE,NULL);
	vip_signal = CreateEvent(NULL,FALSE,FALSE,NULL);
	struct BANK * temp = (struct BANK *)pPM;
	CASE_VIP(temp);
	thread_flag = 0;
	PulseEvent(signal);
	PulseEvent(vip_signal);
	PulseEvent(signal);
	PulseEvent(vip_signal);
	LeaveCriticalSection(&Lock);
	return 0;
}

unsigned int __stdcall sINPUT(void *pPM){
	
	void CASE(char , struct BANK *); 
	struct BANK *temp = ( struct BANK * ) pPM;
	while (1){
		if ( thread_flag == 1 ){
			WaitForSingleObject(signal,INFINITE);
			thread_flag = 0;
			fflush(stdin);
		}
		char c = getch();
		CASE( c , temp );
	}
	return 0;
}

unsigned int __stdcall TIMEP(void *pPM){
	struct BANK *temp = ( struct BANK * )pPM;
	while (1){
		InterlockedIncrement((LPLONG)&(temp->Time));
		clock_t start = clock();
	 	while ((double)(clock() - start)/CLOCKS_PER_SEC < 1);
	 	if ( vip_flag == 1 ){
	 		WaitForSingleObject(vip_signal,INFINITE);
	 		vip_flag = 0;
	 	}
	 	REFRESH(temp);
	}
}

unsigned int __stdcall OUTPUT(void *pPM){
	struct BANK *temp = ( struct BANK * )pPM;
	while(1){
	 	if ( thread_flag == 1 ){
	 		WaitForSingleObject(signal,INFINITE);
	 		thread_flag = 0;
	 	}
	 	system("cls");
	 	STATU_PRINT(*temp);
	 	Sleep(1000);
	 }
	 return 0;
}


void MAIN_THREAD(struct BANK *Bank)
{
	InitializeCriticalSection(&Lock);
	SetCriticalSectionSpinCount(&Lock,1000);
	HANDLE handle1 = (HANDLE)_beginthreadex(NULL,0,OUTPUT,Bank,0,NULL);
	HANDLE handle2 = (HANDLE)_beginthreadex(NULL,0,sINPUT,Bank,0,NULL);
	HANDLE handle3 = (HANDLE)_beginthreadex(NULL,0,TIMEP,Bank,0,NULL);
	WaitForSingleObject(handle2,INFINITE);
	WaitForSingleObject(handle3,INFINITE);
	CloseHandle(signal);
	CloseHandle(handle1);
	CloseHandle(handle2);
	CloseHandle(handle3);
	DeleteCriticalSection(&Lock);
}
