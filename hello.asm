use64
SYS_exit equ 60
SYS_write equ 1

    mov rax, SYS_write
    mov rdi, 1
    mov rsi, msg
    mov rdx, msg_len
    syscall

    ret

msg: db "Hello, World", 10
msg_len = $-msg
