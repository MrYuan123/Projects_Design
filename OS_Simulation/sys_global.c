/*
整个系统的全局变量
 */
#include"memory/MACRO.h"
#include"process/MACRO.h"
#include"memory/basic_data_struct.h"

#include<stdio.h>

struct GLOBAL{
	FILE *mem;//操作MEM.txt
	FILE *disk;//操作DISK.txt
	char diskBuffer[DISK_SIZE/PAGE_SIZE];//表示disk中页表空闲与否的位表
	char memBuffer[MEM_SIZE/PAGE_SIZE];//表示mem中页表空闲与否的位表

	memPageRecord memMMU[MEM_SIZE/PAGE_SIZE];//纪录储存在mem中的页表
	processEntry processEntryList;//在内存中的所有进程
}global;
