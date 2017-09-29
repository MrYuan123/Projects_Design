/*
内存虚拟内存实现
在本次试验中，虚拟地址包含段地址，但不处理段项。直接程序首地址等于段地址
逻辑地址=段地址+段内偏移地址。unsigned int
线性偏移地址=程序起始地址+段内偏移地址。int
物理地址=页地址+页内偏移地址

页表统一编号，页表从内存开始编号一直到磁盘。
 */
#include "funcPart1.c"

int _write(unsigned int virAddr, unsigned int size, unsigned int count, void *buffer)//写入字节
{
	int i = 0;
	unsigned char processName = (virAddr & 0xff000000)>>24;
	int linnerAddr = VirAddr2LinnerAddr(virAddr);//获取线性地址,在非分页结构下，等同于物理地址
	if (linnerAddr < 0)
		return -1;
	int length = size * count;
	int linnerPage = linnerAddr / PAGE_SIZE;
	if ((linnerAddr + length) / PAGE_SIZE > linnerPage)//跨页
	{
		int result_offset = 0;//写入数据偏移量
		int left_count = count;
		count = ((linnerPage + 1) * PAGE_SIZE - linnerAddr) / size;
		left_count -= count;
		int tempLinnerAddr = linnerAddr;

		void *tempBuffer;
		for (linnerPage, tempLinnerAddr = linnerAddr; left_count > 0 && linnerPage <= (linnerAddr + length) / PAGE_SIZE; )
		{			
			int phyAddr = GetPhyAddr(processName, linnerAddr);//获取物理地址			
			if (phyAddr >= 0)
			{
				unsigned int page_num = phyAddr / PAGE_SIZE;			
				for (i = 0; i < MEM_SIZE/PAGE_SIZE ; i++)
				{
					if (global.memMMU[i] != NULL && global.memMMU[i]->page_num == page_num)//找到内存中该逻辑页对应的物理页
					{
						void *Buffer;
						switch (size)
						{
							case sizeof(char):
								Buffer = &((char *)buffer)[result_offset];
							case sizeof(int):
								Buffer = &((int *)buffer)[result_offset];
							case sizeof(double):
								Buffer = &((double *)buffer)[result_offset];
							default:
								Buffer = &((char *)buffer)[result_offset];
						}
						fseek(global.mem, phyAddr, 0);							
						fwrite(Buffer, size, count, global.mem);		
						result_offset += count;				
						break;											
					}				
				}						
			}
			linnerPage++;
			tempLinnerAddr = linnerPage * PAGE_SIZE;
			if (left_count >= PAGE_SIZE / size )//剩余超过一页
			{
				count = PAGE_SIZE / size;				
			}
			else
			{
				count = left_count;
			}
			left_count -= count;
		}
	}
	else
	{
		int phyAddr = GetPhyAddr(processName, linnerAddr);//获取物理地址			
		if (phyAddr >= 0)
		{
			unsigned int page_num = phyAddr / PAGE_SIZE;			
			for (i = 0; i < MEM_SIZE/PAGE_SIZE ; i++)
			{
				if (global.memMMU[i] != NULL && global.memMMU[i]->page_num == page_num)//找到内存中该逻辑页对应的物理页
				{
					fseek(global.mem, phyAddr, 0);							
					fwrite(buffer, size, count, global.mem);
					break;											
				}				
			}						
		}
	}
	return 1;
}

