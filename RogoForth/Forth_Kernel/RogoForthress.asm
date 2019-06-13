global _start

%include "lib.inc"

%define pc r11 
%define w r12
%define ret_stack r13

section .text

%include "forth_words.inc"    ;  words are here

section .rodata
not_found_msg: db ": no such word", 10, 0

section .data

lw: dq last_word       ; stores a pointer to the last word in dictionary
                       ; should be placed after all words are defined
dp: dq user_mem        ; current global data pointer
here: dq user_dict
;stack_start:  dq 0     ; stores a saved address of data stack
;forth_init: dq token_selector 

section .bss

resq 1023              
ret_stack_start: resq 1   

input_buf: resb 1024   ; buffer to read textual words into
user_dict:  resq 65536 ; data for words
user_mem: resq 65536   ; global data for user
state: resq 1          ; changes to 1 if compiling, 0 by default

section .text

_start: 
	jmp impl_init
 ;   mov ret_stack, ret_stack_start
  ;  mov [stack_start], rsp
   ; mov pc, forth_init
    ;jmp next

next:                  
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]