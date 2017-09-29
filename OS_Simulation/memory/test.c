/*
内存模块测试程序
 */
#include "funcPart3.c"

typedef unsigned int ADDR; 

main()
{
	Initialize();
	int segmentAddr = 0;
	struct linnerPageRecord * tempPtr = global.linnerPageList;
	unsigned char testProcess1 = 1, testProcess2 = 2, testProcess3 = 3, testProcess4 = 4,testProcess5 = 5, testProcess6 = 6;
	//========================================测试段地址的分配回收
	{
		segmentAddr = CreateProcess(testProcess1,3*PAGE_SIZE+100);//结果应该为4096

		if (segmentAddr > 0){//段地址分配成功
			printf("testProcess1 segmentAddr %d(4096)\n",segmentAddr);
		}
		else{//段地址分配失败
			printf("testProcess1 CreateProcess failed return %d\n",segmentAddr);
		}
		segmentAddr = CreateProcess(testProcess2,4*PAGE_SIZE);//结果应该为4096+4096*4 = 20480
		if (segmentAddr > 0){//段地址分配成功
			printf("testProcess2 segmentAddr %d(20480)\n",segmentAddr);
		}
		else{//段地址分配失败
			printf("testProcess2 CreateProcess failed return %d\n",segmentAddr);
		}
		segmentAddr = CreateProcess(testProcess3,4*PAGE_SIZE);//结果应该为4096+4096*4+4096*4 = 36864
		if (segmentAddr > 0){//段地址分配成功
			printf("testProcess3 segmentAddr %d(36864)\n",segmentAddr);
		}
		else{//段地址分配失败
			printf("testProcess3 CreateProcess failed return %d\n",segmentAddr);
		}
		DelProcess(testProcess2);//从20480开始释放4页空间
		tempPtr = global.linnerPageList;
		while (tempPtr != NULL)
		{
			printf("%d %d\n",tempPtr->firstLinnerPage_num,tempPtr->size);
			tempPtr = tempPtr->nextLinnerPage;
		}

		segmentAddr = CreateProcess(testProcess4,2*PAGE_SIZE);//结果应该为4096+4096*4 = 20480，位于刚刚删除的process2段内
		if (segmentAddr > 0){//段地址分配成功
			printf("testProcess4 segmentAddr %d(20480)\n",segmentAddr);
		}
		else{//段地址分配失败
			printf("testProcess4 CreateProcess failed return %d\n",segmentAddr);
		}
		segmentAddr = CreateProcess(testProcess5,PAGE_SIZE);//结果应该为4096+4096*4+4096*2 = 28672，位于刚刚删除的process2段内
		if (segmentAddr > 0){//段地址分配成功
			printf("testProcess5 segmentAddr %d(28672)\n",segmentAddr);
		}
		else{//段地址分配失败
			printf("testProcess5 CreateProcess failed return %d\n",segmentAddr);
		}
		DelProcess(testProcess1);//删除第一个分配出去的段
		tempPtr = global.linnerPageList;
		while (tempPtr != NULL)
		{
			printf("%d %d\n",tempPtr->firstLinnerPage_num,tempPtr->size);
			tempPtr = tempPtr->nextLinnerPage;
		}
		DelProcess(testProcess3);//删除最后一个分配出去的段
		tempPtr = global.linnerPageList;
		while (tempPtr != NULL)
		{
			printf("%d %d\n",tempPtr->firstLinnerPage_num,tempPtr->size);
			tempPtr = tempPtr->nextLinnerPage;
		}

		segmentAddr = CreateProcess(testProcess6,PAGE_SIZE);//结果应该为4096
		if (segmentAddr > 0){//段地址分配成功
			printf("testProcess6 segmentAddr %d(4096)\n",segmentAddr);
		}
		else{//段地址分配失败
			printf("testProcess6 CreateProcess failed return %d\n",segmentAddr);
		}
		tempPtr = global.linnerPageList;
		while (tempPtr != NULL)
		{
			printf("%d %d\n",tempPtr->firstLinnerPage_num,tempPtr->size);
			tempPtr = tempPtr->nextLinnerPage;
		}		
		DelProcess(testProcess6);
		tempPtr = global.linnerPageList;
		while (tempPtr != NULL)
		{
			printf("%d %d\n",tempPtr->firstLinnerPage_num,tempPtr->size);
			tempPtr = tempPtr->nextLinnerPage;
		}
	}
	//=======================================================测试段内变量的地址分配，读写
	{
		segmentAddr = CreateProcess(testProcess1,PAGE_SIZE);
		if (segmentAddr > 0){//段地址分配成功
			printf("testProcess1 segmentAddr %d(4096)\n",segmentAddr);
			//字符
			ADDR _char = _malloc(testProcess1,sizeof(char)*1);
			printf("_char %x\n",_char);
			_write(_char,sizeof(char),1,"a");
			printf("after write a, read result: %c\n", *((char *)_read(_char,sizeof(char),1)));
			//字符串
			ADDR _chars = _malloc(testProcess1,sizeof(char)*10);
			_write(_char,sizeof(char),11,"helloWorld\0");
			printf("after write helloWorld, read result: %s\n", (char *)_read(_char,sizeof(char),11));
			//整数
			int input = -2017;
			ADDR _int = _malloc(testProcess1,sizeof(int)*1);
			_write(_int,sizeof(int),1,&input);
			printf("after write %d, read result: %d\n", input, *((int *)_read(_int,sizeof(int),1)));
			//数组
			int array[5] = {1,2,3,4,5};
			ADDR _array = _malloc(testProcess1,sizeof(int)*5);
			_write(_array,sizeof(int),5,array);
			int i = 0;
			for (i = 0; i < 5; i++)
			{
				printf("after write %d, read result: %d\n",array[i],((int *)_read(_array,sizeof(int),5))[i]);
			}
			
		}
		else{//段地址分配失败
			printf("testProcess1 CreateProcess failed return %d\n",segmentAddr);
		}
	}
}