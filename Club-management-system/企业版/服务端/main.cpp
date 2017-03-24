#include<iostream>
#include<stdlib.h>
#include<fstream>
#include"stdafx.h"
#include"operation.h"
#include"club.h"
#include"people.h"
#include"player.h"
#include"show.h"
#include<windows.h>
#include"stdlib.h"
#include"excutive.h"
#include"sever.h"


using namespace std;

int main(void){
	init();
	init_server();
	while(1){
		string now=get_str();
		if(now=="sort"){         //搜索模块 
			cout<<"Now --Doing sort operation"<<endl;
			char strl[100];
			recv(sockClient,strl,100,0);
			string str(strl);
			cout<<str<<endl;
			cout<<"Have got the condition!"<<endl;
			for(int i=0;i<clubSum;i++){
				sort_operating(cl[i],str);
				cout<<"complished the line"<<endl;
				
				for(int m=0;m<cl[i].get_playerSum();m++){
					if(finalSign[m]==1){
						const char *ms1=cl[i].get_player(m).get_name().data(); 
						char ms2[20];   // 年龄 
						itoa(cl[i].get_player(m).get_age(),ms2,10);
						const char *ms3=cl[i].get_player(m).get_ability().data();   //能力 
						char ms4[20];   //转会费 
						itoa(cl[i].get_player(m).get_transfer(),ms4,10);
						char ms5[20];    //总进球数 
						itoa(cl[i].get_player(m).get_sum(),ms5,10);
						char ms6[20];    //限制年龄
						itoa(cl[i].get_player(m).get_limit(),ms6,10); 
						cout<<ms1<<" "<<ms2<<" "<<ms3<<" "<<ms4<<" "<<ms5<<" "<<ms6<<endl;
						
						fflush(stdin); 
						send(sockClient,ms1,strlen(ms1)+1,0);
						Sleep(10);						 	
						send(sockClient,ms2,strlen(ms2)+1,0);	
						Sleep(10);
						send(sockClient,ms3,strlen(ms3)+1,0);	
						Sleep(10);
						send(sockClient,ms4,strlen(ms4)+1,0);	
						Sleep(10);
						send(sockClient,ms5,strlen(ms5)+1,0);	
						Sleep(10);
						send(sockClient,ms6,strlen(ms6)+1,0); 
						Sleep(10);
					}
				}
			}
		
		send(sockClient,"#",strlen("#")+1,0);
		cout<<"Have complished the sort operation!"<<endl;
		} 
		
		else if(now=="charge"){      //交易模块 
			cout<<"Now --Doing charge operation"<<endl;
			char str[100];
			recv(sockClient,str,100,0);
			cout<<str<<endl; 
			for(int i=0;i<clubSum;i++){
				for(int m=0;m<cl[i].get_playerSum();m++){
					if(str==cl[i].get_player(m).get_name()){
						const char *ms1=cl[i].get_player(m).get_name().data(); 
						char ms2[20];   // 年龄 
						itoa(cl[i].get_player(m).get_age(),ms2,10);
						const char *ms3=cl[i].get_player(m).get_ability().data();   //能力 
						char ms4[20];   //转会费 
						itoa(cl[i].get_player(m).get_transfer(),ms4,10);
						char ms5[20];    //总进球数 
						itoa(cl[i].get_player(m).get_sum(),ms5,10);
						char ms6[20];    //限制年龄
						itoa(cl[i].get_player(m).get_limit(),ms6,10); 
						cout<<ms1<<" "<<ms2<<" "<<ms3<<" "<<ms4<<" "<<ms5<<" "<<ms6<<endl;
													
						send(sockClient,ms1,strlen(ms1)+1,0);
						send(sockClient,ms2,strlen(ms2)+1,0);
						send(sockClient,ms3,strlen(ms3)+1,0);
						send(sockClient,ms4,strlen(ms4)+1,0);
						send(sockClient,ms5,strlen(ms5)+1,0);
						send(sockClient,ms6,strlen(ms6)+1,0);
						
						cl[i].del_player(m); 
						cout<<"Have charged the player:"<<str<<endl;
					}
				}
			}
			cout<<"Have complished the charge operation!"<<endl;
		}
		
		else{        //异常模块 
			cout<<"The client system convert a wrong operation!"<<endl;
		}
	} 
	system("pause"); 
	return 0;
} 
