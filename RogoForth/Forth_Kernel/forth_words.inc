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



