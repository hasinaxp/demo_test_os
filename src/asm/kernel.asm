bits 32

section     .text
    align   4
    dd      0x1BADB002
    dd      0x00
    dd      - (0x1BADB002+0x00)

global _start
extern kernel_main
extern call_constructors
_start:
    cli
    mov esp, os_stack_bottom
    call os_proc
    hlt
    jmp $

times 510 - ($ - $$) db 0
dw 0x55AA 


os_proc:
    call call_constructors
    
    mov [magic], eax        ; Store magic number in memory
    mov [mbi], ebx          ; Store mbi pointer in memory

    push dword [mbi]        ; Push mbi pointer onto the stack
    push dword [magic]      ; Push magic number onto the stack
    call kernel_main
    
stop:
    jmp stop


section .bss
    magic: resd 1               ; Reserve space for magic number
    mbi:   resd 1               ; Reserve space for mbi pointer
    resb 8192
os_stack_bottom:


