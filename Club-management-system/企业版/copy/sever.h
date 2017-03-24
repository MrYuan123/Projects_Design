#include<iostream>
#include<Winsock2.h>
#include<string>
#ifndef _SEVER_H_
#define _SEVER_H_
using namespace std;
#pragma comment (lib,"ws2_32.lib")

class Sever{
	private:
		SOCKET m_socket;	
	public:
	   int init(char *address, int port); 
	   void run();
	   //void sendMsg(char *address,int len);
};

int  Sever::init(char *address, int port){
	int rlt=0;
	WORD request;
	WSADATA wsadata;
	int err;
	
	request=MAKEWORD(1,1);
	err=WSAStartup(request,&wsadata);
	
	if(err!=NO_ERROR){
		cout<<"Wsastartup failed with "<<err<<endl;
		rlt=1;
		return rlt;
	}
	
	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_socket==INVALID_SOCKET){
		cout<<"Socket failed with "<<WSAGetLastError();
		rlt=2;
		return rlt;
	}
	
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(address);
	int  iErrorMsg;
	
	iErrorMsg = bind(m_socket,(sockaddr*)&serverAddr,sizeof(serverAddr));
	if (iErrorMsg < 0)
	{
		//°ó¶¨Ê§°Ü
		printf("bind failed with error : %d\n",iErrorMsg);
		rlt = 3;
		return rlt;
	}
	return rlt;
}

void Sever::run(){
	listen(m_socket,5);
	
	sockaddr_in tcpAddr;
	int len = sizeof(sockaddr);
	SOCKET newSocket;
	char buf[1024];
	int rval=1;
	
	while(1){
		newSocket = accept(m_socket,(sockaddr*)&tcpAddr,&len);
		if(newSocket==INVALID_SOCKET)
			cout<<"This is a invalid socket!"<<endl;
			
		else{
			while(rval!=0){
				memset(buf,0,sizeof(buf));
				rval = recv(newSocket,buf,1024,0);
				
				if(rval==SOCKET_ERROR)
					cout<<"Get a wrong socket!"<<endl;
				
				if(rval==0)
					cout<<"Disconnection!"<<endl;
				
				else{
					cout<<buf<<endl;
				}
			}
		
			 closesocket(newSocket);
		}
	}
			closesocket(m_socket);
}

#endif
