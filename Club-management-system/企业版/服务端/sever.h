#include<iostream>
#include<Winsock2.h>
#include<string>
#ifndef _SEVER_H_
#define _SEVER_H_
using namespace std;
#pragma comment (lib,"ws2_32.lib")

	WSADATA wsaData;
	SOCKET sockServer;
	SOCKADDR_IN addrServer;
	SOCKET sockClient;
	SOCKADDR_IN addrClient;
	int len=sizeof(SOCKADDR);
	
void init_server(){
	WSAStartup(MAKEWORD(2,2),&wsaData);
	sockServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	addrServer.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//INADDR_ANY��ʾ�κ�IP
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(6000);//�󶨶˿�6000
	bind(sockServer,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
}

string get_str(){
	//Listen������
	listen(sockServer,5);//5Ϊ�ȴ�������Ŀ
	cout<<"������������:"<<endl<<"������..."<<endl;
	char recvBuf[100];
 
	//���������̣�ֱ���пͻ�����������Ϊֹ
	sockClient=accept(sockServer,(SOCKADDR*)&addrClient,&len);	
	recv(sockClient,recvBuf,100,0);
	
	return recvBuf;
}
#endif
