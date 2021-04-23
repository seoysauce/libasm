section .text
    global _ft_read
    extern  ___error

_ft_read:
    mov rax, 0x2000003  ; read()
    syscall
    jc  _err            ; CF = 1(에러) 시
    ret

_err:
    push rax            ; rax to stack
    call ___error        
    pop  rdx
    mov [rax], rdx
    mov rax, -1
    ret