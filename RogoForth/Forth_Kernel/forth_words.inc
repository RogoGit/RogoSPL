%include "lib.inc"
%include "forth_macro.inc"

; ----------- basic stack operations -------------

native swap,"swap" ; (ab -- ba)
    pop rax
    pop rdx
    push rax
    push rdx
    jmp next

native drop,"drop" ; (a --)
    ; pop rax
    ; may be we do not have to save value in register before deleting, so
    add rsp, 8
    jmp next

native dup, "dup" ; (a -- aa)
    push qword [rsp] ; pushing value from memory with address from top of the stack
    jmp next
	

; ------------ ariphmetic operations ---------------

native plus, "+" ;( x y -- [x+y])
    pop r15 ; destroy top with saving to r15
    add [rsp], r15 ; add r15 to new top - value in memory cell referenced by rsp
    jmp next

native minus, "-" ;( x y -- [x-y])
    pop r15
    sub [rsp], r15 ; same as plus but sub instead of add
    jmp next

native multiple, "*" ;( x y -- [x*y])
    pop rax
    pop r15
    imul r15 ; signed multiple
    push rax 
    jmp next

native divide, "/" ;( x y -- [x/y])
    pop r15
    pop rax
    xor edx,edx ; here will be remainder
    idiv r15 ; signed division rax/r15 -> rax
    push rax ; push result
    jmp next

native mod, "%" ;( x y -- [x mod y])
    pop r15
    pop rax
    xor edx,edx ; here will be remainder
    idiv r15 ; signed division rax/r15 -> rax
    push rdx ; push remainder
    jmp next


; -------------- comparation operations ------------

native less, "<" ;( x y -- [x<y])
    pop r15 ; y
    pop r14 ; x
    cmp r15,r14
    jle .less
    push 1 ; condition is true
    jmp next
    .less:
        push 0 ; condition is false
        jmp next

native more, ">" ;( x y -- [x>y])
    pop r15 ; y
    pop r14 ; x
    cmp r14,r15
    jg .more
    push 0 ; condition is false
    jmp next
    .more:
        push 1 ; condition is true
        jmp next

native equals, "=" ;( x y -- [x=y])
    pop r15
    pop r14
    cmp r14,r15
    jne .neq
    push 1 ; condition is true
    jmp next
    .neq:
        push 0 ; condition is false
        jmp next


; ------------- logical operations ----------------

