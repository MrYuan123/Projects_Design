#include<iostream>
#include<fstream>
#include"people.h"
#include"player.h"
#include"club.h"

#ifndef _STDAFX_H_
#define _STDAFX_H_
club en("MU",60000);
club ma("MA",70000);
club cl[10];
int  CB[10]={0};
int clubSum=0;     //	³ýÈ¥MU

void init(){
	ifstream file,fileS,file2,fileS2;
	file.open("player.txt");
	fileS.open("staff.txt");
	
	string na,ag,ab,trans,su,lim;
	int a,t,s,l;
	
	if(file == NULL){
		cout<<"Can't open this file!"<<endl;
	}
	
	else{
		while(0 == file.eof()){
			file>>na;
			file>>ag;
			file>>ab;
			file>>trans;
			file>>su;
			file>>lim;
			
			a=atoi(ag.c_str());
			t=atoi(trans.c_str());
			s=atoi(su.c_str());
			l=atoi(lim.c_str());
			en.add_player(na,a,ab,t,s,l);
		}
	}
	
	file.close();
	if(fileS == NULL){
		cout<<"Can't open this file!"<<endl;
	}
	
	else{
			while(0 == fileS.eof()){
				fileS>>na;
				fileS>>ag;
				fileS>>ab;
				a=atoi(ag.c_str());
				en.add_staff(na,a,ab);
			}
	}
	cl[clubSum]=en;
	CB[clubSum]=1;
	clubSum++;
	fileS.close();
	
	file2.open("player2.txt");
	fileS2.open("staff2.txt");

	if(file2 == NULL){
		cout<<"Can't open this file!"<<endl;
	}
	
	else{
		while(0 == file2.eof()){
			file2>>na;
			file2>>ag;
			file2>>ab;
			file2>>trans;
			file2>>su;
			file2>>lim;
			a=atoi(ag.c_str());
			t=atoi(trans.c_str());
			s=atoi(su.c_str());
			l=atoi(lim.c_str());
			ma.add_player(na,a,ab,t,s,l);
		}
	}
	
	file2.close();
	if(fileS2 == NULL){
		cout<<"Can't open this file!"<<endl;
	}
	
	else{
			while(0 == fileS2.eof()){
				fileS2>>na;
				fileS2>>ag;
				fileS2>>ab;
				a=atoi(ag.c_str());
				ma.add_staff(na,a,ab);
			}
	}
	cl[clubSum]=ma;
	CB[clubSum]=1;
	clubSum++;
	
	fileS2.close();
}

#endif
