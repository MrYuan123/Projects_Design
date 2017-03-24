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
	addrServer.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//INADDR_ANY表示任何IP
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(6000);//绑定端口6000
	bind(sockServer,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
}

string get_str(){
	//Listen监听端
	listen(sockServer,5);//5为等待连接数目
	cout<<"服务器已启动:"<<endl<<"监听中..."<<endl;
	char recvBuf[100];
 
	//会阻塞进程，直到有客户端连接上来为止
	sockClient=accept(sockServer,(SOCKADDR*)&addrClient,&len);	
	recv(sockClient,recvBuf,100,0);
	
	return recvBuf;
}
#endif
