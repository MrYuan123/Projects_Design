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

void toReadyTail(PCB *p) {  // ��������Լ������Ǹ������������� �����ڵ㣨��������̣��ĸ�ֵ���������ɣ��ำ�˲���Ҫ�Ľڵ������ 
	if(ready == NULL) { /* the ready queue is blank
					put this new process to the head of the ready queue*/
		strcpy(p->state, "Ready"); 
		//p->next = NULL;   
		// ��������ǲ��뵽ready���е�����ڵ��к�̽ڵ㣬 ��ô����������óȻ��next ��ֵΪ��NULL �ͷǳ������ˡ����� 
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
    
 
 
 	// �ڴ���� 

	int process_state = -1; // init state, waiting for memory 
	while((process_state = Processes[pid].getMem()) == -1){  // waiting for memory state
		cout << "\n\nthe process didn't get the memory, waiting\n\n";
	}

	cout << "Memory allocated finished\n";
	


	// ������̵�����ָ���IO ���ŵ�ready���� ����ŵ�waiting��ȥ 
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
		for (current_p = waiting; current_p != NULL; current_p = current_p->next) { // ��ӡ״̬��ʱ�򲻴�ӡwaiting_head 
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
	

	waiting_head = waiting;  // �ȴ�����waiting���е�ͷ 
	
	strcpy(run->state, "Running");
	Processes[run->pid].setStatu(RUNNING);
	
	int finish_num[MAX_PROCESS_AMOUNT], count = 0;
	cout << "a\n";
	
	
	
	int Timer = 0;
	while (run != NULL || waiting != NULL) {
		// ÿ��ѭ�� ����һ��ʱ������ 
		Timer++;
		cout << "**********************************Time:" << Timer << "*********************************************\n";
		PCB *ptr_former = (PCB *) malloc (sizeof(PCB));
		ptr_former->next = waiting;
		// ������ waiting�еĽ���ִ��һ��ʱ�ӵ�IO��ֱ���������IOָ���ִ�� 
		
			
			
		int newPid[MAX_PROCESS_AMOUNT] = {0};	// ���������ڼ�¼�·���waiting���н��̣����⽫�����β���½�����ͬһ��ʱ�������б��� 
		int id = 0;
		bool inTheNewWaitingLine = false;
		for (PCB *ptr_latter = ptr_former->next; ptr_latter != NULL;) {
			strcpy(ptr_latter->info, "Doing IO");		
			// ���� IO ���� ʱ�����Ըı�1
			ptr_latter->time_left -= 1;
			ptr_latter->time_used += 1;
			// ptr_latter->count++; // ʱ��Ƭֻ�� CPU ָ��Ż�ʹ�� 
			 
			Processes[ptr_latter->pid].runOneStep();  
			
	
				
			
			
			int current_line = Processes[ptr_latter->pid].current_line;
			if (Processes[ptr_latter->pid].construction[current_line][2] == 1) {  // IO finished, we interrupt and delete the process in the waiting queue
				// ע�� ���� ���� IO ָ����ɺ�������current_line + 1�ˣ� ����Ҫ��һ 
				//  һ����Ҫ��bug  ���IOָ����ɺ� ����һ������IOָ� ����ɾ�˽ڵ㣡�� 		
				strcpy(ptr_latter->info, "IO construction finishes");
				Processes[ptr_latter->pid].current_line++;  // ��һ�� 
				
		
				if (ptr_latter->time_left == 0) { // ����Ҳ����� 
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
					
				} else { // ��� ����δ���� ȡ��һ��ָ�� ���� ready �� waiting 
					int current_line = Processes[ptr_latter->pid].current_line;					
					if (Processes[ptr_latter->pid].construction[current_line][0] == CPU) {
						PCB *inserer1 = (PCB *) malloc (sizeof(PCB));
						copyPCB(inserer1, ptr_latter);    // ��Ϊһ����ָ����ȣ�ָ��ĺ��Ҳȫ����ֵ�ˣ�ֻ�����һ���ڵ�Ļ� ���ܰѺ�̸�ֵ��ȥ�� 
						toReadyTail(inserer1);
						strcpy(ptr_latter->info, "Put to ready queue");
						Processes[ptr_latter->pid].setStatu(READY);
					} else if (Processes[ptr_latter->pid].construction[current_line][0] == IO) { // ��ǰ����ɵ�IOָ�����һ��ָ���IO �ŵ� waiting���к� �ٴ�ִ�е���ʱ��ǰʱ�����ڲ�����ִ�д˽��� 
						PCB *inserer2 = (PCB *) malloc (sizeof(PCB));
						copyPCB(inserer2, ptr_latter);
						toWaitingTail(inserer2);
						Processes[ptr_latter->pid].setStatu(WAITING);
						strcpy(ptr_latter->info, "Still do IO");
						newPid[id] = ptr_latter->pid;
						id++;
					} 
						
				}
				
				
				if (waiting == ptr_latter && waiting->next != NULL){  // ������� waiting����ָ���ɾ���Ľڵ� 
					waiting = waiting->next;
				} else if (waiting == ptr_latter && waiting->next == NULL) { // ���һ��waiting�ڵ� ɾ���� waitingΪ�� 
					waiting = NULL;
				}
				
				ptr_former->next = ptr_latter->next; // ���˽�����waiting������ȥ�� 
				ptr_latter = ptr_latter->next;   //  ���ֻ�Ǹ���waiting���е����ӹ�ϵ�� ���ܰ�����ڵ�ֱ��ɾ�� 
				
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
			// ������ôִ����process �Լ��� �����㷨��ֻ�����䵱ǰ״̬
			
			// ȡָ��ִ��һ��ʱ������  ����ֻ���� CPUָ�� 
			int execute_result = Processes[run->pid].runOneStep(); // ��ǰ����ִ��һ��ʱ�����ڣ�  ���ݽ�� ����ready ���� �� waiting ���� 

			
			switch (execute_result) {
				case 1: // CPU

					
					// ���ȵ�ǰCPUָ��δ���  �����ʱ��Ƭ�Ѿ����꣬��ͬ������ 
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
					
					if (run->time_left == 0) {  // ������ǰָ����ɣ� ��ǰ����Ҳ��� 
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
						
						
						
					} else { // ���������û�н�����ȡ��һ��ָ�������ŵ� ready �� waiting���� 
						
						Processes[run->pid].current_line++; // ��ָ�����꣬ ������һ 
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
					// ȡready���� �� run���� 
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
