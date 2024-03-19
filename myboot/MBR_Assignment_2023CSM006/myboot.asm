; filename: MBR.asm 

org 7C00h  ; Origin address for MBR

section .text
    global _start

_start:
    ; Display "Hello, from Shovon Raul"
    mov ah, 0x0E         ; Function to display character
    mov al, 'H'          ; Character to display
    int 10h              ; BIOS interrupt for video services
    mov al, 'e'          ; Character to display
    int 10h
    mov al, 'l'          ; Character to display
    int 10h
    mov al, 'l'          ; Character to display
    int 10h
    mov al, 'o'          ; Character to display
    int 10h
    mov al, ','          ; Character to display
    int 10h
    mov al, ' '          ; Character to display
    int 10h
    mov al, 'f'          ; Character to display
    int 10h
    mov al, 'r'          ; Character to display
    int 10h
    mov al, 'o'          ; Character to display
    int 10h
    mov al, 'm'          ; Character to display
    int 10h
    mov al, ' '          ; Character to display
    int 10h
    mov al, 'S'          ; Character to display
    int 10h
    mov al, 'h'          ; Character to display
    int 10h
    mov al, 'o'          ; Character to display
    int 10h
    mov al, 'v'          ; Character to display
    int 10h
    mov al, 'o'          ; Character to display
    int 10h
    mov al, 'n'          ; Character to display
    int 10h
    mov al, ' '          ; Character to display
    int 10h
    mov al, 'R'          ; Character to display
    int 10h
    mov al, 'a'          ; Character to display
    int 10h
    mov al, 'u'          ; Character to display
    int 10h
    mov al, 'l'          ; Character to display
    int 10h
    mov al, 13           ; Carriage return
    int 10h
    mov al, 10           ; Line feed
    int 10h

    ; Halt
    hlt

    times 510 - ($ - $$) db 0  ; Filling the rest of the MBR with zeros
    dw 0xAA55                  ; Boot signature
