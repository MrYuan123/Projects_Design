/*roundRun:
	New processes are all put to the tail of the ready queue
	Get the head of the ready queue to run
	After the time slice or the process is blocked, put the process to the tail of the ready queue
*/

/*
	for a process
	Only after the process has been allocated all the memory it needs can the OS dispatch it
*/
/*
				statu = -1  waiting for memory
				statu = 0  get the memory
				statu = 1 ready in the ready queue
				statu = 2 running
				statu = 3 waiting in the waiting queue
				statu = 4 finished state
			
			*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <windows.h>
#include "For_process.cpp"
#define CPU 1
#define IO 2

//#include "funcPart3.c"
//#include "operator.cpp"
using namespace std;

#define MAX_PROCESS_AMOUNT 3
#define TIME_SLICE 3
#define RUNNING 2
#define WAITING 3
#define READY 1
#define FINISHED 4

typedef struct pcb {
	int pid;
	int total_run_time; // estimated running time
	int time_used;  // the time the process has used the CPU
	int time_left; // the time until the process finishes
	int round;  // the time slice for the roundrun algorithm
	int count; // used to compare with the round to decide whether to swap out
	
	char info[30];
	struct pcb *next;
	char state[20];  // 'waiting' 'ready' 'block' 'terminal'
}PCB;


PCB *run = NULL, *ready = NULL, *waiting = NULL, *finish = NULL;  // four queue, the pointer points to the head
									// finish always point to the tail of the queue
PCB *ready_tail = ready, *waiting_tail = waiting, *waiting_head = NULL; /* points to the tail of the ready queue,
						used to point to the process using up its time slice */
PCB *finish_head = finish;


void roundRunDispatch();
void create(int pid);  // create a new process
void toWaitingTail(PCB *p);
void toReadyTail(PCB *p);
void block(char *pid);
void awaken(char *pid);

void toReadyTail(PCB *p) {  // 这个函数以及后面那个函数，仅限于 单个节点（不包含后继）的赋值，否则会造成，多赋了不必要的节点的问题 
	if(ready == NULL) { /* the ready queue is blank
					put this new process to the head of the ready queue*/
		strcpy(p->state, "Ready"); 
		//p->next = NULL;   
		// 如果被我们插入到ready队列的这个节点有后继节点， 那么在这里我们贸然把next 赋值为了NULL 就非常难受了。。。 
		ready = p;  // the new process becomes the ready head
		ready_tail = p;

	} else {	
		ready_tail->next = p; // put the process to the tail of the ready process
		ready_tail = p;
		strcpy(p->state, "Ready"); 

	}
}

void toWaitingTail(PCB *p) {
	if(waiting == NULL) { 
		strcpy(p->state, "Waiting");
		//p->next = NULL;
		waiting = p;  // the new process becomes the ready head
		waiting_tail = p;

	} else {
		waiting_tail->next = p;
		waiting_tail = p;
		strcpy(p->state, "Waiting");

		//p->next = NULL;
		for (PCB *pter = waiting; pter != NULL; pter = pter->next) 
			cout << pter->pid << endl;
		cout << "\n\n\n";
	}
}

void create(int pid, myProcess *Processes) {  // PCB stuff 
	PCB *p = new PCB();  /* Here we should not use malloc  
							Because when "new" allocates memory, it will call the default constructor, but malloc doesn't
							and string can only be assigned after calling the default string constructor*/
	char process_name[20];
	int i, max_space, time_input;
	cout << "pid is " << pid << endl;
	p->pid = pid;
//	cin >> process_name >> max_space >> time_input;
//	strcpy(p->name, process_name);
	p->total_run_time = Processes[pid].getTimeToRun();  // getTimeToRun failed
	p->time_left = p->total_run_time;
	p->time_used = 0;
    p->round = TIME_SLICE;
    
 
 
 	// 内存管理 

	int process_state = -1; // init state, waiting for memory 
	while((process_state = Processes[pid].getMem()) == -1){  // waiting for memory state
		cout << "\n\nthe process didn't get the memory, waiting\n\n";
	}

	cout << "Memory allocated finished\n";
	


	// 如果进程的首条指令不是IO 即放到ready队列 否则放到waiting中去 
	if(Processes[pid].construction[0][0] == CPU) {
		strcpy(p->state, "Ready"); 
		strcpy(p->info, "In ready queue");
		Processes[pid].setStatu(READY); 	
		toReadyTail(p);
		
	} else if (Processes[pid].construction[0][0] == IO) {
		strcpy(p->state, "Waiting"); 
		strcpy(p->info, "In waiting queue");
		Processes[pid].setStatu(WAITING); 
		toWaitingTail(p);
	}
	
}

