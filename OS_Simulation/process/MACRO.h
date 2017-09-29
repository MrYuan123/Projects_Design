/*
内存大小基于Ubuntu 16.04 x64
 
#define sizeof(int) 			4
#define sizeof(char)			1
#define sizeof(float)			4

*/

#define MEM_SIZE 0x4000000//64MB

#define DISK_SIZE 0x20000000//512MB

#define PAGE_SIZE 0x1000//4KB

#define ADDR_SIZE 32//32BIT,20BIT页表项,12BIT页内偏移量

#define SEGMENT_SIZE 0x400000//段内偏移4MB
