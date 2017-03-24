TITLE .32
.MODEL SMALL
.DATA  
    SINGLIST DD 5 DUP(0)  
    INPUT DB "Input:$"
    ERROR db  0DH,0AH,"number must between 1 and 5!",0DH,0AH,"$"
    OUTPUT DB "START PLAY SONG:$"
    NEXTLINE DB 0DH,0AH,"$"

.CODE
START: 
    MOV AX,@DATA
    MOV DS,AX
INPUTN:
    MOV AH,09H
    LEA DX,INPUT
    INT 21H         ;œ‘ æinputÃ· æ
    
    MOV AH,01H
    INT 21H
    CMP AL,31H
    JL WARN
    CMP AL,35H
    JG WARN 
    
    MOV BH,00H
    MOV BL,AL
    ;SUB BL,30H
    
    MOV AH,09H
    LEA DX,NEXTLINE
    INT 21H
    
    LEA DX,OUTPUT
    INT 21H 
    
    MOV AH,02h
    MOV DL,BL
    INT 21H
    
    MOV AH,09H
    LEA DX,NEXTLINE
    INT 21H 
    
    JMP FINAL
WARN:
    MOV AH,09H
    LEA DX,ERROR
    INT 21H
    JMP INPUTN
FINAL:    
    MOV AH,4CH
    INT 21H
    END START
     
    

