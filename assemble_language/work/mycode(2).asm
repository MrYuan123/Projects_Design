DATA SEGMENT
	STRING DB 100 DUP(0)
	CHAR_NUM DW 0
	DIGI_NUM DW 0
	OTHER_NUM DW 0
DATA ENDS

STACK SEGMENT STACK
	DW 10H DUP(0)
STACK ENDS

CODE SEGMENT
	ASSUME CS:CODE, DS:DATA, SS:STACK
START:
	MOV AX, DATA
	MOV DS, AX
	MOV AX, STACK
	MOV SS, AX

	MOV BX, OFFSET STRING


	PUSH BX
	CALL CIN_STRING

	PUSH BX
	CALL COUNT

	CALL ENTER
	MOV DX, CHAR_NUM
	CALL COUT
	
	CALL ENTER
	MOV DX, DIGI_NUM
	CALL COUT
	
	CALL ENTER
	MOV DX, OTHER_NUM
	CALL COUT

	MOV AH, 4CH
	INT 21H

;------------------------------------------
;	�����ַ����ӳ���
;	��ڣ���bx����¼�ַ����±�
;	���ڣ���
;------------------------------------------
CIN_STRING PROC NEAR
	PUSH BP
	MOV BP, SP
	PUSH AX	

	MOV SI, [BP+4]

	
INPUT:    
    MOV AH, 01H
	INT 21H
	CMP AL, 0DH
	JZ FINISH
	MOV [SI], AL
	INC SI
	JMP INPUT

FINISH:
	MOV [SI], '$'

	POP AX
	POP BP
	RET
CIN_STRING ENDP

;------------------------------------------
;	�ַ������ӳ���
;	��ڣ���bx����¼�ַ����±�
;	���ڣ���
;------------------------------------------	
COUNT PROC NEAR
	PUSH BP
	MOV BP, SP
	PUSH AX			;��¼�����ַ�
	PUSH CX			;��¼��ĸ
	PUSH DX			;��¼����

	MOV SI, [BP+4]
	MOV AX, 0
	MOV CX, 0
	MOV DX, 0
	
COUNTER:
	CMP [SI], '0'
	JB OTHER
	CMP [SI], '9'
	JBE DIG
	CMP [SI], 'A'
	JB OTHER
	CMP [SI], 'Z'
	JBE CHAR
OTHER:
	INC AX	
	JMP FINAL
CHAR:
	INC CX
	JMP FINAL
DIG:	
	INC DX
FINAL:
    INC SI	
	CMP [SI], '$'
	JNZ COUNTER

	MOV OTHER_NUM, AX
	MOV CHAR_NUM, CX
	MOV DIGI_NUM, DX

	POP DX
	POP CX
	POP AX
	POP BP
	RET 
COUNT ENDP

;------------------------------------------
;	��������ӳ���
;	��ڣ���dx��Ҫ�������
;	���ڣ���
;------------------------------------------
COUT PROC ENDP
	PUSH DX
	PUSH AX

	MOV AH, 02H
	ADD DX, '0'
	INT 21H	

	POP AX
	POP DX
	RET
COUT ENDP
 
;------------------------------------------
;	����س������ӳ���
;	��ڣ���
;	���ڣ���
;------------------------------------------
ENTER PROC ENDP
	PUSH DX
	PUSH AX

	MOV AH, 02H
	MOV DX, 0DH
	INT 21H	
	MOV DX, 0AH
	INT 21H
	
	POP AX
	POP DX
	RET
ENTER ENDP

CODE ENDS
	END START