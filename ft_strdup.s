section .text
    global  _ft_strdup
    extern  _malloc
    extern  _ft_strlen
    extern  _ft_strcpy
    extern  ___error

_ft_strdup:
    push rdi            
    call _ft_strlen     
    inc rax             
    mov rdi, rax        
    call _malloc        
                        
    jc _err
    mov rdi, rax        
    pop rsi             
    call _ft_strcpy     
    ret

_err:
    push rax            
    call ___error       
    pop rdx             
    mov [rax], rdx      
    mov rax, -1         
    ret
