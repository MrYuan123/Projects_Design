#ifndef Format
#include <stdio.h>
#include <windows.h>
#define Format 
#define ERROR 0
#define REJECT 0
#define ACCEPT 1
#define TRUE 1
#define FALSE 0

enum WORK_TYPE{WITHDRAW,SAVE,ADDUSR,LOGOUT,STOCK,NONE};	// The present work of current window including save money .etc...
enum WINDOW_STATU{WAITING,DEALING,PAUSE,OUTW};			// The present statu of current window including waiting for customers .etc...
enum GUEST_TYPE{VIP,NOR};
enum BANK_STATU{OPEN,CLOSE,LAST};

struct VIP{
	char name[21];
	char ID[9];
	int deposit;
	struct VIP *next;
};

struct FORMAT{
	int MaxSeqLen;
    int MinSec;
    int MaxSec;
    int MinRestSec;
    int MaxRestSec;
    int VIPSERVLen;
    struct VIP *data;
};

struct Single_Window{									// Struct of a single window
	enum WINDOW_STATU statu;							// Window's current statu
	enum WORK_TYPE work;								// Window's current work type
	enum GUEST_TYPE type;
	int now;											// Current processed customer's number
	int nor_count;
	int vip_count;
	int time_used;
	int end_time;
	int pause_start_time;
	int pause_end_time; 
//	int satisfy;
//	int unsatisfy;
};

struct HAND{											// Struct of a customer
	int time;											// Customer's time
	int number;											// Customer's number
	enum WORK_TYPE work;								// Customer's work type
	enum GUEST_TYPE type;								// Customer's level
	struct HAND *next;
};

struct WINDOWS{											// Gather of all of the windows
	struct Single_Window wins[9];						
	int VIP_open;									// VIP windows be open
//	int VIP_pause[3];									// VIP windows be pause
	int NOR_open;
//	int NOR_pause[7];									// Same to NORMAL guest
	int VIP_SER_TIME;
};

struct GUEST{											// Gather of all of the guests
	int VIP_Len;										// How many VIPs have came
	int VIP_DEALED;
	int NOR_Len;										// Same to NORMAL guest
	int NOR_DEALED;
	struct HAND *VIP_Line,*VIP_Last;							// VIPs' head and rear
	struct HAND *NOR_Line,*NOR_Last;							// NORs' head and rear
};

struct BANK{											// Whole bank
	struct WINDOWS Desk;								// WINDOWS
	struct GUEST Customer;								// CUSTOMERS
	struct FORMAT Announce;
	struct VIP *vip_data;
	enum BANK_STATU Current_Statu;
	long long Time;
};

int thread_flag;
int vip_flag;
int maintain_flag;

HANDLE vip_signal;
HANDLE signal;
unsigned int __stdcall REFRE(void *pPM);
void unvalid(char *,int);
void RELAX_WINDOW(struct BANK *);
void INIT_BANK(struct BANK *,FILE *);
void STATU_PRINT(struct BANK);
int CUSTOMER_COMING(struct BANK *,enum GUEST_TYPE);
void CLOSE_BANK(struct BANK *);
void CASE_VIP(struct BANK *Bank);
void CASE(char c,struct BANK *Bank);
void INIT_VIP(struct BANK *Bank);
int REFRESH(struct BANK *);
void MAIN_THREAD(struct BANK *);
#endif