void copyPCB(PCB *dest, PCB *source) {
	dest->pid = source->pid;
	dest->total_run_time = source->total_run_time; // estimated running time
	dest->time_used = source->time_used;  // the time the process has used the CPU
	dest->time_left = source->time_left; // the time until the process finishes
	dest->round = source->round;  // the time slice for the roundrun algorithm
	dest->count = source->count; 
	strcpy(dest->state, source->state);
	strcpy(dest->info, source->info);
	dest->next = NULL;
} 


void block(int pid) {
	PCB *current_process = NULL;
	if (run->pid != pid)
		cout << "The process you input is not running now" << endl;
	else {
		current_process = run;
		run = ready;   // put the first process of the ready queue to run
		ready = ready->next;
		run->next = NULL;
		toReadyTail(current_process);
	}
}

void printProcessMsg() {
	PCB *current_p = NULL;
	cout << "PRINT PROCESS INFO\npid\tRuntime  TimeLeft  Count Round State Info\n";
	
	if (run != NULL)
		cout << "run" << endl << run->pid << "\t" << run->time_used << "\t" << run->time_left
			<< "\t" << run->count << "\t" << run->round << "\t" << run->state << "\t" << run->info << endl;
			
	if(ready != NULL) {
		cout << "ready" << endl;	
		for (current_p = ready; current_p != NULL; current_p = current_p->next) {
			cout << current_p->pid << "\t" << current_p->time_used << "\t" << current_p->time_left
			<< "\t" << current_p->count << "\t" << current_p->round << "\t" << current_p->state << "\t" << current_p->info << endl;
		}
	}	
		
	if(waiting != NULL) {
		cout << "waiting" << endl;		
		for (current_p = waiting; current_p != NULL; current_p = current_p->next) { // 打印状态的时候不打印waiting_head 
			cout << current_p->pid << "\t" << current_p->time_used << "\t" << current_p->time_left
			<< "\t" << current_p->count << "\t" << current_p->round << "\t" << current_p->state << "\t" << current_p->info << endl;
		}
	}
		
	if(finish_head != NULL)	{
		cout << "finished" << endl;
		for (current_p = finish_head; current_p != NULL; current_p = current_p->next) {  
			cout << current_p->pid << "\t" << current_p->time_used << "\t" << current_p->time_left
			<< "\t" << current_p->count << "\t" << current_p->round << "\t" << current_p->state << "\t" << current_p->info << endl;
		}
	}
		
}


