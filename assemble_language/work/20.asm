DATA SEGMENT
    MEM DW 20 DUP(0,1,0,2,0) 
    TEMP DW 100 DUP(0)
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE,DS:DATA
START:
    MOV AX,DATA
    MOV DS,AX
    MOV DX,0
    MOV SI,OFFSET MEM
    MOV DI,OFFSET TEMP
    MOV CX,100
SORT_MEM:
    CMP [SI],0
    JZ ZERO
    MOV AX,[SI]
    ADD [DI],AX
    ADD DI,2
    INC DX
ZERO:
    ADD SI,2
LOOP SORT_MEM
    
    MOV CX,200 
    MOV SI,OFFSET MEM   
    
INIT:
    MOV [SI],0 
    INC SI
LOOP INIT  

    MOV CX,DX
    MOV SI,OFFSET MEM
    MOV DI,OFFSET TEMP

COPY:
    MOV AX,[DI] 
    MOV [SI],AX
    ADD SI,2
    ADD DI,2
LOOP COPY    

    MOV AH, 02H
	MOV DL, "A"
	INT 21H
       
CODE ENDS
END START