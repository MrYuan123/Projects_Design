#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "Format.h"

int main(int argc, char *argv[]) {
	struct BANK *B = ( struct BANK * ) malloc ( sizeof ( struct BANK ) );
	FILE *format = fopen ( "Format.dat" , "rb" );
	INIT_BANK(B,format);
	INIT_VIP(B); 
	MAIN_THREAD(B);
	return 0;
}
