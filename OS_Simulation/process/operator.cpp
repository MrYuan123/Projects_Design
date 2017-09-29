/*
重载运算符的封装
 */
#include "funcPart3.c"

class _int//返回段内地址，进程名称默认为main. -1内存已满，-2程序不存在
{
public:
	int addr;

	_int(unsigned char processName)
	{
		this->addr = _malloc(processName, sizeof(int));
		if (this < 0)
		{
			printf("申请失败\n");
			this->~_int();
		}
	}
	_int(int addr)
	{
		this->addr = addr;
		if (this < 0)
		{
			printf("申请失败\n");
			this->~_int();
		}
	}
	~_int()
	{
		_free(this->addr);
	}
	bool setAddr(unsigned int addr)
	{
		this->addr = addr;
		return  1;
	}
	int operator +()
	{
		int *temp = (int *)_read(this->addr,sizeof(int), 1);
		return *temp;
	}
	_int * operator &()
	{
		//return this->addr;
		return this;
	}	
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
class _char
{
public:
	int addr;

	_char(unsigned char processName)
	{
		this->addr = _malloc(processName, sizeof(char));
		if (this < 0)
		{
			printf("申请失败\n");
			this->~_char();
		}
	}
	~_char()
	{
		_free(this->addr);
	}
	int operator *()
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		return *temp;
	}
	_char * operator &()
	{
		//return this->addr;
		return this;
	}
	void operator =(char character)//注意和申请语句分开，这个只有赋值功能
	{
		_write(this->addr,sizeof(char), 1, &character);
	}
	int operator +(int num)
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		*temp = *temp + num;
		return *temp;
	}
	int operator -(int num)
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		*temp = *temp - num;
		return *temp;
	}
	bool operator <(int num)
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		if (*temp < num)
		{
			return 1;
		}
		return 0;
	}
	bool operator >(int num)
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		if (*temp > num)
		{
			return 1;
		}
		return 0;
	}
	bool operator ==(int num)
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		if (*temp == num)
		{
			return 1;
		}
		return 0;
	}
	bool operator ==(char character)
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		if (*temp == character)
		{
			return 1;
		}
		return 0;
	}
	bool operator >(char character)
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		if (*temp > character)
		{
			return 1;
		}
		return 0;
	}
	bool operator <(char character)
	{
		char *temp = (char *)_read(this->addr,sizeof(char), 1);
		if (*temp < character)
		{
			return 1;
		}
		return 0;
	}
};
class int_ARRAY
{
public:
	int addr;
	unsigned int count;

	int_ARRAY(unsigned char processName, unsigned int count)
	{
		this->addr = _malloc(processName, sizeof(int)*count);
		this->count = count;
		if (this->addr < 0)
		{
			printf("申请失败\n");
			this->~int_ARRAY();
		}
	}
	~int_ARRAY()
	{
		_free(this->addr);
	}
	int * operator &()
	{
		int *temp = (int *)_read(this->addr,sizeof(int), this->count);
		return temp;
	}
	int operator [](unsigned int i)//仅仅能够读取，不能赋值
	{
		int *temp = (int *)_read(this->addr + i * sizeof(int), sizeof(int), 1);
		return *temp;
	}
	bool assignment(unsigned int i, int value)   // assign the value to the member with corner mark 
	{
		if (_write(this->addr + i * sizeof(int), sizeof(int), 1, &value))
		{
			int *temp = (int *)_read(this->addr + i * sizeof(int),sizeof(int), 1);
			return 1;
		}	
		else
		{
			return 0;
		}	
	}
};


int testOperator()
{
	Initialize();
	unsigned char pid = 1;
	CreateProcess(pid, PAGE_SIZE);

	_int i = _int(pid);  // init our _int using int(pid) 
	i = -100;   // assign directly
	printf("%d\n",+i);  // the + means read 
	i = 200;
	printf("%d\n",+i);
	for (i = 0; i < 5; i++)
	{
		printf("%d\n", +i);	
	}

	int_ARRAY a = int_ARRAY(pid, 5);
	int interger = 1001;
	for (i = 0; i < 5; i++)
	{
		a.assignment(+i, interger);
		printf("%d\n",a[+i]);
	}
}
/*
int main()
{
	Initialize();
	int pid = 1;
	CreateProcess(pid,PAGE_SIZE);
	VARIABLE i;
	int(i,pid);
	for (i = 0; i < 5; i++)
	{
		printf("done\n");
	}	
}
*/
