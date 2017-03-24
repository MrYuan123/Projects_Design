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

using namespace std;

int main(void){
	init();
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
	surface1();
	surface2();
	sort_player(en);

	return 0;
} 
