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
#include"client.h"

using namespace std;

int main(void){
	init();
	
	while(1){
		surface();
		int nu;
		cin>>nu;
		
		if(nu==2){
			init_client();
			system("cls");
			ClientSur();
			int cop;
			cin>>cop;
			if(cop==1){     //进行球员搜索 
				char op[100];
				string con;
				send_Mes("sort"); 
				cout<<"Please input the condition:"<<endl;
				cin>>op;
				con=op;
				send_Mes(op);

				char s1[100],s2[100],s3[100],s4[100],s5[100],s6[100];
				
				cout<<"Server Message:"<<endl; 
				recv(sockClient,s1,100,0);
				while(s1[0]!='#'){
					recv(sockClient,s2,100,0);
					recv(sockClient,s3,100,0);
					recv(sockClient,s4,100,0);
					recv(sockClient,s5,100,0);
					recv(sockClient,s6,100,0);
					cout<<s1<<" "<<s2<<" "<<s3<<" "<<s4<<" "<<s5<<" "<<s6<<endl;
					recv(sockClient,s1,100,0);
				}
				
				cout<<"Client Message:"<<endl;
				for(int i=0;i<clubSum;i++){
					nowSign=cl[i].get_playerSum();
					for(int w=0;w<cl[i].get_playerSum();w++)
						finalSign[w]=1;
							
					final_sort(cl[i],finalSign,con,1);
					
					for(int m=0;m<cl[i].get_playerSum();m++){
						if(finalSign[m]==1){
							cout<<cl[i].get_player(m).get_name()<<" ";
							cout<<cl[i].get_player(m).get_age()<<" ";
							cout<<cl[i].get_player(m).get_ability()<<" ";
							cout<<cl[i].get_player(m).get_transfer()<<" ";
							cout<<cl[i].get_player(m).get_sum()<<" ";
							cout<<cl[i].get_player(m).get_limit()<<endl;
						}
					}
				}
				cout<<"complish sort operation!"<<endl;
			}
			else if(cop==2){    //进行球员转会 
				char op[100];
				send_Mes("charge"); 
				cout<<"Please input the player name:"<<endl;
				cin>>op;
				send_Mes(op);
				cout<<"have sent the charge player name!"<<endl; 
				
					char recvBuf[100];
					char na[30],ag[30],ab[30],trans[30],su[30],lim[30];
					
					recv(sockClient,na,30,0);
					recv(sockClient,ag,30,0);
					recv(sockClient,ab,30,0);
					recv(sockClient,trans,30,0);
					recv(sockClient,su,30,0);
					recv(sockClient,lim,30,0);
					cout<<na<<" "<<ag<<" "<<ab<<" "<<trans<<" "<<su<<" "<<lim<<endl; 
					
					string a(na);
					string b(ag);
					string c(ab);
					string d(trans);
					string e(su);
					string f(lim); 
					
					int ageN,sumN,transN,limN;
					ageN=atoi(b.c_str());
					sumN=atoi(e.c_str());
					transN=atoi(d.c_str());
					limN=atoi(f.c_str());
					  
					cl[0].add_player(a,ageN,c,transN,sumN,limN);
					cout<<"Have complished the charge operation!"<<endl; 
			}
			
			else{
				cout<<"This a wrong operation!"<<endl; 
			}
			
	}
		
		else{
		system("cls");
		Excutive();
	}	

		cout<<endl<<"Go on?(Y or N)"<<endl;
			char temp;
			cin>>temp;
		
			if(temp=='Y'||temp=='y')
			system("CLS");
		
			else
		  		return 0;
	} 
	return 0;
} 
