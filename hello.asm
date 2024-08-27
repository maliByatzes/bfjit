format ELF64 executable

SYS_exit equ 60
SYS_write equ 1

segment executable
entry main
main:
    mov rax, SYS_write
    mov rdi, 1
    mov rsi, msg
    mov rdx, msg_len
    syscall

    mov rax, SYS_exit
    mov rdi, 0
    syscall

msg: db "Hello, World", 10
msg_len = $-msg
