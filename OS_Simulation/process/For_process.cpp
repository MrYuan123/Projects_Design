#include "operator.cpp"
#include <stdlib.h>
#include <iostream>
using namespace std;
#define MAX_LENTH_OF_LINE 1024
#define MAX_CONSTRUCTION_AMOUNT 5 
#define DEFAULT_LINES 2
#define CONSTRUCTION_PER_PROCESS 2
#define CPU 1
#define IO 2

class myProcess {
	public:
		int pid;
		int statu;
		int time_to_run;
		int current_time;  // current executing clock
		int construction_lines;
		int current_line;  // the line we are executing  from 0th line
		int construction[MAX_CONSTRUCTION_AMOUNT][3];  // construction[i][0] property  [1] time it takes  [2]==1 finished this line ==0 not 
		 					//	数组千万别越界。。。。 
		static int current_construct_location;
		static int process_amount;
		myProcess() {
			process_amount++;
			cout << "Input the instruction amount for this process\n";
			cin >> this->construction_lines;
			cout << "Amount get!\n";
			this->current_line = 0;
			this->current_time = 0;
			this->time_to_run = 0;
			for (int i = 0; i < this->construction_lines; i++)
				for (int j = 0; j < 2; j++)
					this->construction[i][j] = 0;
		}
		
		myProcess(int pid, int construction_lines)  // 根据输入的指令条数读取 
		{
			this->current_line = 0;
			this->pid = pid;
			int statu = 0;//初始化状态 
			this->construction_lines = construction_lines;
			this->current_time = 0;
			this->time_to_run = 0;
			for (int i = 0; i < MAX_CONSTRUCTION_AMOUNT; i++)
				for (int j = 0; j < 2; j++)
					this->construction[i][j] = 0;
		}
		
		void setConstructions();
		
		void setPid(int pid) {
			this->pid = pid;
		}
		
		void setTimeToRun(int time_to_run) {
			this->time_to_run = time_to_run;
		}
		
		void setStatu(int statu) {
			this->statu = statu;	
			/*
				statu = -1  waiting for memory
				statu = 0  get the memory
				statu = 1 ready in the ready queue
				statu = 2 running
				statu = 3 waiting in the waiting queue
				statu = 4 finished state
			
			*/
		}
		
		int getMem()
		{
			_int i = _int(pid);
			_int share = _int(0);  // shared memory    
			while(_read(share.addr, sizeof(int), 1) == NULL){    // 生产者消费者问题 signal ! 
				this->statu = -1; // waiting for memory state		
				return -1;
			}
			this->statu = 0; // get the resource
			return 0;
		}
		

		
		int getCurrentProperty() {
			return this->construction[current_line][0];
		}
		
		int getTimeToRun() {
			return this->time_to_run;
		}
		
	
		int runOneStep();
			
		~myProcess() {}
		
		
	/*	int getSize() {
			return this->sizeInByte;
		} */
};

	int myProcess::current_construct_location = 0;  // 初始化静态数据成员 
	int myProcess::process_amount = 0;


int SetPositionByLine(FILE *fp, int nLine)  
{  
    int i = 0;  
    char buffer[MAX_LENTH_OF_LINE + 1];
    fpos_t pos;  
  
    rewind(fp);  
    for (; i < nLine; i++)  
        fgets(buffer, MAX_LENTH_OF_LINE, fp);  
    fgetpos(fp, &pos);  
    return 0;  
}  






/*
	We have different kinds of instruction
	some just use CPU -----------dispatch CPU and use, change dispatch when the clock is up
	some IO like input, just change its state to waiting and return waiting, despite the running time and change dispatch.
*/

// 有了已用时间， 又有所有指令的时间， 可以计算执行到具体哪一条指令 
int myProcess::runOneStep() {  // line is the line of the construction table 
	this->current_time++;
	int time_sum = 0, flag = 0;
	for (int t = 0; t < this->construction_lines; t++) {
		time_sum += this->construction[t][1];
		if (time_sum > this->current_time) {  // 当current_time 首次大于 current_time的时候得到 
			this->current_line = t; // Executing the (t+1)th construction 
			flag = 1;
			break;
		} else if (time_sum == this->current_time) {
			this->current_line = t;
			flag = 1;
			this->construction[t][2] = 1; // JUST FINSHED THIS CONSTRUCTION
		/*	cout << "\n\n pid " << this->pid << " 跑完了第 " << t << "行指令 \n\n"; 
			this->current_line++; // 换下一行 
			cout << "转去执行第 " << current_line << "+1 行指令：" << this->construction[this->current_line][0] << endl;  
			*/
			break;
		}	
	}
	if (flag == 1) {
		switch (this->construction[this->current_line][0]) {
			case CPU:
				if (this->construction[this->current_line][2] == 1) {
				//	if (this->current_line + 1 <= this->construction_lines)   我们把行数加一放到外面去   要记得 有两处！！ 
				//		this->current_line++;
					return 3;  // 跑完了 CPU 指令 
				} else 
					return CPU;  // 当前正在跑 CPU 指令 
			case IO:
				if (this->construction[this->current_line][2] == 1) {
				//	if (this->current_line + 1 <= this->construction_lines)
				//		this->current_line++;
					return 4;  // 跑完了 IO 指令 
				}
				return IO; //　当前正在进行IO  
		}
		
	}

	else 
		return -1;
}

void myProcess::setConstructions() {
	FILE *fptr;
	char name;
	int construct_time;
	fptr = fopen("construction.txt", "r");
	
	
	for (int i = 0; i < this->construction_lines; i++) {
		SetPositionByLine(fptr, current_construct_location + i); // Read the i-1 line  
		fscanf(fptr, "%c %d", &name, &construct_time);
		switch (name) {
			case 'C':
				this->construction[i][0] = 1;
				this->construction[i][1] = construct_time;
				this->construction[i][2] = 0; // unfinished
				this->time_to_run += construct_time; 
				break;
			case 'K':
				this->construction[i][0] = 2;
				this->construction[i][1] = construct_time;
				this->time_to_run += construct_time;
				this->construction[i][2] = 0;
				break;
			default:
				break; 
		} 
		
	}
	
	current_construct_location += this->construction_lines; // 装填完一个进程的指令后，更改当前指令位置 
	
	
	fclose(fptr);
	
}




