DATA SEGMENT
HELLO DB 'HELLO!',0DH,0AH,'$'
DATA ENDS
STACK SEGMENT STACK
    DW 100H DUP(?)
STACK ENDS
CODE SEGMENT          
    ASSUME CS:CODE,DS:DATA,SS:STACK 
START: MOV AX,DATA
       MOV DS,AX
       LEA DX,HELLO
       MOV AH,09
       INT 21H
       MOV AX,4C00H
       INT 21H
CODE   ENDS
       END START
                                                                        `