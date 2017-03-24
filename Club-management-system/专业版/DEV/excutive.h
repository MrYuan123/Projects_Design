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

#ifndef _EXCUTIVE_H_
#define _EXCUTIVE_H_

void Excutive(){
	int clubOp;
	surface1();
	cin>>clubOp;
	
	switch(clubOp){
		case 1:{
			system("CLS");
			surface2();
			string temp;
			cout<<"Please input the club name you want to operation:"<<endl;
			cin>>temp;
			
			int op;
			for(int m=0;m<clubSum;m++){
				if(cl[m].get_name()==temp){
				cout<<"Please input the operation:"<<endl;
				cin>>op;
				switch(op){
					case 1:{
						cout<<"Please input the message of the player:";
						cout<<"(name age  ability tranfer_fee goal_sum limit_age)"<<endl;
						string na,ab;
						int ag,trans,su,lim;
						cin>>na>>ag>>ab>>trans>>su>>lim;
						cl[m].add_player(na,ag,ab,trans,su,lim);
						
						if(m==0){
							ofstream file;
							file.open("player.txt",ios::out);
							for(int ad=0;ad<cl[m].get_playerSum();ad++){
								file<<endl<<cl[m].get_player(ad).get_name()<<" ";
								file<<cl[m].get_player(ad).get_age()<<" ";
								file<<cl[m].get_player(ad).get_ability()<<" ";
								file<<cl[m].get_player(ad).get_transfer()<<" ";
								file<<cl[m].get_player(ad).get_sum()<<" ";
								file<<cl[m].get_player(ad).get_limit();
							}
							file.close();
						}
						
						else if(m==1){
							ofstream file;
							file.open("player2.txt",ios::out);
							for(int ad=0;ad<cl[m].get_playerSum();ad++){
								file<<endl<<cl[m].get_player(ad).get_name()<<" ";
								file<<cl[m].get_player(ad).get_age()<<" ";
								file<<cl[m].get_player(ad).get_ability()<<" ";
								file<<cl[m].get_player(ad).get_transfer()<<" ";
								file<<cl[m].get_player(ad).get_sum()<<" ";
								file<<cl[m].get_player(ad).get_limit();
							}
							file.close();
						}
						break;
					}
					case 2:{
						cout<<"Please input the player name you want to delete:"<<endl;
						 string pn;
						 cin>>pn;
						 if(cl[m].sort_player(pn)==-1){
						 	cout<<"Don't have such player!"<<endl;
						 }
						 else{
						 	cl[m].del_player(cl[m].sort_player(pn));
						 }
						 
						 if(m==0){
							ofstream file;
							file.open("player.txt",ios::out);
							for(int ad=0;ad<cl[m].get_playerSum();ad++){
								file<<endl<<endl<<cl[m].get_player(ad).get_name()<<" ";
								file<<cl[m].get_player(ad).get_age()<<" ";
								file<<cl[m].get_player(ad).get_ability()<<" ";
								file<<cl[m].get_player(ad).get_transfer()<<" ";
								file<<cl[m].get_player(ad).get_sum()<<" ";
								file<<cl[m].get_player(ad).get_limit();
							}
							file.close();
						}
						
						else if(m==1){
							ofstream file;
							file.open("player2.txt",ios::out);
							for(int ad=0;ad<cl[m].get_playerSum();ad++){
								file<<endl<<cl[m].get_player(ad).get_name()<<" ";
								file<<cl[m].get_player(ad).get_age()<<" ";
								file<<cl[m].get_player(ad).get_ability()<<" ";
								file<<cl[m].get_player(ad).get_transfer()<<" ";
								file<<cl[m].get_player(ad).get_sum()<<" ";
								file<<cl[m].get_player(ad).get_limit();
							}
							file.close();
						}
						break;
					}
					case 3:{
						sort_operating(cl[m]);
						break;
					}
					case 4:{
						cout<<"Please input the player you want to get:"<<endl;
						string pn;
						cin>>pn;
						int temp1;
						for(int a=0;a<clubSum;a++){
							for(int b=0;b<cl[a].get_playerSum();b++){
								if(cl[a].get_player(b).get_name()==pn&&a!=m){
									if(cl[m].get_cash()>cl[a].get_player(b).get_transfer()){
									cl[m].copy_player(cl[a].get_player(b));
									cl[m].change_cash(cl[m].get_cash()-cl[a].get_player(b).get_transfer());
									cl[a].del_player(cl[a].sort_player(pn));
									temp1=b;
									cout<<"Club "<<cl[m].get_name()<<" had got player ";
									cout<<pn<<" from club "<<cl[a].get_name()<<"."<<endl;
									}
									
									else{
										cout<<"This club don't ample cash!"<<endl;
									}
								}
							}
						}
						
						if(m==0){
							ofstream file;
							file.open("player.txt",ios::out);
							for(int ad=0;ad<cl[m].get_playerSum();ad++){
								file<<endl<<cl[m].get_player(ad).get_name()<<" ";
								file<<cl[m].get_player(ad).get_age()<<" ";
								file<<cl[m].get_player(ad).get_ability()<<" ";
								file<<cl[m].get_player(ad).get_transfer()<<" ";
								file<<cl[m].get_player(ad).get_sum()<<" ";
								file<<cl[m].get_player(ad).get_limit();
							}
							file.close();
						}
						
						else if(m==1){
							ofstream file;
							file.open("player2.txt",ios::out);
							for(int ad=0;ad<cl[m].get_playerSum();ad++){
								file<<endl<<cl[m].get_player(ad).get_name()<<" ";
								file<<cl[m].get_player(ad).get_age()<<" ";
								file<<cl[m].get_player(ad).get_ability()<<" ";
								file<<cl[m].get_player(ad).get_transfer()<<" ";
								file<<cl[m].get_player(ad).get_sum()<<" ";
								file<<cl[m].get_player(ad).get_limit();
							}
							file.close();
						}
					if(temp1==0){
							ofstream file;
							file.open("player.txt",ios::out);
							for(int ad=0;ad<cl[temp1].get_playerSum();ad++){
								file<<endl<<cl[temp1].get_player(ad).get_name()<<" ";
								file<<cl[temp1].get_player(ad).get_age()<<" ";
								file<<cl[temp1].get_player(ad).get_ability()<<" ";
								file<<cl[temp1].get_player(ad).get_transfer()<<" ";
								file<<cl[temp1].get_player(ad).get_sum()<<" ";
								file<<cl[temp1].get_player(ad).get_limit();
							}
							file.close();
						}
						
						else if(temp1==1){
							ofstream file;
							file.open("player2.txt",ios::out);
							for(int ad=0;ad<cl[temp1].get_playerSum();ad++){
								file<<endl<<cl[temp1].get_player(ad).get_name()<<" ";
								file<<cl[temp1].get_player(ad).get_age()<<" ";
								file<<cl[temp1].get_player(ad).get_ability()<<" ";
								file<<cl[temp1].get_player(ad).get_transfer()<<" ";
								file<<cl[temp1].get_player(ad).get_sum()<<" ";
								file<<cl[temp1].get_player(ad).get_limit();
							}
							file.close();
						}
					
						break;
					}
					case 5:{
						cout<<"Please input the message of the new staff(name age ability)"<<endl;
						string na,ab;
						int ag;
						cin>>na>>ag>>ab;
						cl[m].add_staff(na,ag,ab);
						
						if(m==0){
							ofstream file;
							file.open("staff.txt",ios::out);
							for(int ad=0;ad<cl[m].get_staffSum();ad++){
								file<<endl<<cl[m].get_staff(ad).get_name()<<" ";
								file<<cl[m].get_staff(ad).get_age()<<" ";
								file<<cl[m].get_staff(ad).get_ability()<<" ";
						}
						
						file.close();
					}
					
					else if(m==1){
							ofstream file;
							file.open("staff2.txt",ios::out);
							for(int ad=0;ad<cl[m].get_staffSum();ad++){
								file<<endl<<cl[m].get_staff(ad).get_name()<<" ";
								file<<cl[m].get_staff(ad).get_age()<<" ";
								file<<cl[m].get_staff(ad).get_ability()<<" ";
						}
						
						file.close();	
					}
						break;
					}
					case 6:{
						cout<<"Please input the staff name you want to delete:"<<endl;
						string sn;
						cin>>sn;
						if(cl[m].sort_staff(sn)==-1)
						cout<<"Don't have such staff!"<<endl;
						
						else{
							cl[m].del_staff(cl[m].sort_staff(sn));
						}
						break;
					}
					case 7:{
						cout<<"The sum of players:"<<cl[m].get_playerSum();
						break;
					}
					case 8:{
						cout<<"Thw sum of staff:"<<cl[m].get_staffSum()<<endl;
						break;
					}
					case 9:{
						return;
						break;
					}
				}
			}
				
				}
				
			break;
		}
		case 2:{
			cout<<"Please input new club name:"<<endl;
			string na;
			cin>>na;
			cout<<"Please input the new club cash:"<<endl;
			long int ca;
			cin>>ca;
			cl[clubSum].init(na,ca);
			clubSum++; 
			break;
		}
		case 3:{
			cout<<"Please input the name of club:"<<endl;
			string temp;
			int de;
			cin>>temp;
			if(temp=="MU"){
				cout<<"Can't delete this club!"<<endl;
				return;
			}
			for(int i=0;i<clubSum;i++){
				if(cl[i].get_name()==temp)
				de=i;
			}
			
			for(int i=de+1;i<clubSum;i++){
				cl[i]=cl[i-1];
			}
			
			clubSum--;
			break;
		}
		case 4:{
		cout<<"Please input name of club:"<<endl;
		string temp;
		cin>>temp;
		
		
			for(int i=0;i<clubSum;i++){
				if(cl[i].get_name()==temp){
					cout<<cl[i].get_playerSum()<<endl; 
					cout<<"name:"<<cl[i].get_name()<<"   cash:"<<cl[i].get_cash()<<endl;		
					cout<<"Player:"<<endl;
				for(int J=0;J<cl[i].get_playerSum();J++){
				cout<<"name:"<<cl[i].get_player(J).get_name();
				cout<<"     age:"<<cl[i].get_player(J).get_age();
				cout<<"     ability:"<<cl[i].get_player(J).get_ability()<<endl;
				cout<<"goal sum:"<<cl[i].get_player(J).get_sum();
				cout<<"     transfer fee:"<<cl[i].get_player(J).get_transfer();
				cout<<"     limit age:"<<cl[i].get_player(J).get_limit()<<endl<<endl;
			   }	
			   cout<<"Staff:"<<endl;
			   for(int w=0;w<cl[i].get_staffSum();w++){
			   	cout<<"name:"<<cl[i].get_staff(w).get_name();
			   	cout<<"     age:"<<cl[i].get_staff(w).get_age();
			   	cout<<"     ability:"<<cl[i].get_staff(w).get_ability()<<endl<<endl;
			   }
		    }
		  }
				
			
		break;
	}
		case 5:{
			string Fc,Cname;
			cout<<"Please input the demand:"<<endl;
			fflush(stdin);
			getline(cin,Fc); 
			deal_space(Fc);
			cout<<Fc<<endl;
			int cf=0;
			while(Fc[cf]!=NULL){
				if(Fc[cf]=='|')
					cf+=2;
					
				cf+=6;
				string tempN;
				while(Fc[cf]!=NULL&&Fc[cf]!='|'){
					tempN+=Fc[cf];
					cf++;
				}
				int n=0;
				for(int x=0;x<clubSum;x++){
					if(cl[x].get_name()==tempN){
						n=1;
					  cout<<"The club:"<<cl[x].get_name()<<endl;
					  cout<<"The Player is listed as follow:"<<endl; 
					  for(int J=0;J<cl[x].get_playerSum();J++){
				cout<<"name:"<<cl[x].get_player(J).get_name();
				cout<<"     age:"<<cl[x].get_player(J).get_age();
				cout<<"     ability:"<<cl[x].get_player(J).get_ability()<<endl;
				cout<<"goal sum:"<<cl[x].get_player(J).get_sum();
				cout<<"     transfer fee:"<<cl[x].get_player(J).get_transfer();
				cout<<"     limit age:"<<cl[x].get_player(J).get_limit()<<endl<<endl;
			   }
					}
				}
			if(n==0)
				cout<<"Don't have the club:"<<tempN<<endl;
			}
			
			break;
		}	
		default:{
			cout<<"Don't such operation!"<<endl;
			break;
	}
	}
} 

#endif
