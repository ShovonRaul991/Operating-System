.MODEL tiny

.CODE
    ORG 7C00h  ; Origin address for MBR

    JMP start       ; Jump to the start of the code
    NOP             ; NOP instruction for padding

start:
    ; Display "Hello, from Shovon Raul"
    MOV AH, 0EH    ; Function to display character
    MOV AL, 'H'     ; Character to display
    INT 10h         ; BIOS interrupt for video services
    MOV AL, 'e'     ; Character to display
    INT 10h
    MOV AL, 'l'     ; Character to display
    INT 10h
    MOV AL, 'l'     ; Character to display
    INT 10h
    MOV AL, 'o'     ; Character to display
    INT 10h
    MOV AL, ','     ; Character to display
    INT 10h
    MOV AL, ' '     ; Character to display
    INT 10h
    MOV AL, 'f'     ; Character to display
    INT 10h
    MOV AL, 'r'     ; Character to display
    INT 10h
    MOV AL, 'o'     ; Character to display
    INT 10h
    MOV AL, 'm'     ; Character to display
    INT 10h
    MOV AL, ' '     ; Character to display
    INT 10h
    MOV AL, 'S'     ; Character to display
    INT 10h
    MOV AL, 'h'     ; Character to display
    INT 10h
    MOV AL, 'o'     ; Character to display
    INT 10h
    MOV AL, 'v'     ; Character to display
    INT 10h
    MOV AL, 'o'     ; Character to display
    INT 10h
    MOV AL, 'n'     ; Character to display
    INT 10h
    MOV AL, ' '     ; Character to display
    INT 10h
    MOV AL, 'R'     ; Character to display
    INT 10h
    MOV AL, 'a'     ; Character to display
    INT 10h
    MOV AL, 'u'     ; Character to display
    INT 10h
    MOV AL, 'l'     ; Character to display
    INT 10h
    MOV AL, 13H      ; Carriage return
    INT 10h
    MOV AL, 10H      ; Line feed
    INT 10h

    ; Halt
    HLT

    ; Fill the rest of the MBR with zeros and add boot signature
    ;TIMES 510 - ($ - $$) DB 0  ; Fill the rest of the MBR with zeros  ;Not working for MASM in DOS machine
    DW 0AA55H                   ; Boot signature

END
