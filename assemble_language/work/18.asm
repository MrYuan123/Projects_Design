DATA SEGMENT
   ENG DB 'SUNSUNSUNSUNSUNSUNSUNSUNSUNSUN','$' 
   LEN DW $-ENG
   PRIN DB 'SUM:','$'
DATA ENDS                  

CODE SEGMENT
    ASSUME CS:CODE,DS:DATA
START:
     MOV AX,DATA
     MOV DS,AX
     MOV SI,OFFSET ENG 
     MOV BX,0
     MOV DI,LEN
     SUB DI,1
FIND:
    CMP DI,2
    JZ  OUTPUT
    CMP DI,1
    JZ  OUTPUT
    CMP DI,0
    JZ  OUTPUT
    CMP [SI],'S'
    JNZ NO
    CMP [SI+1],'U'
    JNZ NO
    CMP [SI+2],'N'
    JNZ NO

    INC BX
    SUB DI,3
    ADD SI,3
    JMP FIND
    
    JMP OUTPUT
NO:
    SUB DI,1 
    ADD SI,1
    JMP FIND

OUTPUT:
    MOV AH,09H
    MOV DX,OFFSET PRIN
    INT 21H
    
    MOV AX, BX 
    MOV BL, 0AH
OUTLOOP:   
    
	DIV BL
	MOV DL, AH
	ADD DL, '0'
	MOV AH, 02H
	INT 21H
	CMP AL, 0
	JZ OUTLOOP
	    
            
    MOV AH,4CH
    INT 21H
CODE ENDS
END START