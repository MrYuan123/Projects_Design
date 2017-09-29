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
/* ʱ��Ƭ��ת����
	���������У���ʾ���ж��У� �������У� ��ɶ���
	���룺 ���̱�ʶ�� �Լ���������ʱ�䣬 ����ռ��Ž���PCB��Ϣ
	�����  

	��ÿ�ΰ�CPU����� �������� �׽���ʹ��һ��ʱ��Ƭ��
	ʹ�ý�����ǿ�����ó����������������е�����β��
	ʵ�ִ˵�����Ҫһ�����ʱ�ӣ� ��һ�����̿�ʼ����ʱ��ʱ��Ƭ������ʱ���ڣ� ���ʱ���ж�ʱ��
	�жϴ������֪ͨ���������Ƚ��д������л�����
	 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct pcb {
	char name[10];  // ���̱�ʶ��
	int pArriveTime;  // ����ʱ��
	int pRunTime;  // ��������ʱ��
	int round;  // ����ʱ����תʱ��Ƭ
	int cpuTime;  // ����ռ��CPUʱ��
	int needTime;  // ���̵���ɻ����ʱ��
	int count;  // ������
	char state;  // ���̵�״̬
	struct pcb *next;  // ��ָ�� 
}PCB;

PCB *finish, *ready, *tail, *run;  // ����ָ�� ��ɶ��У� �������У� ��������β���� ���ж��� 
PCB head_input;
int N; // ������
void roundrun();  // ʱ��Ƭ��ת��
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
		p->round = 3;   // ��תʱ��ƬΪ3
		if (ready != NULL)
			insert(p);  
		else{
			p->next = ready;
			ready = p;
			tail = p;
		} 
	}	 
	
	system("cls");  // clear screen
	printf("��ת�����\n");
	printf("**************************************\n");
	print_process();   // �������PCB��Ϣ
	run = ready;  //  ���������еĵ�һ������Ͷ������
	ready = ready->next;
	run->state = 'R';  // Ready 
} 

void roundrun(){ // ʱ��Ƭ��ת�� ���Ƚ���
    while(run != NULL) {
        run->cpuTime = run->cpuTime + 1;
        run->needTime = run->needTime - 1;
        run->count = run->count + 1;
        if (run->needTime == 0) { // �����꽫���̱�Ϊ���̬��������ɶ���
            run->next = finish;
            finish = run;
            run->state = 'F';  // finish
            run = NULL;
            if (ready != NULL) 
                firstin();  // �������в�Ϊ�գ� ����һ������Ͷ������
        } else {
            
            if(run->count == run->round) { // ���ʱ��Ƭ���� 
                run->count = 0;
                if (ready != NULL) { // �������в�Ϊ�� 
                    run->state = 'W';  // ���̲��뵽���������� �ȴ���ת
                    insert(run); 
                    firstin();  // ���������е�һ������Ͷ������  
                }
            }
            
            print_process(); // ���������Ϣ 
        } 
    }

} 

