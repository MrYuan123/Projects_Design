/*
重载运算符的封装
 */
#include "funcPart3.c"
#include <cstdio>
#include <iostream>
using namespace std;

class VARIABLE
{
public:	
	int addr;
	unsigned int sizeInByte;

	void operator =(int num)//注意和申请语句分开，这个只有赋值功能
	{
		_write(this->addr,sizeof(int), 1, &num);
	}
	int operator +(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		*temp = *temp + num;
		return *temp;
	}
	int operator -(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		*temp = *temp - num;
		return *temp;
	}
	int operator *(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		*temp = (*temp) * num;
		return *temp;
	}
	int operator /(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		*temp = *temp / num;
		return *temp;
	}
	int operator %(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		*temp = *temp % num;
		return *temp;
	}
	int operator ++(int)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		*temp = *temp + 1;
		_write(this->addr,sizeof(int), 1, temp);
		temp = (int *)_read(this->addr,sizeof(int), 1);
		return *temp;
	}
	void operator +=(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		*temp = *temp + num;
		_write(this->addr,sizeof(int), 1, temp);
		temp = (int *)_read(this->addr,sizeof(int), 1);
	}
	bool operator <(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		if (*temp < num)
		{
			return 1;
		}
		return 0;
	}
	bool operator >(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		if (*temp > num)
		{
			return 1;
		}
		return 0;
	}
	bool operator ==(int num)
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		if (*temp == num)
		{
			return 1;
		}
		return 0;
	}
};
int INT(VARIABLE &var, unsigned char processName)//返回段内地址，进程名称默认为main. -1内存已满，-2程序不存在
{
	var.addr = _malloc(processName, sizeof(int));
	var.sizeInByte = sizeof(int);
	return var.addr;
}
int CHAR(VARIABLE &var, unsigned char processName)
{
	var.addr = _malloc(processName, sizeof(char));
	var.sizeInByte = sizeof(char);
	return var.addr;
}
int DOUBLE(VARIABLE &var, unsigned char processName)
{
	var.addr = _malloc(processName, sizeof(double));
	var.sizeInByte = sizeof(double);
	return var.addr;
}

int main()
{
	Initialize();
	unsigned char pid = 1, pid2 = 2;
	CreateProcess(pid,PAGE_SIZE);
	CreateProcess(pid2,PAGE_SIZE);
	VARIABLE i;
	INT(i,pid);
/*	for (i = 0; i < 5; i++)
	{
		cout << "pass";
		if (i == 4) 
			cout << "yes" << endl;
	}	*/
	
	VARIABLE j;
	INT(j, pid2);  
	int values = 0;
	for (j = 0; j < 5; j++)
	{	
/*		values += 2;
		_write(i.addr,sizeof(int),1,&values);  */
		
		printf("%d\n", *((int *)_read(j.addr,sizeof(int),1)));
		
	}
}
