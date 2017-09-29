/*
进程内存分配
 */
#include "funcPart2.c"

int CreateProcess(unsigned char processName, int sizeInByte)		//创建一个进程
{
	struct processEntry *tempPtr = global.processEntryList;
	if (tempPtr != NULL)
	{
		while (tempPtr->nextProcess != NULL)
		{
			if (tempPtr->processName == processName)
			{
				return -1;//该进程名称已存在
			}
			tempPtr = tempPtr->nextProcess;
		}	
		if (tempPtr->processName == processName)
		{
			return -1;//该进程名称已存在
		}
	}

	int firstLinnerPage_num = _mallocSegment(sizeInByte);
	if (firstLinnerPage_num >= 0)
	{
		tempPtr->nextProcess = (struct processEntry *)malloc(sizeof(struct processEntry));
		tempPtr->nextProcess->processName = processName;
		tempPtr->nextProcess->size = sizeInByte;
		tempPtr->nextProcess->FirstPage = firstLinnerPage_num;//纪录该进程的虚拟段地址开始
		tempPtr->nextProcess->byte2malloc = 0;//该进程的段内地址从0开始分配
		tempPtr->nextProcess->nextProcess = NULL;

		return firstLinnerPage_num*PAGE_SIZE;//返回逻辑页地址
	}
	else//虚存地址不足
	{
		return -2;//虚存地址不足
	}
}

void DelProcess(unsigned char processName)						//注销进程
{
	printf("try to DelProcess %d\n", processName);
	unsigned int i = 0;
	struct processEntry *tempPtr = (struct processEntry *)malloc(sizeof(struct processEntry));
	tempPtr->nextProcess =  global.processEntryList;
	if (tempPtr != NULL)
	{
		while (tempPtr->nextProcess != NULL)
		{
			if (tempPtr->nextProcess->processName == processName)
			{
				int page_length = tempPtr->nextProcess->size / PAGE_SIZE + (tempPtr->nextProcess->size % PAGE_SIZE==0?0:1);	
				_freeSegment(tempPtr->nextProcess->FirstPage, page_length);//释放这段逻辑地址,MMU,BUFFER								
				
				struct processEntry *temp = tempPtr->nextProcess;				
				tempPtr->nextProcess = tempPtr->nextProcess->nextProcess;
				free(temp);//释放真实系统中的processEntry项
				break;
			}
			tempPtr = tempPtr->nextProcess;
		}
	}
	printf("Deled %d\n", processName);
}

void AllocFirstPage(FILE *men, int addr, int size, int *memBuffer, int *diskBuffer);	//分配进程的一级页表

void AllocSecondPage(FILE *mem, FILE *disk, int addr, int size, int MorD, int *memBuffer, int *diskBuffer);	//对指定的二级页表进行分配

void AllocPageFrame(FILE *mem, int addr);				//分配进程的页


void DelFirstPage(FILE *mem, int addr, int size, int *memBuffer, int *diskBuffer);	//删除进程的一级页表

void DelSecondPage(FILE *mem, FILE *disk, int addr, int size, int MorD, int *memBuffer, int *diskBuffer);	//删除进程的二级页表

int FindFreeProcessEntry(FILE *mem);					//查找空闲进程表项

int FindProcssEntry(FILE *mem, char *processName);		//查找特定名字的进程的进程表项首地址

char Read(char *processName, unsigned int virAddr);		//读进程

/*
main()
{
	Initialize();
	if (CreateProcess("testProcess",4*PAGE_SIZE+100) >= 0 )
	{
		unsigned int i = _malloc("testProcess",4);
		int writeIn = 128;
		_write("testProcess", i, sizeof(int), 1, &writeIn);
		int readOut = 2;
		readOut = *((int *)_read("testProcess", i, sizeof(int), 1));
		printf("read after wrote %d result :%d\n", writeIn,readOut);

		i = _malloc("testProcess", 5);
		char *strIn = "hello";
		_write("testProcess", i, sizeof(char), 5, strIn);
		char *strOut;
		strOut = (char *)_read("testProcess", i, sizeof(char), 5);
		printf("read after wrote %s result :%s\n", strIn, strOut);
	}

	fclose(global.mem);
	fclose(global.disk);
}
*/