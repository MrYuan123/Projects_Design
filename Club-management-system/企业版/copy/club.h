#include<iostream>
#include"people.h"
#include"player.h"
using namespace std;
 
#ifndef _CLUB_H_
#define _CLUB_H_
#define staff_sum       50
#define player_sum      30   

class club{
	private:
		string name;
		long int cash;
		people staff[staff_sum];
		player playerS[player_sum];
		int  staffNow[staff_sum];
		int  playerNow[player_sum];
		int playerSum;
		int staffSum;
		
	public:
		club();
		club(string name, long int cash);
		
		void init(string name, long int cash);
		string get_name();
		long int get_cash();
		void change_name(string na);
		void change_cash(long int ca);
		
		int get_playerSum();
		int get_staffSum();
		void change_playerSum(int s);
		void change_staffSum(int n);
		
		void add_player(string name,int age,string ability,int transfer,int sum,int limit);
		void del_player(int now);
		int sort_player(string na);
		player get_player(int i);
		void copy_player(player na);
		
		void add_staff(string na,int age, string ability);
		void del_staff(int now);
		int sort_staff(string na);
		people get_staff(int i);
}; 

#endif

