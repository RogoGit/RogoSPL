; -------------- working with native words ------------

%define last_word 0 ; last word

%macro native 3

   section .data
   word_ %+ %1 : dq last_word ; indentificator part for label name
   db 0 
   db %2, 0 ; word name
   db %3 ; flags
   
   %define last_word word_%+ %1 ; updating last word reference
           exec_token_ %+ %1 : dq impl_ %+ %1 ; link to implementation

   section .text  ; implementation starts here
   impl_ %+ %1:
   
%endmacro

%macro native 2
   native %1,%2,0 ; native macro with no flags
%endmacro


; ----------- working with return stack -------------

; ret_stack will be defined in another file

%macro ret_push 1
   sub ret_stack, 8 ; making memory address lesser - getting ready for one new stack element
   mov qword [ret_stack], %1 ; writing value from macro argument by address in ret_stack 
%endmacro

%macro ret_pop 1
   mov %1, qword [ret_stack]
   add ret_stack, 8 ; removing top element of return stack
%endmacro


; ------------- working with colon words -------------

%macro colon 3
section .data 
   word_ %+ %1 : dq last_word
   %define last_word word_ %+ %1 
   db 0  ; to mark the name start
   str_ %+ %1:
   db %2, 0
   db %3
    
   exec_token_ %+ %1 : dq impl_docol
%endmacro

%macro colon 2
colon %1, %2, 0 ; colon macro with no flags
%endmacro

