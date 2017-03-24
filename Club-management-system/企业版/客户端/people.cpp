#include<iostream>
#include"people.h"
using namespace std;

people::people(){
	name='0';
	age=0;
	ability='0';
}

 people::people(string n,int a,string ab){
 	name=n;
 	age=a;
 	ability=ab;
 }
 
 void people::init(string na,int ag,string ab){
 	name=na;
 	age=ag;
 	ability=ab;
 }
 
 string people::get_name(){
 	return name;
 }
 
 int people::get_age(){
 	return age;
 }
 
 string people::get_ability(){
 	return ability;
 }
 
 void people::change_name(string n){
 	name=n;
 }
 
 void people::change_age(int s){
 	age=s;
 }
 
 void people::change_ability(string a){
 	ability=a;
 }
 
