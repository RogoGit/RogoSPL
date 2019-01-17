; rdi - first argument
; rsi - second argument
mov     rdx, rsi
mov     rsi, rdi
mov     rax, 1
mov     rdi, 1
syscall
mov rax, 7
ret