DATA SEGMENT 
   ARRAY DW 23,26,2,100,32000,54,0
   ZERO DW ?
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE,DS:DATA
START:
    MOV AX,DATA
    MOV DS,AX
    LEA BX,ARRAY[12]
    MOV DX,[BX-2]   ;����BX���Ƿ��Ѿ�������ȷ�ĵ�ַ
    MOV AH,4CH       ;����ν�����ִ�г���
    INT 21H
ENDS
END START                      