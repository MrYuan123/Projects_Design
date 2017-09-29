/*
从MACRO中读取disk memd的大小信息，初始化内存/硬盘模拟文件
 */
#include "MACRO.H"
#include "../error/systemError.c"
#include<stdio.h>

int main()
{
	FILE *fp;
	fp = fopen("MEM.txt","wb");//只写，新建。形式打开
	int i = 0;
	if (fp != NULL)
	{	
		for (i = 0; i < MEM_SIZE; i++)
		{
			fputc('\0',fp);
		}
		if (fclose(fp) == 0)//文件正常关闭
		{
			fp = fopen("DISK.txt","wb");
			if (fp != NULL)
			{
				for (i = 0; i < DISK_SIZE; i++)
				{
					fputc('\0',fp);					
				}
				if (fclose(fp) == 0)
				{
					fp = fopen("diskBuffer.txt","wb");
					if (fp != NULL)
					{
						for (i = 0; i < DISK_SIZE/PAGE_SIZE; i++)
						{
							fputc('\0',fp);	
						}
						return 1;//成功并返回1；
					}					
				}
			}
			else
			{
				char *msg = "TRY TO OPEN DISK.txt FAILED.";
				equipmentError(msg);
				return -1;	
			}
		}
	}
	else
	{
		char *msg = "TRY TO OPEN MEM.txt FAILED.";
		equipmentError(msg);
		return -1;
	}
}