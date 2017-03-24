#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "Format.h"

char WINDOW_STATU_PRINT[4][11]={"WAITING","DEALING","PAUSE","OUT"};
char GUEST_TYPE_PRINT[2][4]={"VIP","NOR"};
char BANK_STATU_PRINT[3][5]={"OPEN","CLOSE","LAST"};
char WORK_TYPE_PRINT[6][11]={"WITHDRAW","SAVE","ADDUSR","LOGOUT","STOCK","NONE"};


void STATU_PRINT(struct BANK Bank){
	
	void BANK_MAINTAIN_ALL(struct BANK);
	void BANK_MAINTAIN_PER_SEC(struct BANK,int,int,int);
	char *NUM_TO_STRING( int , enum GUEST_TYPE );
	char *GET_TIME( int , int , int );
	int sec = Bank.Time % 60;
	int hour = Bank.Time / 3600;
	int min = ( Bank.Time - hour * 3600 - sec ) / 60;	
	int i;
	if ( Bank.Current_Statu == CLOSE){
		printf("---------------------------------------------------------------------------\n\n");
		printf("                         Bank is Closed!\n\n");
		printf("---------------------------------------------------------------------------");
		Sleep(3000);
		BANK_MAINTAIN_ALL(Bank);
		exit(0);
	}
	printf("-----------------------------------------------------------------------------\n\n");
	printf("                   Bank System V1.0     Statu : %s\n",BANK_STATU_PRINT[Bank.Current_Statu]);
	printf("                    Developed by team 201421131202\n\n");
	printf("    Current Time : %s\n" , GET_TIME(sec,min,hour) );
	if ( Bank.Customer.NOR_Line == NULL) 
		printf("    Normal guests have come to Number %s ,", NUM_TO_STRING( Bank.Customer.NOR_Len , NOR ));
	else 
		printf("    Normal guests have come to Number %s ,", NUM_TO_STRING( Bank.Customer.NOR_Line->number , NOR ));
	if ( Bank.Customer.NOR_Line == NULL) 
		printf(" no guests are waiting!\n");
	else 
		printf(" %d guests are waiting!\n", Bank.Customer.NOR_Last->number - Bank.Customer.NOR_Line->number + 1);
	if ( Bank.Customer.VIP_Line == NULL) 
		printf("    VIP guests have come to Number %s ,", NUM_TO_STRING(Bank.Customer.VIP_Len ,VIP));
	else
		printf("    VIP guests have come to Number %s ,", NUM_TO_STRING(Bank.Customer.VIP_Line->number,VIP));
	if ( Bank.Customer.VIP_Line == NULL) 
		printf(" no guests are waiting!\n\n");
	else
		printf(" %d VIPs are waiting!\n\n", Bank.Customer.VIP_Last->number - Bank.Customer.VIP_Line->number + 1 );
	
	for ( i = 1 ; i <= 8 ; i++ ){
		printf("    Window %d (%s) ", i , GUEST_TYPE_PRINT[Bank.Desk.wins[i].type]);
		if ( Bank.Desk.wins[i].statu == DEALING){
			printf("is dealing with %s ",  Bank.Desk.wins[i].type == VIP ? "VIP" : "NORMAL");
			printf("%s's ", NUM_TO_STRING( Bank.Desk.wins[i].now , Bank.Desk.wins[i].type ));
			printf("%s work.\n", WORK_TYPE_PRINT[ Bank.Desk.wins[i].work ] );
		}
		else 
			printf("is %s.\n", WINDOW_STATU_PRINT[ Bank.Desk.wins[i].statu ] );
	}
	
	printf("\n-----------------------------------------------------------------------------\n\n");
	printf("              Normal_Guest arriving : G   VIP_Guest arriving : V            \n");
	printf("              Relex Apply : R       Off Work : Q         VIP : X              \n\n");
	BANK_MAINTAIN_PER_SEC(Bank,hour,min,sec);
	
}

char *GET_TIME(int secd,int mind,int hourd){
	
	char sec[3],hour[3],min[3];
	sec[0] = secd / 10 + '0' ; sec[1] = secd % 10 + '0' ;sec[2] = '\0';
	hour[0] = hourd / 10 + '0' ; hour[1] = hourd % 10 + '0' ;hour[2] = '\0';
	min[0] = mind / 10 + '0' ; min[1] = mind % 10 + '0';min[2] = '\0';
	char *time = (char *) malloc ( 15 * sizeof ( char ) );
	sprintf( time , "%s : %s : %s", hour , min , sec );
	return time;
	
}

char *NUM_TO_STRING(int num,enum GUEST_TYPE guest){		// int.toString

	char *s = (char *) malloc ( 4 * sizeof(char) );
	s[2] = num % 10 + '0';
	s[1] = ( num % 100 ) / 10 + '0';
	if (guest == NOR)
		s[0] = num / 100 + '0';
	else 
		s[0] = 'V';
	s[3]='\0';
	return s;
	
}

void BANK_MAINTAIN_PER_SEC(struct BANK Bank,int hour,int min,int sec){
	
	FILE *out = fopen ( "Maintain_Per_Sec.dat", "a" );
	fprintf(out,"Time:%s  MAX_NOR:%s MAX_VIP:%s ",GET_TIME(sec,min,hour),NUM_TO_STRING(Bank.Customer.NOR_Len,NOR),NUM_TO_STRING(Bank.Customer.VIP_Len,VIP));
	int i;
	for ( i = 1 ; i <= 8 ; i++){
		fprintf(out,"Win%d-%s",i,WINDOW_STATU_PRINT[Bank.Desk.wins[i].statu]);
		if (Bank.Desk.wins[i].statu == DEALING)
			fprintf(out,"-%s",Bank.Desk.wins[i].type == VIP ?NUM_TO_STRING(Bank.Desk.wins[i].now,VIP):NUM_TO_STRING(Bank.Desk.wins[i].now,NOR));
		fprintf(out," ");
	}
	fprintf(out,"%s",Bank.Current_Statu != OPEN ? "Y":"N");
	fprintf(out,"\n");
	fclose(out);
	return;		
}

void BANK_MAINTAIN_ALL(struct BANK Bank){
	FILE *out = fopen ( "Maintain_All.dat","w");
	fprintf(out,"Total_normal_guests: %d\n",Bank.Customer.NOR_Len);
	fprintf(out,"Total_vip_guests: %d\n\n",Bank.Customer.VIP_Len);
	int i;
	for ( i = 1 ; i <= 8 ; i++){
		fprintf(out,"Win%d :  Total_guests:%d  ",i,Bank.Desk.wins[i].nor_count+Bank.Desk.wins[i].vip_count);
		fprintf(out,"Total_time:%d  ",Bank.Desk.wins[i].time_used);
		if (Bank.Desk.wins[i].nor_count+Bank.Desk.wins[i].vip_count == 0)
			fprintf(out,"Average_time:%d\n",0);
		else
			fprintf(out,"Average_time:%d\n",Bank.Desk.wins[i].time_used/(Bank.Desk.wins[i].nor_count+Bank.Desk.wins[i].vip_count));
	}
	fclose(out);
}
