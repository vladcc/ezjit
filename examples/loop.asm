;
; the stack after a fn call
; ...
; first arg   | bp[-2]
; return addr | bp[-1]
; saved bp    | bp[0]  <-- sp
; first local | bp[1]
; ...
;

; the stack is set up by the system before executing
; the first instruction of fn_enter()
#define _step -3
#define _var  -2
.fn_enter:
pushv  bp[_step]
pushv  bp[_var]
call   fn_loop
pop    2
finish
#undef _var
#undef _step

#define _step -3
#define _var  -2
#define _count 1
.fn_loop:
pusha   1
.fn_loop_top:
bgeqvv  bp[_count] bp[_var]   fn_loop_end
addvvv  bp[_count] bp[_count] bp[_step]
jmp     fn_loop_top
.fn_loop_end:
movrv   rret bp[_count]
pop     1
ret
#undef _count
#undef _var
#undef _step
