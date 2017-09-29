/* 
    When creating one process, we give it one PCB
    After it finishes its work, system recycles the PCB, the process terminals
	Four states: running ready terminal waiting 	

typedef struct node {
	char name[30];  
	int prio;   // priority
	int round;  // time slice
	int cputime;  // the cpu time occupied 
	int needtime; // the time until it finishes
	char state;   
	struct node *next;  
}PCB;

*/ 
/* 时间片轮转法：
	三个链队列，表示运行队列， 就绪队列， 完成队列
	输入： 进程标识符 以及进程所需时间， 申请空间存放进程PCB信息
	输出：  

	把每次把CPU分配给 就绪队列 首进程使用一个时间片，
	使用结束后强迫其让出处理器，让其排列到队列尾部
	实现此调度需要一个间隔时钟， 当一个进程开始运行时，时间片置入间隔时钟内， 间隔时钟中断时，
	中断处理程序通知处理器调度进行处理器切换工作
	 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct pcb {
	char name[10];  // 进程标识符
	int pArriveTime;  // 到达时间
	int pRunTime;  // 估计运行时间
	int round;  // 进程时间轮转时间片
	int cpuTime;  // 进程占用CPU时间
	int needTime;  // 进程到完成还需的时间
	int count;  // 计数器
	char state;  // 进程的状态
	struct pcb *next;  // 链指针 
}PCB;

PCB *finish, *ready, *tail, *run;  // 队列指针 完成队列， 就绪队列， 就绪队列尾部， 运行队列 
PCB head_input;
int N; // 进程数
void roundrun();  // 时间片轮转法
void inser(PCB *p);
void create();


void print_process() {
	int i;
	printf("name\tcpuTime\tneedTime\tcount\tround\tstate\n");
	for(i = 0; i < N; i++) {
		
	}
}

void insert(PCB *p){  // insert function for the roundrun
	tali->next = p;  // put the PCB to the tail of the waiting queue
	tail = p;
	p->next = NULL;
}

void create(){  
	PCB *p;
	int i, time, n;
	char name[30];  // process_name 
	ready = NULL;  // init the three queues  
	finish = NULL;
	run = NULL;
	printf("Please input the name and the run time of the process");
	for (i = 1; i <= N; i++){
		p = (PCB*) malloc (sizeof(PCB));
		cin >> p->name >> time;
		p->cpuTime = 0;
		p->needTime = time;
		p->count = 0;
		p->state = 'W'; // waiting
		p->round = 3;   // 轮转时间片为3
		if (ready != NULL)
			insert(p);  
		else{
			p->next = ready;
			ready = p;
			tail = p;
		} 
	}	 
	
	system("cls");  // clear screen
	printf("轮转法输出\n");
	printf("**************************************\n");
	print_process();   // 输出进程PCB信息
	run = ready;  //  将就绪队列的第一个进程投入运行
	ready = ready->next;
	run->state = 'R';  // Ready 
} 

void roundrun(){ // 时间片轮转法 调度进程
    while(run != NULL) {
        run->cpuTime = run->cpuTime + 1;
        run->needTime = run->needTime - 1;
        run->count = run->count + 1;
        if (run->needTime == 0) { // 运行完将进程变为完成态，插入完成队列
            run->next = finish;
            finish = run;
            run->state = 'F';  // finish
            run = NULL;
            if (ready != NULL) 
                firstin();  // 就绪队列不为空， 将第一个进程投入运行
        } else {
            
            if(run->count == run->round) { // 如果时间片到了 
                run->count = 0;
                if (ready != NULL) { // 就绪队列不为空 
                    run->state = 'W';  // 进程插入到就绪队列中 等待轮转
                    insert(run); 
                    firstin();  // 将就绪队列第一个进程投入运行  
                }
            }
            
            print_process(); // 输出进程信息 
        } 
    }

} 