void *_read(unsigned int virAddr, unsigned int size, unsigned int count)//读出字节
{
	void *buffer = (void *)malloc(size*count);
	int i = 0;
	unsigned char processName = (virAddr & 0xff000000)>>24;//进程唯一识别符
	int linnerAddr = VirAddr2LinnerAddr(virAddr);//获取线性地址,在非分页结构下，等同于物理地址
	int length = size * count;
	int linnerPage = linnerAddr / PAGE_SIZE;
	if ((linnerAddr + length) / PAGE_SIZE > linnerPage)//跨页
	{
		int left_count = count;
		unsigned int result_offset = 0;
		count = ((linnerPage + 1) * PAGE_SIZE - linnerAddr) / size;
		left_count -= count;
		int tempLinnerAddr = linnerAddr;
		for (linnerPage, tempLinnerAddr = linnerAddr; left_count > 0 && linnerPage <= (linnerAddr + length) / PAGE_SIZE; )
		{
			int phyAddr = GetPhyAddr(processName, linnerAddr);//获取物理地址			
			if (phyAddr >= 0)
			{
				unsigned int page_num = phyAddr / PAGE_SIZE;			
				for (i = 0; i < MEM_SIZE/PAGE_SIZE ; i++)
				{
					if (global.memMMU[i] != NULL && global.memMMU[i]->page_num == page_num)//找到内存中该逻辑页对应的物理页
					{
						void *Buffer;
						switch (size)
						{
							case sizeof(char):
								Buffer = &((char *)buffer)[result_offset];
							case sizeof(int):
								Buffer = &((int *)buffer)[result_offset];
							case sizeof(double):
								Buffer = &((double *)buffer)[result_offset];
							default:
								Buffer = &((char *)buffer)[result_offset];
						}						
						fseek(global.mem, phyAddr, 0);							
						fread(Buffer, size, count, global.mem);		
						result_offset += count;				
						break;											
					}				
				}						
			}
			linnerPage++;
			tempLinnerAddr = linnerPage * PAGE_SIZE;
			if (left_count >= PAGE_SIZE / size )//剩余超过一页
			{
				count = PAGE_SIZE / size;				
			}
			else
			{
				count = left_count;
			}
			left_count -= count;
		}
	}
	else
	{
		int phyAddr = GetPhyAddr(processName, linnerAddr);//获取物理地址	
		if (phyAddr >= 0)
		{
			unsigned int page_num = phyAddr / PAGE_SIZE;
			for (i = 0; i < MEM_SIZE/PAGE_SIZE ; i++)
			{
				if (global.memMMU[i] != NULL && global.memMMU[i]->page_num == page_num)//找到内存中该逻辑页对应的物理页
				{
					fseek(global.mem, phyAddr, 0);						
					fread(buffer, size, count, global.mem);	
					break;											
				}				
			}						
		}
	}
	return buffer;
}