native not, "not" ;( a -- a' ) a' = 0 if a != 0 a' = 1 if a == 0
    pop r15
    test r15,r15
    jz .zero
    push 0 
    jmp next
    .zero:
        push 1
        jmp next

native and, "and" ;( a b -- [a&b])
    pop r15
    and [rsp], r15 ; bitwise and r15 with new top - value in memory cell referenced by rsp
    jmp next

native or, "or" ;( a b -- [a|b])
    pop r15
    or [rsp], r15 ; bitwise or r15 with new top - value in memory cell referenced by rsp
    jmp next

native land, "land" ;( a b -- [a&&b])
    pop r15
    pop r14
    test r15,r15
    jz .no
    push r14
    jmp next
    .no:
        push r15
        jmp next

native lor, "lor" ;( a b -- [a||b])
    pop r15
    pop r14
    test r15,r15
    jnz .yes
    push r14
    jmp next
    .yes:
        push r15
        jmp next


; -------------- return stack operations ------------

native to_ret, ">r" ; pop from data stack into return stack
    pop r15
    ret_push r15 ; macro from forth_macro.inc
    jmp next

native from_ret, "r>" ; push from return stack into data stack
    ret_pop r15 ; macro from forth_macro.inc
    push r15
    jmp next

native fetch_ret, "r@" ; non-destructive copy from the top of return stack to the top of data stack
    push qword [ret_stack] ; ret_stack will be defined in another file
    jmp next


; --------------- new words creation -----------------

native create_word, "create" ; adding new word to dictionary
    mov rcx, [last_word]
    mov rsi, [here]
    mov [rsi], rcx
    mov [last_word], rsi
    add rsi, 8
    mov byte [rsi], 0
    inc rsi
    pop rdi
    push rsi
    call string_copy
    pop rsi
    push rsi
    mov rdi, rsi
    call string_length
    pop rsi
    add rsi, rax
    inc rsi
    pop rax
    mov [rsi], al
    inc rsi
    mov [here], rsi
    jmp next


; --------------- words definition -------------------

colon colon, ":" ; read and define word
    .again:
    dq exec_token_inbuf, exec_token_word
    branch0 .again
    dq exec_token_lit, 0, exec_token_inbuf, exec_token_create
    dq exec_token_lit, 1, exec_token_state, exec_token_write
    dq exec_token_lit, impl_docol, exec_token_comma
    dq exec_token_exit

colon semicolon, ";", 1 ; end definition
    dq exec_token_lit, 0, exec_token_state, exec_token_write
    dq exec_token_lit, exec_token_exit, exec_token_comma
    dq exec_token_exit

native comma, "," ; ( x -- ) add x to word being defined
    mov rax, [here]
    pop qword [rax]
    add qword [here], 8
    jmp next

native char_comma, "c," ; ( c -- ) add a byte to the word being defined
    mov rax, [here]
    pop rdx
    mov [rax], dl
    add qword[here], 1
    jmp next
	
	

; ------------- working with memory ------------------

native fetch, "@" ;( addr -- value ) fetch value from memory
    pop r15
    push qword [r15] ; putting to stack head value by address from stack head
    jmp next


native write, "!" ;( val addr -- ) store value by address
    pop r15 ; addr
    pop r14 ; val
    mov [r15], r14 ; write value 'val' by address 'addr'
    jmp next

native write_byte, "c!" ;( char addr -- ) store one byte by address
    pop r15 ; addr
    pop r14 ; char
    mov [r15], r14b ; writing only lower byte
    jmp next

native fetch_byte, "c@" ;( addr -- char ) read one byte starting at addr
    pop r15
    movzx r15, byte [r15] ; in r15 - lower byte, the rest bytes are zero
    push r15
    jmp next

native word, "word" ; ( addr -- len ) read word and store it starting at address + word length pushed into stack
    pop rdi
    call read_word
    push rdx
    jmp next
	
native number, "number" ; ( str -- len ) parse string to number
    pop rdi
    call parse_int
    push rax
    push rdx
    jmp next	

native find, "find" ; ( str -- header_addr )
    mov rsi, [lw]
    mov rax, [state]
    test rax, rax
    jz .loop
    mov rsi, [rsi]
    .loop:
        mov rdi, [rsp]
        push rsi
        add rsi, 9
        call string_equals
        pop rsi
        test rax, rax
        jnz .found
        mov rsi, [rsi]
        test rsi, rsi
        jnz .loop
    .not_found:
        mov qword [rsp], 0
        jmp next
    .found:
        mov [rsp], rsi
        jmp next
		
native cfa, "cfa" ; ( word_addr -- exec_token ) converting to execution token
    pop rsi
    add rsi, 9
    .loop:
        mov al, [rsi]
        test al, al
        jz .end
        inc rsi
        jmp .loop
    .end:
        add rsi, 2
        push rsi
        jmp next		

native execute, "execute" ; ( xt -- ) execute word with this execution token on TOS
    pop r15
    mov w, r15 ; w - address of current instruction handler, will be defined in another file  
    jmp [r15] ; going by address stored in poped value


; ----------------- working with stack -----------------

native dot, "." ; ( a -- ) drop element from stack and send it to stdout
    pop rdi
    call print_int
    jmp next

native show_stack, ".S" ; show stack content
    mov rcx, rsp
    .loop:
        cmp rcx, [stack_base]
        jae next
        mov rdi, [rcx]
        push rcx
        call print_int
        call print_newline
        pop rcx
        add rcx, 8
        jmp .loop
		
		
; ----------------- printing string -------------------

native emit, "emit" ; output character to stdout
    pop rdi
    call print_char
    jmp next

colon count, "count" ; null-terminated string length
     dq exec_token_dup
 	.loop:
    	dq exec_token_dup
     	dq exec_token_fetch_byte
     	branch0 .exit
     	dq exec_token_lit, 1, exec_token_plus
     	branch .loop
 	.exit:
     	dq exec_token_swap, exec_token_minus, exec_token_exit

colon printc, "printc" ; prints a certain amount of symbols from string
    dq exec_token_to_ret
	.loop:
    	dq exec_token_fetch_ret
    	branch0 .exit
    	dq exec_token_dup, exec_token_fetch_byte, exec_token_emit
    	dq exec_token_lit, 1, exec_token_plus
    	dq exec_token_from_ret, exec_token_lit, 1, exec_token_minus, exec_token_to_ret
    	branch .loop
    .exit:
    	dq exec_token_from_ret, exec_token_drop, exec_token_drop, exec_token_exit

colon prints, "prints" ; print a null-terminated string
     dq exec_token_dup, exec_token_count, exec_token_printc, exec_token_exit


; ---------------- execution control -------------------

native exit, "exit" ; exit from colon word
    ret_pop pc ; write return stack head to program counter - next instruction
    jmp next

native branch, "branch" ; jump to a absolute location. Using it interactively is quasi-impossible; using it as a low-level primitive to implement if and similar constructs is much more convenient
    ; compile-only word
    mov pc, [pc]
    jmp next

native branch0, "0branch" ; jump to a location if TOS = 0. Location is calculated in a similar way
    ; compile-only word
    pop r15
    test r15, r15
    jnz .skip ; not 0 - skipping
    mov pc, [pc] ; ok - doing like in branch
    jmp next
    .skip:
        add pc, 8 ; adding to program counter - going to next instruction
        jmp next
	
colon bye, "bye" ; exiting forth
    dq exec_token_lit, 60
    dq exec_token_lit, 0
    dq exec_token_lit, 0
    dq exec_token_lit, 0
    dq exec_token_lit, 0
    dq exec_token_lit, 0
    dq exec_token_lit, 0
    dq exec_token_syscall	

native docol, "docol" ; this is the implementation of any colon-word
    ret_push pc ; program counter -> to return stack
    add w,8
    mov pc, w
    jmp next

; ------------------ forth syscall --------------------- 

native syscall, "syscall" ; ( call_num a1 a2 a3 a4 a5 a6 -- new_rax new_rdx) 
  ; executes syscall The following registers store arguments (according to ABI) rdi , rsi , rdx , r10 , r8 and r9
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next

; ---------------- forth special words ----------------

native lit, "lit" ; push a value immediately following this XT
  push qword [pc]
  add pc, 8
  jmp next

; native forth-dp, "forth-dp"  DP address - link to first free memory cell for user data - global data pointer

;native forth-sp, "forth-sp" ; same as push rsp
;  push rsp
;  jmp next

;native forth-stack-base, "forth-stack-base" ; push to stack [stack_start]
  ; stack_start stores a saved address of data stack
;  push [stack_start]
 ; jmp next

; -------------------- initialization -------------------

const inbuf, input_buf
const mem, user_mem
const lw, lw
const state, state

const here, [here]
const in_fd, in_fd
const dp, dp
	
section .data
    stack_base: dq 0

native init, "init"
    mov qword [state], 0
    mov ret_stack, ret_stack_start
    mov pc, interpreter_stub
    cmp qword [stack_base], 0
    je  .first
    mov rsp, [stack_base]
    jmp next
    .first:
    mov [stack_base], rsp
    jmp next
	
; ------------------ interpretation --------------------

section .rodata
interpreter_stub:
    dq exec_token_lit, 0    
    dq exec_token_interpret_fd
    dq exec_token_bye
	
colon interpret_fd, "interpret-fd"
    dq exec_token_lit, in_fd, exec_token_fetch, exec_token_to_ret
    dq exec_token_lit, in_fd
    dq exec_token_write
    .loop:
        dq exec_token_inbuf, exec_token_word
        branch0 .end_of_input
        dq exec_token_inbuf
        dq exec_token_interpret
        branch .loop
    .end_of_input:
        dq exec_token_from_ret
        dq exec_token_lit, in_fd, exec_token_write
        dq exec_token_exit

colon interpret, "interpret"
	.start:
        dq exec_token_dup, exec_token_find ; find word
    	dq exec_token_dup
    	branch0 .number  ; if not found parse as number
    	dq exec_token_cfa
    	dq exec_token_state, exec_token_fetch ; compiling
    	branch0 .interpr
		; immediate
    	dq exec_token_dup
    	dq exec_token_lit, 1, exec_token_minus, exec_token_fetch_byte, exec_token_not
    	branch0 .interpr
    	; compile
    	dq exec_token_swap, exec_token_drop
    	dq exec_token_comma
    	dq exec_token_exit
	.interpr:
    	dq exec_token_swap, exec_token_drop
    	dq exec_token_execute
    	dq exec_token_exit
    .number:
    	dq exec_token_drop, exec_token_dup
    	dq exec_token_number 
    	branch0 .not_found
    	dq exec_token_swap, exec_token_drop
    	dq exec_token_state, exec_token_fetch  ; finish interpr
    	branch0 .exit
        ; when compiling, check further
    	dq exec_token_here, exec_token_lit, 8, exec_token_minus, exec_token_fetch
    	dq exec_token_dup
    	dq exec_token_lit, exec_token_branch0
    	dq exec_token_equals, exec_token_not
    	branch0 .branch_state
		dq exec_token_dup
    	dq exec_token_lit, exec_token_branch
   		dq exec_token_equals, exec_token_not
    	branch0 .branch_state
		dq exec_token_lit, exec_token_lit, exec_token_comma
	.branch_state:
    	dq exec_token_drop
    	dq exec_token_comma
    	dq exec_token_exit
	.not_found: 
    	dq exec_token_drop
    	dq exec_token_prints
    	dq exec_token_lit, not_found_msg, exec_token_prints
	.exit:
    	dq exec_token_exit
	  
