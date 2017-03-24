DATA SEGMENT
     ARRAY DW 40H DUP(-1), 40H DUP(5), 40H DUP(3), 40H DUP(-5) 
     TOTAL DW $-ARRAY
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE,DS:DATA
START:
    MOV AX,DATA
    MOV DS,AX  
    
    MOV AX,TOTAL
    MOV CX,2
    DIV CX
    MOV TOTAL,AX
    
    MOV CX,TOTAL
    MOV AX,0
    MOV DX,0 
    MOV SI,OFFSET ARRAY
    MOV DI,0
SUM:
    CMP [SI],0
    JL NEGA
       
    ADD AX,[SI]
    ADC DX,0
    JMP CONTINUE
NEGA:
    MOV BX,[SI]
    NEG BX
    SUB AX,BX
    SBB DX,0      ;此处有疑问 
    
CONTINUE:
    ADD SI,2 
    INC DI
    LOOP SUM
;---------------------------   
    MOV CX,0100H
    DIV CX 
; --------------------------
    MOV CX,TOTAL 
    MOV SI,OFFSET ARRAY
    MOV BX,0
    
COUNTER:
    CMP [SI],AX
    JAE GONO
    INC BX
GONO:
    ADD SI,2
    LOOP COUNTER
    
    MOV BX,AX
   
    MOV AH,4CH
    INT 21H  
CODE ENDS
END START 