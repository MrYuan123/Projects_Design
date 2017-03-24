#include<iostream> 
#include"stdafx.h"

using namespace std;

#ifndef _SHOW_H_
#define _SHOW_H_
void surface1(){
	cout<<"Please select club operstion:"<<endl;
	cout<<"Club Sum:"<<clubSum<<"(Inlude MU)"<<endl;
	cout<<"1:club operation    2:Add Club      3:Delete Club(You can't delete MU)"<<endl;
	cout<<"4:Inquire Club      5:Sort Club"<<endl;
}
void surface2(){
	cout<<"Please Select The Operation:"<<endl;
	cout<<"1:Add Player      2:Delete Player      3:Sort Player"<<endl;
	cout<<"4:Charge          5:Add Staff          6:Delete Staff"<<endl;
	cout<<"7:Player Sum      8:Staff Sum          9:Withdrawl"<<endl;
}

#endif
