#include<iostream>
#include<string>
#ifndef _CLIENT_H_
#define _CLIENT_H_
using namespace std;

	WSADATA wsaData;
	SOCKET sockClient;//客户端Socket
	SOCKADDR_IN addrServer;//服务端地址
	
	
void init_client(){
	WSAStartup(MAKEWORD(2,2),&wsaData);
	sockClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//定义要连接的服务端地址
	addrServer.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");//目标IP(127.0.0.1是回送地址)
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(6000);//连接端口6000
	//连接到服务端
	connect(sockClient,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
} 

void send_Mes(char *mess){
	send(sockClient,mess,strlen(mess)+1,0);
}

#endif
