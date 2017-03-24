DATA SEGMENT
    TABLE DW 100H DUP(1)  
    LEN DW ($-TABLE) / 2
    NUM DW 0
    FREQ DW 0
DATA ENDS
CODE SEGMENT 
   ASSUME CS:CODE,DS:DATE
START:
   MOV AX,DATA
   MOV DS,AX 
   
   MOV CX,LEN  
   MOV DX,OFFSET LEN       ;FLAG THE PLACE
   MOV SI,OFFSET TABLE     ;FIRST 
   MOV DI,OFFSET TABLE     ;SECOND
FIRST_RANGE:
    MOV AX,[SI]
    SECOND_RANGE:
        CMP AX,[DI]
        JNZ NO
        INC BX
        
        ADD DI,2
        CMP DI,DX
        JNZ SECOND_RANGE
     NO:
        CMP BX,NUM
        JNA NEXTLOOP
        MOV FREQ,BX
        MOV NUM,AX
     NEXTLOOP:
        MOV SI,DI
        MOV BX,0
        
        CMP SI,DX
        JNZ FIRST_RANGE
        
       MOV AX,FREQ
       MOV CX,NUM           
   MOV AH,4CH
   INT 21H 
CODE ENDS
END START
 