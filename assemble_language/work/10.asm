DATA SEGMENT
    INPUT DB "Input:$"
    OUTPUT DB "Output:$"
    ERROR DB "Wrong word!$" 
DATA ENDS                          
CODE SEGMENT
      ASSUME DS:DATA,CS:CODE
      START: 
      MOV AX,DATA
      MOV DS,AX
      LEA DX,INPUT        
      MOV AH,9H  
      INT 21H     ;input the string 'INPUT'  
      
      MOV AH,01H
      INT 21H   
      cmp AL,61H
      JB QUIT
      CMP AL,7AH
      JA QUIT  
      
      MOV BL,AL        ;BL中暂存输入的字符
      
      MOV DL,0DH
      MOV AH,2
      INT 21H
      MOV DL,0AH
      MOV AH,2
      INT 21H       ;换行
      
      LEA DX,OUTPUT
      MOV AH,9H
      INT 21H   ;output the word "output"
      
      MOV AH,2  
      MOV DL,BL
      DEC DL
      INT 21H 
 
      INC DL
      INT 21H 
       
      INC DL
      INT 21H
      
      JMP FINAL
QUIT: 
      LEA DX,ERROR
      MOV AH,9H
      INT 21H
    
FINAL:
      MOV AH,4CH
      INT 21H
        
CODE ENDS  
END START 