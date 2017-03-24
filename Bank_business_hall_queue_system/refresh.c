#include <malloc.h>
#include <stdio.h>
#include <time.h> 
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "Format.h"

extern char WINDOW_STATU_PRINT[4][11];
extern char GUEST_TYPE_PRINT[2][4];
extern char BANK_STATU_PRINT[3][5];
extern char WORK_TYPE_PRINT[6][11];

int CUSTOMER_COMING(struct BANK *Bank,enum GUEST_TYPE guest){	// return the task statu

	if (Bank->Current_Statu == LAST || Bank->Current_Statu == CLOSE)
		return FALSE;
	struct HAND *newg;
	newg = (struct HAND *)malloc(sizeof(struct HAND));
	if ( guest == NOR )
		newg->number = ++Bank->Customer.NOR_Len;
	else 
		newg->number = ++Bank->Customer.VIP_Len;
	srand((unsigned)newg->number);
	rand();
	newg->work = (1+(int)(5.0*rand()/(RAND_MAX+1.0))) % 5;
	newg->time = ((1+(int)(100.0*rand()/(RAND_MAX+1.0))) % ( Bank->Announce.MaxSec - Bank->Announce.MinSec )) + Bank->Announce.MinSec;
	newg->type = guest;
	newg->next = NULL;
	if (guest == NOR){
		if (Bank->Customer.NOR_Last == NULL || Bank->Customer.NOR_Line == NULL ){
			Bank->Customer.NOR_Last = newg;
			Bank->Customer.NOR_Line = newg;
		}
		else {
			Bank->Customer.NOR_Last->next = newg;
			Bank->Customer.NOR_Last = newg;
		}
	}
	else {
		if (Bank->Customer.VIP_Last == NULL || Bank->Customer.VIP_Line == NULL ){
			Bank->Customer.VIP_Last = newg;
			Bank->Customer.VIP_Line = newg;
		}
		else {
			Bank->Customer.VIP_Last->next = newg;
			Bank->Customer.VIP_Last->next = newg;
		}
	}
	return TRUE;
}

int REFRESH(struct BANK *Bank){
	
	void CHECK_LEN(struct BANK *);
	void CHECK_WINDOW_TYPE(struct BANK *);
	void RELEX_WINDOW(struct BANK *);
	void CASE(char,struct BANK*);
	
	if ( Bank->Current_Statu == CLOSE )
		return REJECT;
		
	CHECK_LEN(Bank);
	
	CHECK_WINDOW_TYPE(Bank);
	
	int Close_flag = 0;
	
	int i;
	for ( i = 1 ; i <= 8 ; i ++ ){
		if ( Bank->Desk.wins[i].type == NOR){
			if ( Bank->Desk.wins[i].statu == DEALING && Bank->Desk.wins[i].pause_start_time == Bank->Time ){
				Bank->Desk.wins[i].statu = PAUSE;
				Bank->Desk.wins[i].pause_start_time = -1;
			}
			if ( Bank->Desk.wins[i].statu == PAUSE && Bank->Desk.wins[i].pause_end_time == Bank->Time ){
				Bank->Desk.wins[i].statu = WAITING;
				Bank->Desk.wins[i].pause_end_time = -1;
			}
			if ( Bank->Desk.wins[i].statu == DEALING ){
					Close_flag = 1;
				Bank->Desk.wins[i].time_used++;
			if ( Bank->Desk.wins[i].end_time == Bank->Time )
					Bank->Desk.wins[i].statu = WAITING;
			}
			if ( Bank->Desk.wins[i].statu == WAITING ){
				if ( Bank->Customer.NOR_Line != NULL ){
					Bank->Desk.wins[i].statu = DEALING;
					Bank->Customer.NOR_DEALED++;
					Bank->Desk.wins[i].now = Bank->Customer.NOR_Line->number;
					Bank->Desk.wins[i].nor_count++;
					Bank->Desk.wins[i].end_time = Bank->Time + Bank->Customer.NOR_Line->time;
					Bank->Desk.wins[i].work = Bank->Customer.NOR_Line->work;
					struct HAND *temp = Bank->Customer.NOR_Line;
					Bank->Customer.NOR_Line = Bank->Customer.NOR_Line->next; 
					free(temp);
				}
			}
		}
	
		else {
			if ( Bank->Desk.wins[i].statu == DEALING && Bank->Desk.wins[i].pause_start_time == Bank->Time ){
				Bank->Desk.wins[i].statu = PAUSE;
				Bank->Desk.wins[i].pause_start_time = -1;
			}
			if ( Bank->Desk.wins[i].statu == PAUSE && Bank->Desk.wins[i].pause_end_time == Bank->Time ){
				Bank->Desk.wins[i].statu = WAITING;
				Bank->Desk.wins[i].pause_end_time = -1;
			}
			if ( Bank->Desk.wins[i].statu == DEALING ){
					Close_flag = 1;
				Bank->Desk.wins[i].time_used++;
				if ( Bank->Desk.wins[i].end_time == Bank->Time )
					Bank->Desk.wins[i].statu = WAITING;
			}
			if ( Bank->Desk.wins[i].statu == WAITING ){
				if ( Bank->Customer.VIP_Line != NULL ){
					Bank->Desk.wins[i].statu = DEALING;
					Bank->Customer.VIP_DEALED++;
					Bank->Desk.wins[i].now = Bank->Customer.VIP_Line->number;
					Bank->Desk.wins[i].work = Bank->Customer.VIP_Line->work;
					Bank->Desk.wins[i].vip_count++;
					Bank->Desk.wins[i].end_time = Bank->Time + Bank->Customer.VIP_Line->time;
					struct HAND *temp = Bank->Customer.VIP_Line;
					Bank->Customer.VIP_Line = Bank->Customer.VIP_Line->next; 
					free(temp);
				}
			}
		}
	}
	
	if ( Bank->Customer.VIP_Line ==NULL && Bank->Customer.NOR_Line  == NULL && Close_flag == 0 && Bank->Current_Statu == LAST )
		Bank->Current_Statu = CLOSE;
	return ACCEPT;
}

