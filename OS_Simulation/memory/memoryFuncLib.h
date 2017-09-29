/*
所有的函数包含
 */
#include<stdio.h>
#include "MACRO.H"
//#include "basic_data_struct.h"
#include "../sys_global.h"
#include "../error/systemError.c"
//===========================================================================<PART 1>======================================================================
void Initialize(void);									//初始化模拟内存和磁盘的文件

void BitMapToBuffer();	//把文件中的位图拷入到真正的内存数组中，使读取速度增快

void BufferToBitMap();	//把数组中的数据拷入到文件中的位图中，使得下次打开有正确的位图

int FindFreeBitMapMem();					//查找内存中的第一个空闲位图

int FindFreeBufferDisk();				//查找硬盘中的第一个空闲位图

int FindTotalFreeBufferMem();				//查找内存中所有空闲位图的数目总数

void MaskBuffer(char *buffer, int numOfBit);				//置特定位图的值为1

void ClearBuffer(char *buffer, int numOfBit);			//置特定位图的值为0

//===========================================================================<PART 2>======================================================================
int _write(unsigned int virAddr, unsigned int size, unsigned int count, void *buffer);				//写入字符

void *_read(unsigned int virAddr, unsigned int size, unsigned int count);						//读出字符

int VirAddr2LinnerAddr(unsigned int virAddr);			//虚地址和实地址的转换

int GetPhyAddr(unsigned char processName, int linnerAddr);						//由线性地址计算得到内存或磁盘中的物理地址

int PageSwap(unsigned int pageInDisk, unsigned int pageInMem);			//页的换入/换出

unsigned int PageSwapStratgy(unsigned char processName);//得到该段中可以换出的页表下标，FIFO
int GetPageOutAddr(unsigned int addrOfPageIn);			//得到内存中换出页的地址，同时更新其上一级页表

int CopyIntoMemory(unsigned int addrOfPageIn, unsigned int addrOfPageOut, int writeBack);	
//根据已经得到的换入页的辅存地址和换出页的主存地址，进行页面的换入/换出。
//对于内存已满的情况，需要将原来内存中的页面备份到辅存；
//如果换入时内存未满，则没有必要把内存中原本为空的内容备份

int GetFreePageAddr();									//在内存未满的情况下发生缺页异常，返回内存中的空闲页地址

int SetFatherPageWhenIn(unsigned int fatherPage, unsigned int newMemoryAddr, int rw, unsigned int addrInQueue);
//当页面换入主存时，设置他的上级页表项，指向主存地址，将P位置1，
//对于写操作，将r, m位置1；对于读操作，将r位置1。更新搜索队列中相应位置内容

int SetFatherPageWhenOut(unsigned int fatherPage, unsigned int newDiskAddr);	//当页面换出时，设置它的上级页表项，指向辅存地址，将P位置0

//===========================================================================<PART 3>======================================================================
int CreateProcess(unsigned char processName, int sizeInByte);		//创建一个进程

void DelProcess(unsigned char processName);						//注销进程

void AllocFirstPage(FILE *men, int addr, int size, int *memBuffer, int *diskBuffer);	//分配进程的一级页表

void AllocSecondPage(FILE *mem, FILE *disk, int addr, int size, int MorD, int *memBuffer, int *diskBuffer);	//对指定的二级页表进行分配

void AllocPageFrame(FILE *mem, int addr);				//分配进程的页

void DelProcess(unsigned char processName);						//删除特定名字的进程

void DelFirstPage(FILE *mem, int addr, int size, int *memBuffer, int *diskBuffer);	//删除进程的一级页表

void DelSecondPage(FILE *mem, FILE *disk, int addr, int size, int MorD, int *memBuffer, int *diskBuffer);	//删除进程的二级页表

int FindFreeProcessEntry(FILE *mem);					//查找空闲进程表项

int FindProcssEntry(FILE *mem, char *processName);		//查找特定名字的进程的进程表项首地址

char Read(char *processName, unsigned int virAddr);		//读进程
