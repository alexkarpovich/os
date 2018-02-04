mov ah, 0x0e

mov bx, HELLO
call print

call print_nl

mov bx, GOODBYE
call print

call print_nl

mov dx, 0x12fe
call print_hex

jmp $ ; jump to current address = infinite loop

%include "boot_print.asm"
%include "boot_print_hex.asm"

HELLO:
	db 'Hello, world', 0

GOODBYE:
	db 'Goodbye', 0

the_secret:
	db "Xerx"
; fill with 510 zeros minus the size of the previous code
times 510-($-$$) db 0
; magic number
dw 0xaa55
