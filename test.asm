ORG 0x00

START:  MOV A, #000000H
        MOV R0, #0x000005

LOOP:   ADD A, #0x02
        DJNZ R0, LOOP

        MOV 30H, A

        SJMP $

        END
