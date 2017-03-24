DATA SEGMENT 
   SET DB -7,0,0,0,3,4,5,6,7,8,9,1,2,3,4,5,6,-8,-9,-10
   POSI DB 20 DUP(0)
   NEGI DB 20 DUP(0)
   EQUAL DB "Equal zero:$" 
   LARGE DB "Large zero:$"
   LESS DB "Less zero:$" 
   COUNT1 DB 0
   COUNT2 DB 0  
   COUNT3 DB 0 
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE,DS:DATA
    START:
         MOV AX,DATA
         MOV DS,AX 
         
         MOV BP,OFFSET SET
         MOV CX,20         ;设置循环的次数
    SETIN:
      CMP [BP],0
      JE IS_ZERO
      CMP [BP],0 
      JG LA_ZERO 
      CMP [BP],0
      JL LES_ZERO
      LES_ZERO:
	    MOV AX,[BP]
	    MOV NEGI,AX
	    INC NEGI
            INC COUNT3
            JMP FLAG 
      IS_ZERO:
            INC COUNT1
            JMP FLAG
      LA_ZERO:
            MOV AX,[BP]
	    MOV POSI,AX
	    INC POSI
            INC COUNT2
            JMP FLAG
             
       FLAG: 
            INC BP
            LOOP SETIN   
       
       MOV AH,09H
       MOV DX,OFFSET  EQUAL
       INT 21H     ;打印输出条件
       
       MOV BX,OFFSET COUNT1
       CALL DISPBXD   ;调用转换函数
       
       MOV AH, 02H
	   MOV DL, 0DH
	   INT 21H
	   MOV DL, 0AH
	   INT 21H	    ;打印换行
	   
       MOV AH,09H
       MOV DX,OFFSET LARGE 
       INT 21H     ;打印输出条件  
       
       MOV BX,OFFSET COUNT2
       CALL DISPBXD  
       
       MOV AH, 02H
	   MOV DL, 0DH
	   INT 21H
	   MOV DL, 0AH
	   INT 21H	    ;打印换行 
	   
	   MOV AH,09H
       MOV DX,OFFSET LESS 
       INT 21H     ;打印输出条件 
       
       MOV BX,OFFSET COUNT3
       CALL DISPBXD  
;================
    DISPBXD  PROC  NEAR
       PUSH SI
       PUSH CX
       PUSH AX
       PUSH BX
       PUSH DX
       MOV  SI,10 
       XOR  CX,CX
       MOV  AX,BX
  NEXT:MOV  DX,10
       DIV  SI
       PUSH  DX 
       INC  CX
       CMP  AX,0 ;商为0则完成转换
       JNZ  NEXT
  OUTP:POP  DX
       ADD  DL,30H
       MOV  AH,2 
       INT  21H
       LOOP OUTP
       
       POP SI
       POP CX
       POP AX
       POP BX
       POP DX  
       RET
       DISPBXD  ENDP
;================       
       
       CONVERT PROC NEAR 
       CONVERT ENDP
       MOV AH,4CH
       INT 21H 
CODE ENDS
END START