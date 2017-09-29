/*
内存基础初始化功能
 */
#include "memoryFuncLib.h"
#include <malloc.h>
#include <time.h>
#include <string.h>

void Initialize(void)									//初始化模拟内存和磁盘的文件
{
	global.mem = fopen("MEM.txt", "rb+");//读写打开一个二进制文件，允许读和写 
	global.disk = fopen("DISK.txt", "rb+");//读写打开一个二进制文件，允许读和写
	if (global.mem == NULL || global.disk == NULL)
	{
		char *errorMsg = "MEM DISK Initialize FAILED. file could not open";
		shutDownError(errorMsg);
	}
	
	global.processEntryList = (struct processEntry *)malloc(sizeof(struct processEntry));
	global.processEntryList->processName = 0;//初始基础进程的PID为0
	global.processEntryList->size = PAGE_SIZE;//刚好一页
	global.processEntryList->FirstPage = 0;//逻辑页面号0
	global.processEntryList->byte2malloc = 0;//段内分配从0开始
	global.processEntryList->nextProcess = NULL;

	global.memMMU[0] = (struct memPageRecord *)malloc(sizeof(struct memPageRecord));
	global.memMMU[0]->page_num = 0;
	global.memMMU[0]->phyAddrInDisk = 1;//不是磁盘中出来的都直接位置等于1，/PAGE_SIZE != 0
	time (&global.memMMU[0]->timeStamp);
	global.memMMU[0]->isModified = 0;
	global.memMMU[0]->isReadable = 1;

	global.memBuffer[0] = 1;//mem的物理页面0被使用

	global.linnerPageList = (struct linnerPageRecord *)malloc(sizeof(struct linnerPageRecord));
	global.linnerPageList->preLinnerPage = NULL;
	global.linnerPageList->nextLinnerPage = NULL;
	global.linnerPageList->firstLinnerPage_num = 1;//逻辑页面0已被baseProcess使用，下一段从逻辑页面1开始
	global.linnerPageList->size = (MEM_SIZE + DISK_SIZE) / PAGE_SIZE - 1;//空闲逻辑长度为多少个页面长度
}

void BitMapToBuffer()	//把文件中的位图拷入到真正的内存数组中，使读取速度增快
{
	FILE *fp;
	fp = fopen("diskBuffer.txt","rb");//只读打开一个二进制文件，只允许读数据
	int i = 0;
	if (fp != NULL)//从文件中读入上次硬盘的位图情况
	{
		for (i = 0; i < DISK_SIZE/PAGE_SIZE; i++)
		{
			global.diskBuffer[i] = fgetc(fp);
		}
		fclose(fp);
	}
	else
	{
		char *errorMsg = "BitMapToBuffer.txt open failed.";
		shutDownError(errorMsg);
	}
	
	for (i = 0; i < MEM_SIZE/PAGE_SIZE; i++)//内存位图全部清零
	{
		global.memBuffer[i] = 0;
	}
}

void BufferToBitMap()	//把数组中的数据拷入到文件中的位图中，使得下次打开有正确的页表位图
{
	FILE *fp;
	fp = fopen("diskBuffer.txt","wb");//只写打开或建立一个二进制文件，只允许写数据
	int i = 0;
	for (i = 0; i < DISK_SIZE/PAGE_SIZE; i++)
	{
		fputc(global.diskBuffer[i], fp);
	}
	fclose(fp);
}

int FindFreeBufferMem()					//查找内存中的第一个空闲页表位图
{
	//unsigned int *temp = (unsigned int *)global.memBuffer;
	int i = 0;
	for (i = 0; i < (MEM_SIZE/PAGE_SIZE); i++)
	{
		if (global.memBuffer[i] != 1)
		{
			return i;
		}
	}
	return -1;//无空闲，返回-1
}

int FindFreeBufferDisk()				//查找硬盘中的第一个空闲位图
{
	unsigned int *temp = (unsigned int *)global.diskBuffer;
	int i = 0;
	unsigned int mask = 0;
	mask = ~mask;
	for (i = 0; i*( sizeof(int) / sizeof(char) ) < (DISK_SIZE/PAGE_SIZE); i++)
	{
		
		if (temp[i] != mask)
		{
			int offset = 0;
			i = i * ( sizeof(int) / sizeof(char) );
			for (offset = 0; offset < ( sizeof(int) / sizeof(char) ); offset++)
			{
				if (global.diskBuffer[i + offset] == 0)
				{
					return i + offset;
				}
			}
		}
	}
	return -1;//无空闲，返回-1
}

int FindTotalFreeBufferMem()				//查找内存中所有空闲位图的数目总数
{
	unsigned int *temp = (unsigned int *)global.memBuffer;
	int i = 0;
	unsigned int mask = 0;
	mask = ~mask;
	unsigned int sum = 0;
	for (i = 0,sum = 0; i*sizeof(int) < (MEM_SIZE/PAGE_SIZE); i++)
	{
		if (temp[i] != mask)
		{
			int offset = 0;
			i = i * sizeof(int);
			for (offset = 0; offset < sizeof(int); offset++)
			{
				if (global.memBuffer[i + offset] == 0)
				{
					sum++;
				}
			}
		}
	}
	return sum;
}

void MaskBuffer(char *buffer, int numOfBit)				//置特定位图的值为1
{
	buffer[numOfBit] = 1;
}

void ClearBuffer(char *buffer, int numOfBit)			//置特定位图的值为0
{
	buffer[numOfBit] = 0;
}

/*TEST FUNCTION
int main()
{
	Initialize();
	printf("1\n");
	BitMapToBuffer();
	printf("2\n");
	MaskBuffer(global.memBuffer,0);
	printf("3\n");
	MaskBuffer(global.diskBuffer,0);
	printf("4\n");
	
	printf("5 %d\n",FindFreeBufferMem());
	
	printf("6 %d\n",FindFreeBufferDisk());
	FindTotalFreeBufferMem();
	printf("7\n");
	BufferToBitMap();

	return 0;
}
*/