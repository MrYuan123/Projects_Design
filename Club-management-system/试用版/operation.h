#include<iostream>
#include<fstream>
#include<windows.h>
#include<stdlib.h> 
#include<string>
#include<cstring>
#include"club.h"
#include"people.h"
#include"player.h"
#include"show.h"
#include"stdafx.h"
#include"show.h"

#ifndef _OPERATION_H_
#define _OPERATION_H_
using namespace std;
void name_sortV(string str){
	int n=0;
	str.erase(str.begin(),str.begin()+6);
	for(int i=0;i<clubSum;i++){
		for(int m=0;m<cl[i].get_playerSum();m++){
			if(cl[i].get_player(m).get_name()==str){
			n=1;
			cout<<"Have a player named:"<<cl[i].get_player(m).get_name();
			cout<<" from club "<<cl[i].get_name()<<"!"<<endl;
			cout<<"Name:"<<cl[i].get_player(m).get_name()<<"  ";
			cout<<"Age:"<<cl[i].get_player(m).get_age()<<"  ";
			cout<<"Ability:"<<cl[i].get_player(m).get_ability()<<"  ";
			cout<<"Limit Age:"<<cl[i].get_player(m).get_limit()<<endl;
			cout<<"Goal sum:"<<cl[i].get_player(m).get_sum()<<"  ";
			cout<<"Transfer Fee:"<<cl[i].get_player(m).get_transfer()<<endl;	
			}
		}
	}
	
	if(n==0)
		cout<<"Don't have such a player!"<<endl;
}

