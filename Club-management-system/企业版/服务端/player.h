#include<iostream>
using namespace std;

#ifndef _PLAYER_H_
#define _PLAYER_H_
class player{
	private:
		string name;
		int age;
		string ability;
		int transfer;
		int sum;
		int limit;
		
	public:
	   player();
	   player(string name, int age, string ability, int transfer, int sum, int limit);
	   void init(string name, int age, string ability, int transfer, int sum, int limit);
	   string get_name();
	   int get_age();
	   string get_ability();
	   int get_transfer();
	   int get_sum();
	   int get_limit();
	   
};
#endif
