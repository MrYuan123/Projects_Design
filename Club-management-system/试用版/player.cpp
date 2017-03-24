#include<iostream>
#include"player.h"

player::player(){
	name='0';
	age=0;
	ability='0';
	transfer=0;
	sum=0;
	limit=0;
}

player::player(string na,int ag,string ab,int trans,int su, int lim){
	name=na;
	ability=ab;
	age=ag;
	transfer=trans;
	sum=su;
	limit=lim;
}

void player::init(string na,int ag,string ab,int trans,int su, int lim){
	name=na;
	ability=ab;
	age=ag;
	transfer=trans;
	sum=su;
	limit=lim;
}

string player::get_name(){
	return name;
}

int player::get_age(){
	return age;
}

string player::get_ability(){
	return ability;
}

int player::get_transfer(){
	return transfer;
}

int player::get_sum(){
	return sum;
}
int player::get_limit(){
	return limit;
}

