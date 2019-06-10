global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global string_copy
global print_error
global in_fd

section .data
in_fd: dq 0

section .text

string_length:
             xor rax,rax
	         push rdi
	         xor rcx,rcx
	         dec rdi
calc_length: inc rcx
             inc rdi
		     cmp byte[rdi],0
		     jnz calc_length
	         dec rcx
	         pop rdi
	         mov rax,rcx
    ret



print_string:
    call string_length   ; syscall - syswrite (descriptor, buffer, length)
	mov rsi, rdi         ; rsi=rdi - адрес строки
	mov rdx, rax         ; rdx - string length
	mov rdi, 1           ; rdi - 1 - stdout
	mov rax, 1           ; sys_write
	syscall
    ret

print_error:
    call string_length   ; syscall - syswrite (descriptor, buffer, length)
	mov rsi, rdi         ; rsi=rdi - адрес строки
	mov rdx, rax         ; rdx - string length
	mov rdi, 2           ; rdi - 2 - stderr
	mov rax, 1           ; sys_write
	syscall
    ret

print_char:
    xor rax, rax
    push rdi            ; кладём на стэк 
    mov rdi, rsp        ; получаем адрес, откуда выводить
  	call print_string
  	pop rdi
  	xor rax, rax
    ret



print_newline:
    xor rax, rax
    mov rdi, 0xA       ;0xA - ASCII символ перехода строки
    call print_char
    ret



print_uint:
push r15
    xor rcx, rcx                 ; обнулчем счётчик и прочее
    xor rax, rax                  
	xor rdx, rdx
	mov rsi, rsp                 ; теперь голова стэка - начало строки с числом
	times 8 push 0               ; 64 бит - 8 ячеек по 8 
	mov rax,rdi                  ; в rax - строку
	mov r15,10                   ; делитель 
	dec rsi                      ; новый элемент
	mov byte [rsi],0             ; символ конца строки в стэк
 print_uint_loop:
    xor rdx,rdx                  ; Остатки
    div r15
	add rdx, 0x30                ; convert to ASCII
	dec rsi                      
	mov [rsi],dl                 
	cmp rax,0
	jz now_print
	jmp print_uint_loop
now_print:
	mov rdi,rsi                  ; отправляем строку для функции
	call print_string
	times 8 pop rax
	pop r15
    ret



print_int:
    xor rax,rax
	mov rax,rdi
	test rax,rax                 ; побитовое И с выставлением флагов
	js negative
	call print_uint
	jmp finish
negative:
     mov rdi,'-'
	 push rax                   ; сохраняем число
	 call print_char
	 pop rax
 	 neg rax                    ; меняем знак
	 mov rdi,rax
	 call print_uint
	 jmp finish
finish:
    ret



string_equals:
    xor rax,rax
	xor rcx,rcx
	xor r8,r8
    xor r9,r9
equ_loop:
   mov r8b,[rdi+rcx]
   mov r9b,[rsi+rcx]     ; заполняем строки и посимвольно сравниваем
   cmp r8b,r9b
   jnz comp_fail
   inc rcx
   cmp r9b,0
   jz comp_good
   jmp equ_loop
comp_good:
   mov rax,1
   ret
comp_fail:
   mov rax,0
    ret


read_char:
    xor rax, rax     ; sys_read
    dec rsp
	mov rdx, 1       ; 1 символ
	mov rdi, [in_fd]    
    mov rsi, rsp     ; куда записываем символ
    syscall
    test rax, rax     ; сколько прочитали, если 0, то конец ввода
    jz pr_char
    xor rax, rax
    mov al, [rsp]     
pr_char:
    inc rsp
    ret
  
read_word:
    push r13
    push r14
    xor r14, r14                         ; r14 - счётчик
    mov r13, rdi                         ; r13 - начало строки
    mov r10, rsi                         ; размер
 space_check_loop:                       ; здесь пропускаем пробелы
    call read_char
    cmp al, ' '
	jnz now_read_char
    jmp space_check_loop
go_on_reading:
    call read_char
    cmp r14, r10                         ; смотрим, чтобы не слишком большая                         
    jg read_0
    cmp al, ' '
    jz read_end
now_read_char:                             ; не пробел - читаем
	cmp al, 0                            
    jz read_end
	cmp al, 0xA
	jz read_end
	cmp al, 0x9                          
    jz read_end
    mov byte [r13+r14], al                ; записываем символ
    inc r14
    jmp go_on_reading
read_end:
    mov rax, r13                                              
    mov byte [r13+r14], 0                ; null terminator
    mov rdx, r14                         ; длина строки
    pop r14
    pop r13
    ret
read_0:
    xor rax, rax                        ; слишком много - обнуляем
    xor rdx, rdx 
    pop r14
    pop r13
    ret
 


parse_uint:
    xor rax,rax
	xor rcx,rcx
	xor rdx,rdx
    xor r8,r8
	mov r9,10
parse_loop:
    mov r8b,[rdi+rcx]         
    cmp r8b, 0x30             ; меньше кода 0 - плохо - заканчиваем
    jl escape
    cmp r8b, 0x39             ; больше кода 9 - плохо
    jg escape
    inc rcx
    sub r8b, 0x30
    mul r9
    add rax, r8              ; шаг за шагом в rax появляются цифры 
    jmp parse_loop
escape:
    mov rdx, rcx             ; кол-во символов
    ret


; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
	xor r8,r8
	mov r8b,[rdi]
	cmp r8b,45              ; '-' в ascii
	jz parse_neg
	call parse_uint
	jmp exit_of_parse
parse_neg:
    inc rdi                ; берём, что после минуса
	call parse_uint
	cmp rdx,0              ; если цифр нет, то и минус не считается
	jz exit_of_parse
	neg rax
	inc rdx                ; прибавляем минус
exit_of_parse:
    ret 



string_copy:
push rbx
	xor rcx, rcx             ; счётчик
	copy:       
	cmp rcx,rdx
	jz too_long_copy
	mov bl, [rdi + rcx]      ; через память нельзя
	mov [rsi + rcx], bl      ; копируем через регистр
	inc rcx                  ; следующий символ
	cmp bl, 0
	jz copy_ret
	jnz copy
too_long_copy:
   mov rax,0
	pop rbx
    ret
copy_ret:
  mov [rsi + rcx], bl  
  pop rbx
  ret
	






