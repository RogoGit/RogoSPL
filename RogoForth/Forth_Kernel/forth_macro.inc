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


