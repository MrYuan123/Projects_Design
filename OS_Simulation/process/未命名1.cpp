#include <cstdlib>
#include <cstdio>
#include <iostream>
#define MAX_LENTH_OF_LINE 1024
using namespace std;


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


int main() {
	FILE *fptr;
	char name;
	int time; 
	char buffer[MAX_LENTH_OF_LINE + 1];
	fptr = fopen("construction.txt", "r");
	SetPositionByLine(fptr, 1);
//	fgets(buffer, MAX_LENTH_OF_LINE, fptr);
	fscanf(fptr, "%c %d", &name, &time);
	cout << name << " " << time << endl;
	switch (name) {
		case 'C':
			cout << "This is a calculation construct, it takes " << time;
			break;
		case 'K':
			cout << "Keyboard constructino, it takes " << time;
			break;
		default:
			cout << "Invalid construction\n";
			break; 
	} 
	fclose(fptr);
	cout << "\n\n\n\n";
	char a = (char)(1 + 48);
	cout << a; 
}



