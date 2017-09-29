/*
集中处理所有的错误信息
 */
#include<stdio.h>
#include<time.h>

void write2Log(char *msg)
{
	FILE *log = fopen("log.txt","at");//追加打开一个文本文件，并在文件末尾写数据
	fputs(msg,log);
	fclose(log);
}
void shutDownError(char *msg)
{
	write2Log(msg);
}
void equipmentError(char *msg)
{
	write2Log(msg);
}