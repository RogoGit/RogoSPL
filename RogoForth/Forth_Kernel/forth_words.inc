
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



