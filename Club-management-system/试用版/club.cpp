#include<iostream>
#include"people.h"
#include"player.h"
#include"club.h"
#include<stdlib.h>
using namespace std;

club::club(){
	name='0';
	cash =0;
	playerSum=0;
	staffSum=0;
	for(int i=0;i<staff_sum;i++)
		this->staffNow[i]=-1;
		
	for(int i=0;i<player_sum;i++)
		this->playerNow[i]=-1;
}

club::club(string na, long int ca){
	name=na;
	cash=ca;
	
	playerSum=0;
	staffSum=0;
	for(int i=0;i<staff_sum;i++)
		this->staffNow[i]=-1;
		
	for(int i=0;i<player_sum;i++)
		this->playerNow[i]=-1;
}

void club::init(string na,long int ca){
	name=na;
	cash=ca;
}

//////////////////////////////////////////////////////////name和cash的操作 

void club::change_name(string na){
	name=na;
}
void club::change_cash(long int ca){
	cash=ca;
}
string club::get_name(){
	return name;
}
long int club::get_cash(){
	return cash;
}
/////////////////////////////////////////////////////playerSum和staffSum的操作 
int club::get_playerSum(){
	return playerSum;
}

int club::get_staffSum(){
	return staffSum;
}

void club::change_playerSum(int s){
	playerSum=s;
} 

void club::change_staffSum(int n){
	staffSum=n;
}
////////////////////////////////////////////player的操作 
void club::add_player(string name,int age, string ability, int transfer, int sum, int limit){
	if(playerSum<player_sum ){
		playerS[playerSum].init(name,age,ability,transfer,sum,limit);
		playerNow[playerSum]=1;
		playerSum++;
		//cout<<"Add success!"<<endl;
	}
}

void club::del_player(int now){
	if(now>=0){
	for(int i=now+1;i<player_sum;i++){
		playerS[i-1]=playerS[i];
	}
	playerSum--;
	playerNow[playerSum]=0;
	}
}

int club::sort_player(string na){
	for(int i=0;i<playerSum;i++){
		if(playerS[i].get_name()==na)
		return i;
	}
	return -1;
}

player club::get_player(int i){
	return playerS[i];
}

void club::copy_player(player na){
	if(playerSum<player_sum ){
		string name,ability;
		int age,transfer,sum,limit;
		
		name=na.get_name();
		ability=na.get_ability();
		age=na.get_age();
		transfer=na.get_transfer();
		sum=na.get_sum();
		limit=na.get_limit();
		
		playerS[playerSum].init(name,age,ability,transfer,sum,limit);
		playerNow[playerSum]=1;
		playerSum++;
	}
}
//////////////////////////////////////////////////staff的操作 
void club::add_staff(string na,int ag,string ab){
	if(staffSum<staff_sum){
		staff[staffSum].init(na,ag,ab);
		staffNow[staffSum]=1;
		staffSum++;
	}
}	

void club::del_staff(int now){

		
		for(int i=now+1;i<player_sum;i++){
		staff[i-1]=staff[i];
	}
	staffSum--;
	staffNow[playerSum]=0;
	
} 

int club::sort_staff(string na){
	for(int i=0;i<staffSum;i++){
		if(staff[i].get_name()==na)
		return i;
	}
	return -1;
}

people club::get_staff(int i){
	return staff[i];
}
