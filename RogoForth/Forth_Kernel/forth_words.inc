
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

native execute, "execute" ; ( xt -- ) execute word with this execution token on TOS
  pop r15
  mov w, r15 ; w - address of current instruction handler, will be defined in another file  
  jmp [r15] ; going by address stored in poped value


; ---------------- execution control -------------------

native exit, "exit" ; exit from colon word
  ret_pop pc ; write return stack head to program counter - next instruction
  jmp next

native branch, "branch" ; jump to a absolute location. Using it interactively is quasi-impossible; using it as a low-level primitive to implement if and similar constructs is much more convenient
  ; compile-only word
  mov pc, [pc]
  jmp next

native branch_0, "0branch" ; jump to a location if TOS = 0. Location is calculated in a similar way
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

; -------------------- initialization -------------------

const inbuf, input_buf
const mem, user_mem
const lw, lw
const state, state

const here, [here]
const in_fd, in_fd
const dp, dp

section .rodata
interpreter_stub:
    dq exec_token_lit, 0    
    dq exec_token_interpret_fd
    dq exec_token_bye

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

 
  
