TITLE .28
.MODEL SAMLL
.DATA
    A DW 4
    B DW 2
    C DW 1 
    D DW ?   
.CODE
START:
    MOV AX,@DATA
    MOV DS,AX
    
    CMP A,0
    JZ SETZERO
    CMP B,0
    JZ SETZERO
    CMP C,0
    JZ SETZERO
    MOV AX,A
    ADD AX,B
    ADD AX,C
    MOV D,AX
    JMP FINAL
    
SETZERO:
    MOV A,0
    MOV B,0
    MOV C,0
FINAL:    
    MOV AH,4CH
    INT 21H
    END START