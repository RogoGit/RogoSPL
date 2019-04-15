global _start

%include "lib.inc"
%include "forth_macro.inc"

%define pc r11 ; let it be r11
%define w r12 ; let it be r12
%define ret_stack r13

section .text

%include "forth_words.inc"    ;  words are here

section .bss

resq 1023              
rstack_start: resq 1   

user_mem: resq 65536 

section .data

lw: dq last_word       ; stores a pointer to the last word in dictionary
                       ; should be placed after all words are defined
dp: dq user_mem        ; current global data pointer
stack_start:  dq 0     ; stores a saved address of data stack
forth_init: dq token_selector 
 
section .text

_start: 
    mov ret_stack, rstack_start
    mov [stack_start], rsp
    mov pc, forth_init
    jmp next

next:                  
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]

