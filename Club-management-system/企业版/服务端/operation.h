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

int finalSign[40];
int nowSign=0;
using namespace std;
void name_sortV(string str){
	int n=0;
	str.erase(str.begin(),str.begin()+6);
	for(int i=0;i<clubSum;i++){
		for(int m=0;m<cl[i].get_playerSum();m++){
			if(!cl[i].get_player(m).get_name().find(str)){
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

void final_sort(club s,int *preSign,string str,int kind){
	int sign[s.get_playerSum()];
	for(int i=0;i<s.get_playerSum();i++){
		sign[i]=1;	
	}
	
	char t;
	int m=0;
	
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
				//cout<<num<<endl;
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
				//cout<<num<<endl;
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
				//cout<<num<<endl;
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
				//cout<<num<<endl;
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
				//cout<<num<<endl;
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
				//cout<<num<<endl;
				t=str[++m];
			}	
			temp[3]=num;
		}
		}

	  change(s,temp,sign);
	  //cout<<temp[0]<<" "<<temp[1]<<" "<<temp[2]<<" "<<temp[3]<<endl; 
	}
	
	if(kind==1){
		for(int lc=0;lc<s.get_playerSum();lc++){
			preSign[lc]=preSign[lc]*sign[lc];
		}
	}
	
	else if(kind==2){
		for(int lc=0;lc<s.get_playerSum();lc++){
			if((preSign[lc]+sign[lc])!=0){
				preSign[lc]=1;
			}
		}
	}
}
void sort_alg(club s,int *preSign,string str,int kind){
	int sign[s.get_playerSum()];
	for(int i=0;i<s.get_playerSum();i++)
		sign[i]=1;	
	
	
	int m=0;
	char t=str[m];
	
	while(str[m]!=NULL){ 
		if(str[m]=='('){
		int leftB=1,rightB=0,nowB=1;
		t=str[nowB];
		while(leftB!=rightB){
			if(t=='(')
				leftB++;
			else if(t==')')
				rightB++;
				
			nowB++;
			t=str[nowB];
		}
			m=nowB;
			
			fflush(stdin);
			//cout<<str.substr(1,nowB-2)<<endl;
			sort_alg(s,preSign,str.substr(1,nowB-2),kind);
			//cout<<"Part one ok!"<<endl;
	}

  		else if(str[m]=='&'){
  			m+=2;
  			fflush(stdin);
  			sort_alg(s,preSign,str.substr(m,*str.end()-2),1);
  			m=str.size();
  			t=str[m];
  			
		  }
		  
		else if(str[m]=='|'){
			m+=2;
			fflush(stdin);
			//cout<<str.substr(m,str.size())<<endl;
			sort_alg(s,preSign,str.substr(m,str.size()),2);
			//cout<<"Part two ok!"<<endl;
			m=str.size();
			t=str[m];
		}
  		
  		else{
  			int low=m;
  			int high=m;
  			while(str[high]!=NULL){
  				if(str[high]!='|')
  				   high++;
  				
  				else{
  					sort_alg(s,preSign,str.substr(low,high),kind);
  					//cout<<str.substr(high,str.size())<<endl;
  					sort_alg(s,preSign,str.substr(high,str.size()),kind);
  					high=str.size();
  					m=str.size();
				  }
			  }
			  
			  if(m!=str.size()){
			  	fflush(stdin);
			    final_sort(s,preSign,str,kind);
			    m=str.size(); 
			}
		}
  } 
}

void sort_operating(club s,string str){
	nowSign=s.get_playerSum();
	for(int i=0;i<s.get_playerSum();i++){
		finalSign[i]=1;	
	}
	
		sort_alg(s,finalSign,str,1);	
} 

#endif