void roundRunDispatch(myProcess *Processes) {
	cout << "roundRunDispatch\n\n";
	bool totally_finish = false;
	PCB *finished_process = NULL;
	run = ready;  // put the first process of the ready queue to run
	ready = ready->next;
	run->next = NULL;
	

	waiting_head = waiting;  // 先创建好waiting队列的头 
	
	strcpy(run->state, "Running");
	Processes[run->pid].setStatu(RUNNING);
	
	int finish_num[MAX_PROCESS_AMOUNT], count = 0;
	cout << "a\n";
	
	
	
	int Timer = 0;
	while (run != NULL || waiting != NULL) {
		// 每次循环 当作一个时钟周期 
		Timer++;
		cout << "**********************************Time:" << Timer << "*********************************************\n";
		PCB *ptr_former = (PCB *) malloc (sizeof(PCB));
		ptr_former->next = waiting;
		// 让所有 waiting中的进程执行一个时钟的IO，直到程序完成IO指令的执行 
		
			
			
		int newPid[MAX_PROCESS_AMOUNT] = {0};	// 此数组用于记录新放入waiting队列进程，避免将放入队尾的新进程在同一个时钟周期中遍历 
		int id = 0;
		bool inTheNewWaitingLine = false;
		for (PCB *ptr_latter = ptr_former->next; ptr_latter != NULL;) {
			strcpy(ptr_latter->info, "Doing IO");		
			// 所有 IO 进程 时间属性改变1
			ptr_latter->time_left -= 1;
			ptr_latter->time_used += 1;
			// ptr_latter->count++; // 时间片只有 CPU 指令才会使用 
			 
			Processes[ptr_latter->pid].runOneStep();  
			
	
				
			
			
			int current_line = Processes[ptr_latter->pid].current_line;
			if (Processes[ptr_latter->pid].construction[current_line][2] == 1) {  // IO finished, we interrupt and delete the process in the waiting queue
				// 注意 这里 由于 IO 指令完成后，我们让current_line + 1了， 这里要减一 
				//  一个重要的bug  如果IO指令完成后 后面一条还是IO指令， 则不能删此节点！！ 		
				strcpy(ptr_latter->info, "IO construction finishes");
				Processes[ptr_latter->pid].current_line++;  // 加一行 
				
		
				if (ptr_latter->time_left == 0) { // 进程也完成了 
					finish_num[count] = ptr_latter->pid;
					count++;
					strcpy(ptr_latter->state, "Finished");
					strcpy(ptr_latter->info, "Process finished too jobs done");			
					Processes[ptr_latter->pid].setStatu(FINISHED);				
					ptr_latter->count = 0;  // count ?? 
					PCB *newFinishProcess = (PCB *) malloc (sizeof(PCB));
					copyPCB(newFinishProcess, ptr_latter);
					finished_process = newFinishProcess;
					
					if (finish == NULL) {
						finish = finished_process;
						finish_head = finish;
						finish->next = NULL;
					} else {
						finish->next = finished_process;
						finish = finished_process;
						finish->next = NULL;
					}
					
				} else { // 如果 进程未结束 取下一条指令 放入 ready 或 waiting 
					int current_line = Processes[ptr_latter->pid].current_line;					
					if (Processes[ptr_latter->pid].construction[current_line][0] == CPU) {
						PCB *inserer1 = (PCB *) malloc (sizeof(PCB));
						copyPCB(inserer1, ptr_latter);    // 因为一旦让指针相等，指针的后继也全部赋值了，只想添加一个节点的话 不能把后继赋值过去！ 
						toReadyTail(inserer1);
						strcpy(ptr_latter->info, "Put to ready queue");
						Processes[ptr_latter->pid].setStatu(READY);
					} else if (Processes[ptr_latter->pid].construction[current_line][0] == IO) { // 当前已完成的IO指令的下一跳指令还是IO 放到 waiting队列后 再次执行到的时候当前时钟周期不能再执行此进程 
						PCB *inserer2 = (PCB *) malloc (sizeof(PCB));
						copyPCB(inserer2, ptr_latter);
						toWaitingTail(inserer2);
						Processes[ptr_latter->pid].setStatu(WAITING);
						strcpy(ptr_latter->info, "Still do IO");
						newPid[id] = ptr_latter->pid;
						id++;
					} 
						
				}
				
				
				if (waiting == ptr_latter && waiting->next != NULL){  // 特殊情况 waiting正好指向待删除的节点 
					waiting = waiting->next;
				} else if (waiting == ptr_latter && waiting->next == NULL) { // 最后一个waiting节点 删除后 waiting为空 
					waiting = NULL;
				}
				
				ptr_former->next = ptr_latter->next; // 将此进程在waiting队列中去除 
				ptr_latter = ptr_latter->next;   //  这里，只是改了waiting队列的连接关系， 不能把这个节点直接删了 
				
				for (PCB *pter = waiting; pter != NULL; pter = pter->next) 
					cout << pter->pid << endl;
					
			} else {// end of if
				 ptr_former = ptr_former->next, ptr_latter = ptr_latter->next;
			}
			

			for(int i = 0; i < id; i++)
				if (ptr_latter != NULL && newPid[i] == ptr_latter->pid)
					inTheNewWaitingLine = true;
			if(ptr_latter == NULL || inTheNewWaitingLine)
				break;
			
		} // end of for

		
		if (run != NULL) {
			strcpy(run->state, "Running");
			strcpy(run->info, "Just Running");
			run->time_used += 1;
			run->time_left -= 1;
			run->count++;
			
	
			/*
				We have different kinds of instruction
				some just use CPU -----------dispatch CPU and use, change dispatch when the clock is up
				some IO like input, just change its state to waiting and return waiting, despite the running time and change dispatch.
			*/
			// 具体怎么执行由process 自己跑 调度算法中只处理其当前状态
			
			// 取指令执行一个时钟周期  这里只能是 CPU指令 
			int execute_result = Processes[run->pid].runOneStep(); // 当前进程执行一个时钟周期，  根据结果 调整ready 队列 和 waiting 队列 

			
			switch (execute_result) {
				case 1: // CPU

					
					// 首先当前CPU指令未完成  但如果时间片已经用完，则同样调度 
					if (run->count == run->round) { // Time is up
						run->count = 0;
						if (ready != NULL) {
							strcpy(run->state, "Ready");
							strcpy(run->info, "Swap out for time slice");
							Processes[run->pid].setStatu(READY);
							toReadyTail(run);
							run = ready;
							strcpy(run->state, "Running");
							strcpy(run->info, "Dispatch to run");
							Processes[run->pid].setStatu(RUNNING);
							ready = ready->next;
							run->next = NULL;
						} else {
							strcpy(run->info, "Continue to run");
						}
							
					}
					break;
					
				case 2: // IO
					break;
					
				case 3: // CPU just finished, update the run ready queue
					
					if (run->time_left == 0) {  // 不仅当前指令完成， 当前进程也完成 
						/*run out dispatch*/
						finish_num[count] = run->pid;
						count++;
						strcpy(run->state, "Finished");	
						strcpy(run->info, "Jobs done");		
						Processes[run->pid].setStatu(FINISHED);				
						run->count = 0;  // count ?? 
						
						PCB *newFinishProcess = (PCB *) malloc (sizeof(PCB));
						copyPCB(newFinishProcess, run);
						finished_process = newFinishProcess;
						
						if (finish == NULL) {
							finish = finished_process;
							finish_head = finish;
							finish->next = NULL;
						} else {
							finish->next = finished_process;
							finish = finished_process;
							finish->next = NULL;
						}
						
						
						
					} else { // 如果本进程没有结束，取下一跳指令，解析后放到 ready 或 waiting队列 
						
						Processes[run->pid].current_line++; // 该指令跑完， 行数加一 
						int current_line = Processes[run->pid].current_line;
						if (Processes[run->pid].construction[current_line][0] == CPU) {
							strcpy(run->info, "Go on using CPU");
							toReadyTail(run);
						}
							
						else if (Processes[run->pid].construction[current_line][0] == IO) {
							strcpy(run->info, "Go to IO");
							toWaitingTail(run);
						}
					}
					// 取ready队列 入 run队列 
					if (ready != NULL) {
						run = ready; // put the head of the ready queue to run
						strcpy(run->state, "aboutToRun");
						strcpy(run->info, "Run in next clock");
						Processes[run->pid].setStatu(RUNNING);
						ready = ready->next;
						run->next = NULL;
					} else {
						run = NULL;
					}
					 
					break;
					
				case 4: 
					break;
					
				default: 
					break;
					
			}// end of switch 
			
					
			
			
		}
		
	/*	if (ready == NULL && run == NULL && waiting->next == NULL && waiting != NULL) {
			cout << "Finally!";
			waiting = NULL;
		} */
			printProcessMsg();
			Sleep(2000);	
			
			
	} // end of while
	
	
	
	cout << "The finish sequence is: \n";
	for (int i = 0; i < count; i++) {
		cout << finish_num[i] << " ";
	}
}


int main() {

	myProcess *Processes = new myProcess[MAX_PROCESS_AMOUNT];  // Using the blank constructor and set the constructions 
	 
	
	for (int i = 0; i < myProcess::process_amount; i++) {
		Processes[i].setPid(i);
		Processes[i].setConstructions();
		create(i, Processes);  // init the process and put all the process to the ready queue
	}
	
	
	printProcessMsg();
	
	roundRunDispatch(Processes);
	return 0;
}
