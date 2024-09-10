bits 32

section     .text
    align   4
    dd      0x1BADB002
    dd      0x00
    dd      - (0x1BADB002+0x00)

global _start
extern kernel_main
_start:
    cli
    mov esp, os_stack_bottom
    call os_proc
    hlt
    jmp $

times 510 - ($ - $$) db 0
dw 0x55AA 


os_proc:
    call kernel_main
    
stop:
    jmp stop
section .bss
    resb 8192
os_stack_bottom:


