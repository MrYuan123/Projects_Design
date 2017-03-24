DATA SEGMENT 
    MAXIUM DB 50
    STRING1 DB 50 DUP(0)
    STRING2 DB 50 DUP(0) 
    COM_YES DB 'MATCH$'
    COM_NO DB 'NOMATCH$'
DATA ENDS

CODE SEGMENT 
    ASSUME CS:CODE,DS:DATA
START:
    MOV AX,DATA
    MOV DS,AX  
    
    MOV  BX,OFFSET STRING1
    CALL INPUT_STRING         ;��ȡ�ַ���1
    MOV BX,OFFSET STRING2
    CALL INPUT_STRING         ;��ȡ�ַ���2
    
    MOV SI,OFFSET STRING1
    MOV DI,OFFSET STRING2           
    CALL COMPARE_STRING  
;======================================�����ַ�����������    
INPUT_STRING PROC NEAR 
    PUSH BX
    PUSH AX
    PUSH CX 
    MOV CX,WORD PTR MAXIUM-1
LOOPTHIS:
    MOV AH,1
    INT 21H
    
    CMP AL,0DH
    JZ ENDINPUT
    
    MOV [BX],AL
    INC BX
    LOOP LOOPTHIS
ENDINPUT:
    MOV [BX], BYTE PTR '$' 
    MOV AH, 02H
	MOV DL, 0DH
	INT 21H
	MOV DL, 0AH
	INT 21H	
	
    POP BX
    POP AX
    POP CX
    RET     
INPUT_STRING  ENDP  
;======================================�Ƚ��ַ�����������
COMPARE_STRING PROC NEAR 
    PUSH SI
    PUSH DI
    PUSH CX
    PUSH DX
    
COMPARE_CIRCLE:
    MOV CL,[SI]
    CMP CL,[DI]
    JNZ NO
    
    CMP CL,'$'
    JZ YES 
    INC SI
    INC DI  
    JMP COMPARE_CIRCLE
YES:
    MOV AH, 09H
	MOV DX, OFFSET COM_YES
	INT 21H
	JMP FINISH
NO:
	MOV AH, 09H
	MOV DX, OFFSET COM_NO
	INT 21H
	JMP FINISH   
	 
FINISH:
    POP SI
    POP DI
    POP CX
    POP DX
    RET	
COMPARE_STRING ENDP    
;====================================================    
    MOV AH,4CH
    INT 21H
CODE ENDS
END START