void CHECK_WINDOW_TYPE(struct BANK *Bank){
	
	if ( Bank->Customer.VIP_Len - Bank->Customer.VIP_DEALED == 0 ){
		int i;
		for ( i = 7 ; i < 7 + Bank->Desk.VIP_open ; i++ )
			if (Bank->Desk.wins[i].statu != DEALING)
				Bank->Desk.wins[i].type = NOR;
	}
	
	if ( Bank->Customer.VIP_Len - Bank->Customer.VIP_DEALED != 0 ){
		int i;
		for ( i = 7 ; i < 7 + Bank->Desk.VIP_open ; i++ )
			if (Bank->Desk.wins[i].statu != DEALING)
				Bank->Desk.wins[i].type = VIP;
			else if ( Bank->Time == Bank->Desk.wins[i].end_time )
				Bank->Desk.wins[i].type = VIP;
	}

}

void CHECK_LEN(struct BANK *Bank){
			
	if ( Bank->Customer.NOR_Len - Bank->Customer.NOR_DEALED > 3 * Bank->Desk.NOR_open){
		if (Bank->Desk.NOR_open <6){
			++Bank->Desk.NOR_open;
			Bank->Desk.wins[Bank->Desk.NOR_open].statu = WAITING ;
		}
	}
	
	if ( Bank->Customer.NOR_Len - Bank->Customer.NOR_DEALED < 2 * Bank->Desk.NOR_open && Bank->Desk.NOR_open > 3 )
		Bank->Desk.wins[Bank->Desk.NOR_open--].statu = OUTW;
		
	if ( Bank->Desk.VIP_open == 1 && Bank->Customer.VIP_Len - Bank->Customer.VIP_DEALED >= 3 ){
		Bank->Desk.VIP_open = 2;
		Bank->Desk.wins[8].statu = WAITING;
	}
	
	if ( Bank->Desk.VIP_open == 2 && Bank->Customer.VIP_Len - Bank->Customer.VIP_DEALED < 3 ){
		if ( Bank->Time - Bank->Desk.VIP_SER_TIME >= Bank->Announce.VIPSERVLen ){
			Bank->Desk.VIP_open = 1;
			Bank->Desk.VIP_SER_TIME = Bank->Time;
		} 
	}
	else 
		Bank->Desk.VIP_SER_TIME = Bank->Time ;

}

void CASE(char c,struct BANK *Bank){
	
	int CHECK_VIP(struct BANK*);
	if ( Bank->Current_Statu == CLOSE )
		return;
	if ( c == 'g' || c == 'G' ){
		if ( Bank->Current_Statu == OPEN ){
			CUSTOMER_COMING(Bank,NOR);
		}
		else{
			if ( Bank->Current_Statu == LAST)
				unvalid("Bank is ready for close!",1000);
			else 
				unvalid("Bank is already close!",1000);
		}
	}
	else if ( c == 'X' || c == 'x' ){
		thread_flag = 1;
		vip_flag = 1;
		Sleep(10);
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0,REFRE,Bank,0,NULL);
		Sleep(10);
		WaitForSingleObject(signal,INFINITE);
		CloseHandle(handle);
		thread_flag = 0;
		vip_flag = 0;
		fflush(stdin);
	}
	else if ( c == 'Q' || c == 'q' ){
		CLOSE_BANK(Bank);
		system("cls"); 
		STATU_PRINT(*Bank);
	} 
	else if ( c == 'R' || c == 'r' ){
		RELAX_WINDOW(Bank);
		fflush(stdin);
	}
	else if ( c == 'V' || c == 'v' ){
		if ( Bank->Current_Statu == OPEN ){
			if ( CHECK_VIP(Bank) == 1 )
				CUSTOMER_COMING(Bank,VIP);
			else CUSTOMER_COMING(Bank,NOR);
		}
		else{
			if ( Bank->Current_Statu == LAST)
				unvalid("Bank is ready for close!",1000);
			else 
				unvalid("Bank is already close!",1000);
		}
	}
	
}

