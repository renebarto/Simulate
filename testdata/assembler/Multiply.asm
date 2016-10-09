        CPU Intel8080

MULT:   MVI B,0     ;INITIALIZE MOST SIGNIFICANT BYTE
                    ;OF RESULT
        MVI E,9     ;BIT COUNTER
MULT0:  MOV A,C     ;ROTATE LEAST SIGNIFICANT BIT OF
        RAR         ;MULTIPLIER TO CARRY AND SHIFT
        MOV C,A     ;LOW-ORDER BYTE OF RESULT
        DCR E
        JZ DONE     ;EXIT IF COMPLETE
        MOV A,B
        JNC MULT1
        ADD D       ;ADD MULTIPLICAND TO HIGH-
                    ;ORDER BYTE OF RESULT IF BIT
                    ;WAS A ONE
MULT1:  RAR         ;CARRY=O HERE SHIFT HIGH-
                    ;ORDER BYTE OF RESULT
        MOV B,A
        JMP MULT0
DONE:   END
