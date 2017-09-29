/*
整个系统的全局变量
 */
#include "memory/MACRO.h"
#include "memory/basic_data_struct.h"
#include "process/MACRO.h"

#include<stdio.h>

typedef struct{
	FILE *mem;//操作MEM.txt
	FILE *disk;//操作DISK.txt
	char diskBuffer[DISK_SIZE/PAGE_SIZE];//表示disk中物理页表空闲与否的位表
	char memBuffer[MEM_SIZE/PAGE_SIZE];//表示mem中物理页表空闲与否的位表

	struct linnerPageRecord *linnerPageList;//表示逻辑页面的分配情况
	struct memPageRecord *memMMU[MEM_SIZE/PAGE_SIZE];//纪录mem中物理页表对应的逻辑页表
	struct diskPageRecord *diskMMU[MEM_SIZE/PAGE_SIZE];//纪录disk中l页表对应的逻辑页表
	struct processEntry *processEntryList;//在内存中的所有进程
}GLOBAL;

GLOBAL global;