unsigned int _mallocSegment(int sizeInByte)//申请一段地址，返回逻辑段首页逻辑地址
{
	//首次适应逻辑页面分配
	struct linnerPageRecord *tempPtr = global.linnerPageList;
	int sizeInpage = sizeInByte / PAGE_SIZE + (sizeInByte % PAGE_SIZE==0 ? 0 : 1); 
	while (tempPtr != NULL)
	{
		if (tempPtr->size >= sizeInpage)
		{
			unsigned int mallocFirstPage = tempPtr->firstLinnerPage_num;
			tempPtr->size -= sizeInpage;
			tempPtr->firstLinnerPage_num += sizeInpage;
			if (tempPtr->size == 0)//如果这个空闲段刚好被分配完，需要被删除
			{
				if (tempPtr->preLinnerPage != NULL)
				{
					tempPtr->preLinnerPage->nextLinnerPage = tempPtr->nextLinnerPage;
				}				
				if (tempPtr->nextLinnerPage != NULL)
				{
					tempPtr->nextLinnerPage->preLinnerPage = tempPtr->preLinnerPage;	
				}
				if (tempPtr == global.linnerPageList)
				//如果这个就正好是global.linnerList,需要保证global.linnerList一直指向空闲链表的头部
				{
					global.linnerPageList = tempPtr->nextLinnerPage;					
				}
				free (tempPtr);
			}
			int linnerPage = 0;
			for (linnerPage = mallocFirstPage; linnerPage < mallocFirstPage + sizeInpage; linnerPage++)
			{
				int pageInMem = FindFreeBufferMem();
				if (pageInMem >= 0)
				{
					global.memBuffer[pageInMem] = 1;//该物理页置为已被使用

					global.memMMU[pageInMem] = (struct memPageRecord *)malloc(sizeof(struct memPageRecord));//纪录该物理页使用情况
					global.memMMU[pageInMem]->page_num = linnerPage;
					global.memMMU[pageInMem]->phyAddrInDisk = 1;
					time (&global.memMMU[pageInMem]->timeStamp);
					global.memMMU[pageInMem]->isModified = 0;
					global.memMMU[pageInMem]->isReadable = 1;				
				}
				else
				{
					;//当内存不够分配时，将一部分分配到磁盘中
				}
			}			
			return mallocFirstPage;
		}
		tempPtr = tempPtr->nextLinnerPage;
	}
	return -1;
}
unsigned int _freeSegment(unsigned int firstLinnerPage_num, int sizeInpage)//释放逻辑地址中的一段地址
{
	printf("firstLinnerPage_num: %d sizeInpage: %d\n",firstLinnerPage_num,sizeInpage);
	//释放该逻辑段中在内存中的页
	int i = 0, j = 0;
	for (i = firstLinnerPage_num; i < firstLinnerPage_num + sizeInpage; i++)
	{
		for (j = 0; j < MEM_SIZE / PAGE_SIZE; j++)
		{
			if (global.memMMU[j] != NULL && global.memMMU[j]->page_num == i)
			{
				global.memMMU[j] = NULL;
				global.memBuffer[j] = 0;//该物理页可用
				//free(global.memMMU[j]);//释放使用纪录
				
				break;
			}
		}
	}
	//释放逻辑段地址
	if (global.linnerPageList != NULL)//还有空闲区
	{
		if (firstLinnerPage_num >= global.linnerPageList->firstLinnerPage_num)//释放区在global.linnerPageList结点之后
		{
			struct linnerPageRecord *tempPtr = global.linnerPageList;
			while (tempPtr != NULL)
			{
				if (tempPtr->firstLinnerPage_num + tempPtr->size >= firstLinnerPage_num)
				{
					if (firstLinnerPage_num == tempPtr->firstLinnerPage_num + tempPtr->size)//和前面一块接壤
					{
						tempPtr->size += sizeInpage; 
						//与前后都接壤
						if (tempPtr->nextLinnerPage != NULL && ((tempPtr->firstLinnerPage_num + tempPtr->size) == tempPtr->nextLinnerPage->firstLinnerPage_num))
						{
							tempPtr->size += tempPtr->nextLinnerPage->size;
							tempPtr->nextLinnerPage = tempPtr->nextLinnerPage->nextLinnerPage;
							if (tempPtr->nextLinnerPage != NULL)
							{
								tempPtr->nextLinnerPage->preLinnerPage = tempPtr;
							} 
						}	
								
					}
					//仅和后面一块接壤
					else if (tempPtr->nextLinnerPage != NULL && firstLinnerPage_num + sizeInpage == tempPtr->nextLinnerPage->firstLinnerPage_num)
					{
						tempPtr->nextLinnerPage->firstLinnerPage_num -= sizeInpage;
						tempPtr->size += sizeInpage;
					}
					else//w位于前后两块之间，不接壤
					{
						struct linnerPageRecord *newPtr = (struct linnerPageRecord *)malloc(sizeof(struct linnerPageRecord));
						newPtr->firstLinnerPage_num = firstLinnerPage_num;
						newPtr->size = sizeInpage;
						newPtr->preLinnerPage = tempPtr;
						newPtr->nextLinnerPage= tempPtr->nextLinnerPage;

						tempPtr->nextLinnerPage = newPtr;
						if (newPtr->nextLinnerPage != NULL)
						{
							newPtr->nextLinnerPage->preLinnerPage = newPtr;
						}
					}
					return 1;
				}
				tempPtr = tempPtr->nextLinnerPage;
			}
		}
	}
	else//之前已完全没有空闲区，global.linnerPageList == NULL
	{
		global.linnerPageList = (struct linnerPageRecord *)malloc(sizeof(struct linnerPageRecord));
		global.linnerPageList->firstLinnerPage_num = firstLinnerPage_num;
		global.linnerPageList->size = sizeInpage;
		global.linnerPageList->preLinnerPage = NULL;
		global.linnerPageList->nextLinnerPage = NULL;
	}
	//存在global.linnerPageList指向的空闲区，但该空闲区在释放区后面，global.linnerPageList需要修改
	struct linnerPageRecord *newPtr = (struct linnerPageRecord *)malloc(sizeof(struct linnerPageRecord));
	newPtr->firstLinnerPage_num = firstLinnerPage_num;
	newPtr->size = sizeInpage;
	newPtr->preLinnerPage = NULL;
	newPtr->nextLinnerPage= global.linnerPageList;
	if (newPtr->nextLinnerPage != NULL)
	{		
		//和后面接壤
		if (newPtr->nextLinnerPage->firstLinnerPage_num == firstLinnerPage_num + sizeInpage)
		{			
			newPtr->size += newPtr->nextLinnerPage->size;
			newPtr->nextLinnerPage = newPtr->nextLinnerPage->nextLinnerPage;
			if (newPtr->nextLinnerPage->nextLinnerPage != NULL)
			{
				newPtr->nextLinnerPage->nextLinnerPage->preLinnerPage = newPtr;	
			}
		}
	}
	global.linnerPageList = newPtr;//该空闲区为新的最前空闲区
	return 1;
}

