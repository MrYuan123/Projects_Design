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
#include"client.h"

using namespace std;

int main(void){
	init();
	surface();
	int nu;
	cin>>nu;
	
	if(nu==2){
		
	}
	
	
	else{
		system("cls");
		while(1){
		Excutive();
		cout<<endl<<"Go on?(Y or N)"<<endl;
		char temp;
		cin>>temp;
		
		if(temp=='Y'||temp=='y')
		system("CLS");
		
		else
		  return 0;
	}	
	}
	
	surface1();
	surface2();

	return 0;
} 
