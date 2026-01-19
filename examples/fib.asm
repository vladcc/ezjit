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
#define _n -2
.fn_enter:
pushv  bp[_n]
call   fn_fib
pop    1
finish
#undef _n

#define _n -2
#define _a  1
#define _b  2
.fn_fib:
bltvc   bp[_n] 2 fn_fib_base
pusha   2
subvvc  bp[_a] bp[_n] 1
pushv   bp[_a]
call    fn_fib
movvr   bp[_a] rret
pop     1
subvvc  bp[_b] bp[_n] 2
pushv   bp[_b]
call    fn_fib
movvr   bp[_b] rret
pop     1
addvvv  bp[_a] bp[_a] bp[_b]
movrv   rret   bp[_a]
pop     2
ret
.fn_fib_base:
movrv rret bp[_n]
ret
#undef _n
#undef _a
#undef _b
