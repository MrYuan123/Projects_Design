#include<iostream>
#include<string>
#ifndef _CLIENT_H_
#define _CLIENT_H_
using namespace std;

	WSADATA wsaData;
	SOCKET sockClient;//�ͻ���Socket
	SOCKADDR_IN addrServer;//����˵�ַ
	
	
void init_client(){
	WSAStartup(MAKEWORD(2,2),&wsaData);
	sockClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//����Ҫ���ӵķ���˵�ַ
	addrServer.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");//Ŀ��IP(127.0.0.1�ǻ��͵�ַ)
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(6000);//���Ӷ˿�6000
	//���ӵ������
	connect(sockClient,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
} 

void send_Mes(char *mess){
	send(sockClient,mess,strlen(mess)+1,0);
}

#endif
