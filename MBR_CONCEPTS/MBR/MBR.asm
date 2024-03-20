; MBR.asm

; Define the MBR as a 512-byte buffer filled with zeros
times 510 - ($ - $$) db 0
dw 0xAA55; Boot signature at the end of the MBR
