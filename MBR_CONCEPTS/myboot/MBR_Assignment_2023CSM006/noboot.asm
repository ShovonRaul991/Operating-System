; filename: MBR.asm 

org 7C00h  ; Origin address for MBR

section .text
    global _start

_start:


    times 510 - ($ - $$) db 0  ; Filling the rest of the MBR with zeros
    dw 0xAA55                  ; Boot signature