int _malloc(unsigned char processName, int sizeInByte)//一个程序段内的分配没有回收。
{
	int i = 0;
	int linnerAddr = 0;
	struct processEntry *tempPtr = global.processEntryList;
	while (tempPtr != NULL)
	{
		if (tempPtr->processName == processName)
		{
			for ( i = tempPtr->FirstPage * PAGE_SIZE; i< tempPtr->FirstPage * PAGE_SIZE + tempPtr->size; i++)
			{
				printf("1\n");
				if (tempPtr->byte2malloc + sizeInByte <= tempPtr->size)//段内偏移地址
				{
					tempPtr->byte2malloc += sizeInByte;
					int segment_offset = tempPtr->byte2malloc - sizeInByte;//返回段内偏移地址
					if (segment_offset <= 0x00ffffff)
						return processName<<24 + segment_offset;//8bit进程标识符+24bit段内偏移
					else
					{
						return -3;//超过段最大长度
					}
				}
			}
			return -1;//内存已满
		}
		tempPtr = tempPtr->nextProcess;
	}
	return -2;//查无此程序
}
unsigned int _free(unsigned int virAddr)
{
	return 1;//段内释放不写
}
int VirAddr2LinnerAddr(unsigned int virAddr)			//虚地址和实地址的转换
{
	unsigned char processName = (virAddr & 0xff000000)>>24;//获取前8位段项,这里的段项和processEntry有啥关系不？

	unsigned int segment_offset = virAddr & 0x00ffffff;//获取末24位段内偏移地址，4M

	int i = 0;
	int linnerAddr = 0;
	struct processEntry *tempPtr = global.processEntryList;
	while (tempPtr != NULL)
	{
		if (tempPtr->processName == processName)
		{
			if (segment_offset <= tempPtr->size)
			{
				linnerAddr = tempPtr->FirstPage * PAGE_SIZE + segment_offset;//段地址*页大小 + 段内偏移地址 = 线性地址
				return linnerAddr;
			}
			else
			{
				return -1;//虚拟地址给出来的超出了内存分配给他的地址
			}
		}
		tempPtr = tempPtr->nextProcess;
	}
	return -2;//该进程不存在于进程列表中
}

