#include<iostream>
using namespace std;

#ifndef _PEOPLE_H_
#define _PEOPLE_H_
class people{
	private:
		string name;
		int age;
		string ability;
		
	public:
	 people();
	 people(string name,int age,string ability);
	 void init(string name,int age,string ability);
	 string get_name();
	 int get_age();
	 string get_ability();
	 void change_name(string n);
	 void change_age(int s);
	 void change_ability(string a);
				
}; 
#endif
