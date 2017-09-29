/*
    memory recycling and rearrangement
	In the beginning all the memory should be free
	Those occupied memory should be taken from those free one
*/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sstream>
#include <time.h>

#define MAX_AMOUNT 100
using namespace std;


typedef struct node {
    int start;
    int length;
    string tag;  // the name of the job
}job;

const int MAXJOB = 100;
job frees[MAXJOB]; // free blocks
int free_amount = 0;
job occupied[MAXJOB];  // occupied blocks
int occupied_amount = 0;

void init();
void readData();
void firstFit(string job_name, int job_length);
void memRecycle(string job_name);
string to_string(int val);
void show();


// firstFit algorithm
void firstFit(string job_name, int job_length) {

	int i, first = -1, j, flag, t;
	flag = 0;
	for (i = 0; i < free_amount; i++){
		if (frees[i].length >= job_length){   // found the first block to fit the job
			flag = 1;
			first = i;
			break;
		}
	}
	if (flag == 0)
		cout << "Sorry, no space for your new process" << endl;
	else {
		cout << "We have " << free_amount << " free blocks. " << "This job can be put to the free block number " << first << endl;
 		occupied[occupied_amount].start = frees[first].start;
		occupied[occupied_amount].tag = job_name;
		occupied[occupied_amount].length = job_length;
		occupied_amount++;

		// the former free block has been occupied
		if (frees[first].length > job_length) { //
			frees[first].start += job_length;
			frees[first].length -= job_length;
		} else {  // the length of the free block is exactly the same of the job_length, just move
			for (j = first; j < free_amount-1; j++)
				frees[j] = frees[j+1];
		    free_amount--;
		}
		
		cout << "After the change, we have " << free_amount << " free blocks" << endl << "its start and length becomes "
			<< frees[first].start << " and " << frees[first].length << "\n" 
			<< "The newest occupied block start at " << occupied[occupied_amount-1].start << " with length " << occupied[occupied_amount-1].length << endl; 
	}
}


void memRecycle(string job_name) {
	int i, j, flag = 0, found;
	int start;  // the start point of the job to recycle
	int length;  // the length of the job

	found = -1; // the "flag"
	for (i = 0; i < occupied_amount; i++) {
		if (occupied[i].tag == job_name) {
			found = i;
			start = occupied[i].start;
			length = occupied[i].length;
		}
 	}

	if (found == -1) {
		cout << "Job not found! Please check the name!" << endl;
	} else {
		for (i = 0; i < free_amount; i++) {
			// the former adjacent block
			if (frees[i].start + frees[i].length == start) {
				// also adjacent to the latter free block
				if ((i+1 < free_amount) && (frees[i+1].start == start+length)) {
					frees[i].length = frees[i].length + frees[i+1].length + length;

					// move the latter blocks
					for (j = i+1; j < free_amount; j++)
						frees[j] = frees[j+1];
					free_amount--;
					flag = 1;
					cout << "free block " << i << "adjacent to both sides, start at " << frees[i].start << "with new length " << frees[i].length << endl;
				} else {  // only adjacent to the former one
					frees[i].length = frees[i].length + length;
					flag = 1;
					cout << "free block " << i << "only adjacent to the former free block, start at " << frees[i].start << "with new length " << frees[i].length << endl;
				}	
			}

			// only adjacent to the latter one
			if (start + length == frees[i].start) {
				frees[i].start = start;
				frees[i].length += length;
				flag = 1;
			}
		}

		// not adjacent to any free block, put it to the end
		if (flag == 0) {
			frees[free_amount].start = start;
			frees[free_amount].length = length;
			free_amount++;
			cout << "With no adjacent free blocks, the newest free block start at " << frees[free_amount-1].start << " with length " << frees[free_amount-1].length << endl;
		}

		// free the occupied blocks
		for (i = found; i < occupied_amount; i++) {
			occupied[i] = occupied[i+1];
		}
		occupied_amount--;

	}

}


void init() {
	int i;
	srand((unsigned)time(NULL));
	cout << "We have " << free_amount << "free blocks" << endl;
	occupied_amount = rand() % 30;
	

	frees[0].start = 0;
	frees[0].length = 20 + rand() % 15;
	frees[0].tag = to_string(0);
//	printf("0\n"); 
	for (i = 1; i < free_amount; i++) {
		frees[i].length = rand() % 15 + 20;
		frees[i].start = frees[i-1].start + frees[i-1].length; // located consequently
		frees[i].tag = to_string(i);
	}
//	printf("1\n");

	for (i = 0; i < occupied_amount; i++) {
		if (i < free_amount) {
			occupied[i].start = frees[i].start + frees[i].length + rand() % 5;  // the gap between occupied and the free
			occupied[i].length = rand() % (frees[i+1].start - occupied[i].start);
			occupied[i].tag = to_string(i);
	//		printf("2\n");
		} else {
			occupied[i].start = occupied[i-1].start + occupied[i-1].length + rand() % 5 ;
			occupied[i].length = rand() % 30;
			occupied[i].tag = to_string(i);
	//		printf("3\n");
		}
	}
	printf("4\n");

	show();
}

void show() {
	cout << "id\tstart\tlength\ttags\n";
	for (int i = 0; i < free_amount; i++)
		cout << i << "\t" << frees[i].start << "\t" << frees[i].length << "\n";
	cout << "For those occupied" << endl;
	for (int i = 0; i < occupied_amount; i++) {
		cout << i << "\t" << occupied[i].start << "\t" << occupied[i].length << "\n";
	}
}

string to_string(int val) {
	ostringstream stream;
	stream << val;
	return stream.str();
}


int main() {
	for(int i= 0; i < 5; i++) {
		init();
//		firstFit("1000", 31);
		memRecycle("34");

	}
}
