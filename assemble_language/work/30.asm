TITLE .30
.MODEL SMALL
.DATA 
    KEY DW 256 DUP("$") 
    CRLF DB 0DH,0AH,"$"
    MESS DB "The queue is:",0DH,0AH,"$"
    WOR DW 0
    NUM DW 0 
    OTHER DW 0
.STACK
    DW 100H DUP(0)
.CODE
START:
    MOV AX,@DATA
    MOV DS,AX
    MOV AX,STACK
    MOV SS,AX
    
    LEA DX,KEY
    MOV AH,0AH
    INT 21H     
     
    LEA DX,CRLF
    MOV AH,09H
    INT 21H         ;打印换行
    
    LEA DX,MESS
    MOV AH,09H
    INT 21H
    
    MOV AH,09H
    LEA DX,KEY
    INT 21H         ;打印输入的字符串
    
    CALL COUNT 
    
    MOV AH,4CH
    INT 21H 
;===================================================== 
CIN_STRING PROC NEAR
	PUSH SI
	PUSH AX
	PUSH BX
	MOV SI,KEY
INPUT: 

    MOV AH,01H
    INT 21H
    CMP AL,0DH
    JZ EN
    MOV [SI],AL
    INC SI
    JMP INPUT
EN:
    MOV [SI],"$"    	
	POP AX
	POP BX
	POP SI
	RET
CIN_STRING ENDP
;====================================================
COUNT PROC NEAR
    PUSH AX
    PUSH BX 
    PUSH CX
    PUSH SI
    PUSH DI  
    LEA SI,KEY
CO:
    CMP [SI],"$"
    JZ FINAL 
    CMP [SI],"/" 
    JNLE CASE3
    CMP [SI],"9"
    JNLE CASE1
    CMP [SI],"@"
    JNLE CASE3
    CMP [SI],"Z"
    JNLE CASE2
    CMP [SI],"a"
    JL CASE3
    CMP [SI],"z"
    JNLE CASE2 
    
    JMP GONO
    
CASE1:
    INC AX
    JMP GONO
CASE2:
    INC BX
    JMP GONO
CASE3:
    INC CX
    JMP GONO 
    
GONO:
    ADD SI,2
    JMP CO
    
FINAL: 
    MOV WOR,AX
    MOV NUM,BX
    MOV OTHER,CX   
    POP AX    
    POP BX 
    POP CX
    POP SI
    POP DI 
    RET
COUNT ENDP 
;======================================================   
    END START