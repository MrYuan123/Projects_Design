DATA SEGMENT
    FLD1B DB "personal computer"
    FLD2B DB 32
    FLD3B DB 20H
    FLD4B DB 01011011B
    FLD5B DB "32654"
    FLD6B DB 10 DUP(0)
    FLD7B PARTIAL  <"PART1",20>,<"PART2",50>,<"PART2",14>
    FLD1W DW FFF0H
    FLD2W DW 01011001B
    FLD3W EQU OFFSET FLD7B
    FLD4W DW 5,6,7,8,9
    FLD5W DW 5 DUP(0)   
    FLD6W EQU ((OFFSET FLD1W)-(OFFSET FLD1B))
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE,DS:DATA
START:
    MOV AX,DATA
    MOV DS,AX
    MOV BX,FLD4W[2]
    MOV AH,4CH
    INT 21H
ENDS
END START