void deal_space(string &s){
	int e=0;
	string::iterator it;
	
	while(s[e]!=NULL){
		if(s[e]==' '){
		it=s.begin()+e;
		s.erase(it);	
		}
		else{
			e++;
		}
	} 
}
void change(club s,int temp[4],int *sign){
	int sum=s.get_playerSum();
	int opposite=0,convert[s.get_playerSum()];
	
	if(temp[0]==1)
		opposite=1;
		
	for(int i=0;i<s.get_playerSum();i++){
		convert[i]=sign[i];
		sign[i]=1;
	}
		
	
	switch(temp[1]){
		case 1:{
			switch(temp[2]){
				case 1:{
					for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_age()<=temp[3])
							sign[i]=0;
					}
					break;
				}
				case 2:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_age()>=temp[3])
							sign[i]=0;
					}
					break;
				}
				case 3:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_age()!=temp[3])
							sign[i]=0;
							
					}
					break;
				}
				case 4:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_age()<temp[3])
							sign[i]=0;
					}
					break;
				}
				case 5:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_age()>temp[3])
							sign[i]=0;
					}
					break;
				}
			}
			break;
		}
		case 2:{
				switch(temp[2]){
				case 1:{
					for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_limit()<=temp[3])
							sign[i]=0;
					
					}
					break;
				}
				case 2:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_limit()>=temp[3])
							sign[i]=0;
							
					}
					break;
				}
				case 3:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_limit()!=temp[3])
							sign[i]=0;
					}
					break;
				}
				case 4:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_limit()<temp[3])
							sign[i]=0;
					}
					break;
				}
				case 5:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_limit()>temp[3])
							sign[i]=0;
					}
					break;
				}
			}
			break;
		}
		case 3:{
			switch(temp[2]){
				case 1:{
					for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_sum()<=temp[3])
							sign[i]=0;
							
					}
					break;
				}
				case 2:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_sum()>=temp[3])
							sign[i]=0;
							
					}
					break;
				}
				case 3:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_sum()!=temp[3])
							sign[i]=0;
					}
					break;
				}
				case 4:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_sum()<temp[3])
							sign[i]=0;
							
					}
					break;
				}
				case 5:{
						for(int i=0;i<s.get_playerSum();i++){
						if(s.get_player(i).get_sum()>temp[3])
							sign[i]=0;
				
					}
					break;
				}
			}
			break;
		}
		default:{
			cout<<"Wrong operation!"<<endl;
			exit(-1);
			break;
		}
	}
	
	//cout<<sign[0]<<" "<<sign[1]<<endl;
	
	if(opposite==1){
		for(int i=0;i<s.get_playerSum();i++)
		if(sign[i]==0)
			sign[i]=1;
		
		else
			sign[i]=0;
	} 
	
	//cout<<sign[0]<<" "<<sign[1]<<endl;
	for(int i=0;i<s.get_playerSum();i++)
	sign[i]=sign[i]*convert[i];
	
}
void sort_player(club s){
	
	int sign[s.get_playerSum()];
	for(int i=0;i<s.get_playerSum();i++){
		sign[i]=1;	
	}
	string str;
	char t;
	int m=0;
	
	cout<<"Please input the demand:"<<endl;
	fflush(stdin);
	getline(cin,str); 
	deal_space(str);
	
	string ms(str,0,6);
	
	if(ms=="@name~"){
		name_sortV(str);
		return;
	} 
	t=str[m];
	while(t!=NULL){
		if(str[m]=='&'&&str[m+1]=='&')
		m+=2;
		
		t=str[m];
		int temp[4]={0};
	    if(t=='!'){
	    	temp[0]=1;
	    	m+=2;
	    	t=str[m];
	   	if(t=='@')
	   	  m++;
	   	  
	   	  t=str[m];
		if(t!=')'){
			if(str[m]=='a'&&str[m+1]=='g'&&str[m+2]=='e'){
				temp[1]=1;
				switch(str[m+3]){
					case 62:{
						if(str[m+4]!=61){
							temp[2]=1;	
							m+=4;
						}
						if(str[m+4]==61){
							temp[2]=4;
							m+=5;	
						}
						break;
					}
					case 60:{
						if(str[m+4]!=61){
							temp[2]=2;
							m+=4;	
						}
						if(str[m+4]==61){
							temp[2]=5;
							m+=5;
						}
						
						break;
					}
					case 61:{
						temp[2]=3;
						m+=4;
						break;
					}
				}	
			int num=0;
			t=str[m];
			while(t!=')'){
				
				num=num*10+(int)t-48;
				cout<<num<<endl;
				t=str[++m];
			}	
			temp[3]=num;
			t=str[++m];	
			} 
			
		else if(str[m]=='s'&&str[m+1]=='u'&&str[m+2]=='m'){
			
			temp[1]=3;
			switch(str[m+3]){
					case 62:{
						if(str[m+4]!=61){
							temp[2]=1;	
							m+=4;
						}
						if(str[m+4]==61){
							temp[2]=4;
							m+=5;	
						}
						break;
					}
					case 60:{
						if(str[m+4]!=61){
							temp[2]=2;
							m+=4;	
						}
						if(str[m+4]==61){
							temp[2]=5;
							m+=5;
						}
						
						break;
					}
					case 61:{
						temp[2]=3;
						m+=4;
						break;
					}
				}	
			int num=0;
			t=str[m];
			while(t!=')'){
				
				num=num*10+(int)t-48;
				cout<<num<<endl;
				t=str[++m];
			}	
			temp[3]=num;
			t=str[++m];	
		}
		else if(str[m]=='l'&&str[m+1]=='i'&&str[m+2]=='m'){
			temp[1]=2;
			switch(str[m+3]){
					case 62:{
						if(str[m+4]!=61){
							temp[2]=1;	
							m+=4;
						}
						if(str[m+4]==61){
							temp[2]=4;
							m+=5;	
						}
						break;
					}
					case 60:{
						if(str[m+4]!=61){
							temp[2]=2;
							m+=4;	
						}
						if(str[m+4]==61){
							temp[2]=5;
							m+=5;
						}
						
						break;
					}
					case 61:{
						temp[2]=3;
						m+=4;
						break;
					}
				}	
			int num=0;
			t=str[m];
			while(t!=')'){
				
				num=num*10+(int)t-48;
				cout<<num<<endl;
				t=str[++m];
			}	
			temp[3]=num;
			t=str[++m];	
		}		
		} 
		}	
		
		
		else{
			temp[0]=0;
			if(str[m]=='@')
			m++;
			
			if(str[m]=='a'&&str[m+1]=='g'&&str[m+2]=='e'){
				temp[1]=1;
				switch(str[m+3]){
					case 62:{
						if(str[m+4]!=61){
							temp[2]=1;	
							m+=4;
						}
						if(str[m+4]==61){
							temp[2]=4;
							m+=5;	
						}
						break;
					}
					case 60:{
						if(str[m+4]!=61){
							temp[2]=2;
							m+=4;	
						}
						if(str[m+4]==61){
							temp[2]=5;
							m+=5;
						}
						
						break;
					}
					case 61:{
						temp[2]=3;
						m+=4;
						break;
					}
				}	
			int num=0;
			t=str[m];
			while(t!=NULL&&t!='&'){
				
				num=num*10+(int)t-48;
				cout<<num<<endl;
				t=str[++m];
			}	
			temp[3]=num;	
			} 
			
			else if(str[m]=='s'&&str[m+1]=='u'&&str[m+2]=='m'){
				temp[1]=3;
			switch(str[m+3]){
					case 62:{
						if(str[m+4]!=61){
							temp[2]=1;	
							m+=4;
						}
						if(str[m+4]==61){
							temp[2]=4;
							m+=5;	
						}
						break;
					}
					case 60:{
						if(str[m+4]!=61){
							temp[2]=2;
							m+=4;	
						}
						if(str[m+4]==61){
							temp[2]=5;
							m+=5;
						}
						
						break;
					}
					case 61:{
						temp[2]=3;
						m+=4;
						break;
					}
				}	
			int num=0;
			t=str[m];
			while(t!=NULL&&t!='&'){
				
				num=num*10+(int)t-48;
				cout<<num<<endl;
				t=str[++m];
			}	
			temp[3]=num;
			}
			
			else if(str[m]=='l'&&str[m+1]=='i'&&str[m+2]=='m'){
			temp[1]=2;
			switch(str[m+3]){
					case 62:{
						if(str[m+4]!=61){
							temp[2]=1;	
							m+=4;
						}
						if(str[m+4]==61){
							temp[2]=4;
							m+=5;	
						}
						break;
					}
					case 60:{
						if(str[m+4]!=61){
							temp[2]=2;
							m+=4;	
						}
						if(str[m+4]==61){
							temp[2]=5;
							m+=5;
						}
						
						break;
					}
					case 61:{
						temp[2]=3;
						m+=4;
						break;
					}
				}	
			int num=0;
			t=str[m];
			while(t!=NULL&&t!='&'){
				
				num=num*10+(int)t-48;
				cout<<num<<endl;
				t=str[++m];
			}	
			temp[3]=num;
		}
		}

	  change(s,temp,sign);
	  //cout<<temp[0]<<" "<<temp[1]<<" "<<temp[2]<<" "<<temp[3]<<endl; 
	}
	
	for(int i=0;i<s.get_playerSum();i++){
		if(sign[i]==1){
			cout<<"Name:"<<s.get_player(i).get_name()<<"   ";
			cout<<"Age:"<<s.get_player(i).get_age()<<"   ";
			cout<<"Ability:"<<s.get_player(i).get_ability()<<endl;
			cout<<"Transfer Fee:"<<s.get_player(i).get_transfer()<<"   ";
			cout<<"Goal sum:"<<s.get_player(i).get_sum()<<"   ";
			cout<<"Limit Age:"<<s.get_player(i).get_limit()<<endl;
			
		}
	}
	
}


#endif