int CHECK_VIP(struct BANK *Bank){
	
	thread_flag = 1;
	vip_flag = 1;
	signal = CreateEvent(NULL,FALSE,FALSE,NULL);
	vip_signal = CreateEvent(NULL,FALSE,FALSE,NULL);
	char id[8];
	int i;
	for ( i = 0 ; i < 3 ; i++ ){
		system("cls");
		system("color 4");
		printf("\n\n----------------------------------------------------------------\n");
		printf("\n               Please input your VIP ID : ");
		system("color 7");
		scanf("%s",id);
		struct VIP *findp = Bank->vip_data;
		while ( findp != NULL ){
			if ( strcmp (findp->ID,id) == 0)
				break;
			findp = findp -> next;
		}
		if( findp == NULL || strcmp(findp->ID,id) != 0 ) {
			printf("\n                      Can't Find ! Input again ! \n");
			Sleep(500);
		}
		else if ( strcmp(findp->ID , id) == 0 ){
			printf("\n                          Welcome!");
			Sleep(300);
			PulseEvent(signal);
			PulseEvent(vip_signal);
			CloseHandle(vip_signal);
			CloseHandle(signal);
			thread_flag = 0;
			vip_signal = 0;
			return 1;
		}
	}
	printf("\n   			        Turn to Normal guest !\n");
	Sleep(300);
	PulseEvent(signal);
	PulseEvent(vip_signal);
	CloseHandle(vip_signal);
	CloseHandle(signal);
	thread_flag = 0;
	vip_signal = 0;
	return 0;		
}
void RELAX_WINDOW(struct BANK *Bank){
	
	thread_flag = 1;
	vip_flag = 1;
	signal = CreateEvent(NULL,FALSE,FALSE,NULL);
	vip_signal = CreateEvent(NULL,FALSE,FALSE,NULL);
	system("cls");
	printf("\n\n-------------------------------------------------------------------\n\n");
	printf("                         Choose the working Window \n\n");
	int i;
	for ( i = 1 ; i <= 8 ; i++ )
		if ( Bank->Desk.wins[i].statu == WAITING || Bank->Desk.wins[i].statu == DEALING )
			printf("                              Window [ %d ]\n",i);
	char c;
	srand((unsigned)Bank->Time);
	c = getch();
	if ( '0' >= c || c >= '9'){
		printf("\n                              Wrong Input!\n");
		Sleep(600);
	}
	else if ( Bank->Desk.wins[c-'0'].statu == PAUSE || Bank->Desk.wins[c-'0'].statu == OUTW){
		printf("\n                     This Window is not working!\n");
		Sleep(600);
	}
	else {
		if ( Bank->Desk.wins[c-'0'].statu == DEALING ){
			Bank->Desk.wins[c-'0'].pause_start_time = Bank->Desk.wins[c-'0'].end_time;
			Bank->Desk.wins[c-'0'].pause_end_time = Bank->Desk.wins[c-'0'].end_time + (1+(int)(100.0*rand()/(RAND_MAX+1.0)))  % ( Bank->Announce.MaxRestSec - Bank->Announce.MinRestSec ) + Bank->Announce.MinRestSec;
			printf("       Window %d will relex after current mission completed!\n",c-'0');
		}
		else {
			Bank->Desk.wins[c-'0'].statu = PAUSE;
			Bank->Desk.wins[c-'0'].pause_end_time = Bank->Time + rand() % ( Bank->Announce.MaxRestSec - Bank->Announce.MinRestSec ) + Bank->Announce.MinRestSec;
			printf("	             Window %d relexs successfully!\n",c-'0');
		}
		Sleep(500);
	}
	Sleep(300);
	PulseEvent(signal);
	PulseEvent(vip_signal);
	CloseHandle(vip_signal);
	CloseHandle(signal);
	thread_flag = 0;
	vip_signal = 0;
	return;		
}

void CLOSE_BANK(struct BANK *Bank){						// Give the close order
	if (Bank->Customer.VIP_Len !=0 || Bank->Customer.NOR_Len !=0)
		Bank->Current_Statu = LAST;
	else 
		Bank->Current_Statu = CLOSE;
}

void unvalid(char *s,int timestop){
	
	thread_flag = 1;
	vip_flag = 1;
	signal = CreateEvent(NULL,FALSE,FALSE,NULL);
	system("cls");
	system("color 4");
	printf("\n\n----------------------------------------------------------------\n");
	printf("\n         Unvalid operation : %s\n\n",s);
	printf("----------------------------------------------------------------");
	system("color 7");
	Sleep(timestop);
	PulseEvent(signal);
	CloseHandle(signal);
	vip_flag = 0;
	thread_flag = 0;	
}