int GetPhyAddr(unsigned char processName, int linnerAddr)		//由线性地址计算得到内存或磁盘中的物理地址
{
	int i = 0;
	unsigned int page_num = linnerAddr / PAGE_SIZE;//逻辑页
	unsigned int page_offset = linnerAddr % PAGE_SIZE;
	for (i = 0; i < MEM_SIZE/PAGE_SIZE && global.memMMU[i] != NULL; i++)
	{
		if (global.memMMU[i]->page_num == page_num)//该逻辑页在内存中
		{
			return i*PAGE_SIZE + page_offset;//返回该物理页（内存中）地址+页内偏移
		}
	}
	//该逻辑页在磁盘中
	unsigned int swapOutPage = PageSwapStratgy(processName);
	PageSwap(page_num, swapOutPage);//将该逻辑页换到内存中
	return swapOutPage*PAGE_SIZE + page_offset;
}
int PageSwap(unsigned int pageInDisk, unsigned int pageInMem)			//页的换入/换出
{
	printf("inDisk:%d inMem:%d\n",pageInDisk,pageInMem);
	char temp;
	int i = 0;
	if (global.memMMU[pageInMem] == NULL)//相当于直接从磁盘读入内存
	{
		global.memMMU[pageInMem] = (struct memPageRecord *)malloc(sizeof(struct memPageRecord));
		global.memMMU[pageInMem]->isModified = 0;//剩下的初始化在后面完成
		global.memBuffer[pageInMem] = 1;//该内存物理页面被使用
	}
	if (global.memMMU[pageInMem]->isModified)//当存在改动的时候需要将其写回(新分配的时候需要将其都置为0)
	{
		fseek(global.disk, global.memMMU[pageInMem]->phyAddrInDisk, 0);
		fseek(global.mem,  pageInMem * PAGE_SIZE, 0);
		
		for (i = 0; i < PAGE_SIZE; i++)
		{
			temp = fgetc(global.mem);
			fputc(temp, global.disk);
		}
	}

	fseek(global.disk, pageInDisk * PAGE_SIZE - MEM_SIZE, 0);
	//逻辑页在磁盘中的物理地址是pageInDisk * PAGE_SIZE - MEM_SIZE
	fseek(global.mem,  pageInMem * PAGE_SIZE, 0);
	for (i = 0; i < PAGE_SIZE; i++)
	{
		temp = fgetc(global.disk);
		fputc(temp, global.mem);
	}
	global.memMMU[pageInMem]->page_num = pageInDisk;
	time( &global.memMMU[pageInMem]->timeStamp );//纪录换入时间
	global.memMMU[pageInMem]->isModified = 0;
	global.memMMU[pageInMem]->isReadable = 1;
	global.memMMU[pageInMem]->phyAddrInDisk = pageInDisk * PAGE_SIZE - MEM_SIZE;

	return 1;
}

unsigned int PageSwapStratgy(unsigned char processName)//得到该段中可以换出的页表下标，FIFO
{
	int i = 0;
	int freePage = GetFreePageAddr();
	if (freePage != -1)
	{
		return freePage;//内存中第一个空页表
	}
	else
	{
		struct processEntry *tempPtr = global.processEntryList;
		while (tempPtr != NULL)
		{
			if (tempPtr->processName == processName)
			{
				int page_count = tempPtr->size/PAGE_SIZE;
				if (tempPtr->size % PAGE_SIZE != 0)
				{
					page_count += 1;
				}
				int j = 0;
				for (i = 0, j = 0; i < page_count; i++)
				{					
					unsigned int fisrtInPage = 0;
					time_t tempTimeStamp = 0xffffffff;
					for (j = 0; j < MEM_SIZE/PAGE_SIZE; j++)
					{
						if (global.memMMU[j] != NULL && global.memMMU[j]->page_num == tempPtr->FirstPage + i)
						{
							if (global.memMMU[j]->timeStamp < tempTimeStamp)
								fisrtInPage = j;
						}
					}
				}
				return j;//该程序段中最早进来的一页
			}
			tempPtr = tempPtr->nextProcess;
		}
	}
}

int GetFreePageAddr()									//在内存未满的情况下发生缺页异常，返回内存中的空闲页地址
{
	return  FindFreeBufferMem();
}



/*TEST 
main()
{
	Initialize();
	printf("%d\n",_write("baseProcess",0x00000110,0x55));
	printf("%x\n",_read("baseProcess",0x00000110) );
}
*/