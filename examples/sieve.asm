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
#define _lim -2
.fn_enter:
pushv  bp[_lim]
call   fn_sieve
pop    1
finish
#undef _lim

#define _lim -2
#define _mem  1
#define _i    2
#define _j    3
#define _ret  4
.fn_sieve:
pusha  4
pushv  bp[_lim]
call   mem_alloc
pop    1
movvr  bp[_mem] rret
pushv  bp[_lim]
pushv  bp[_mem]
pushc  1
call   mem_init
pop    3
movvc  bp[_i] 2
.fn_sieve_loop_top:
bgrtvv bp[_i] bp[_lim] fn_sieve_loop_end
pushv  bp[_mem]
pushv  bp[_i]
pushv  bp[_lim]
call   fn_should_mark_non_primes
pop    3
beqrc  rret 0 fn_sieve_loop_bottom
pushv  bp[_mem]
pushv  bp[_i]
pushv  bp[_lim]
call   fn_mark_non_primes
pop    3
.fn_sieve_loop_bottom:
addvvc bp[_i] bp[_i] 1
jmp    fn_sieve_loop_top
.fn_sieve_loop_end:
pushv  bp[_mem]
pushv  bp[_lim]
call   fn_get_prime
movvr  bp[_ret] rret
pop    2
pushv  bp[_mem]
call   mem_free
pop    1
movrv  rret bp[_ret]
pop    4
ret
#undef _ret
#undef _j
#undef _i
#undef _mem
#undef _lim

#define _mem -4
#define _i   -3
#define _lim -2
.fn_should_mark_non_primes:
movrc rret 0
pushv bp[_i]
pushv bp[_lim]
call  fn_is_in_range
pop   2
beqrc rret 0 fn_should_mark_non_primes_ret
pushv bp[_mem]
pushv bp[_i]
call  fn_is_prime
pop   2
beqrc rret 0 fn_should_mark_non_primes_ret
movrc rret 1
.fn_should_mark_non_primes_ret:
ret
#undef _lim
#undef _i
#undef _mem

#define _i   -3
#define _lim -2
#define _prd  1
.fn_is_in_range:
pusha  1
movrc  rret 1
mulvvv bp[_prd] bp[_i] bp[_i]
bleqvv bp[_prd] bp[_lim] fn_is_in_range_ret
movrc  rret 0
.fn_is_in_range_ret:
pop    1
ret
#undef _prd
#undef _lim
#undef _i

#define _mem   -3
#define _i     -2
#define _is_pr  1
.fn_is_prime:
pusha  1
movrc  rret 1
ldbvvv bp[_is_pr] bp[_mem] bp[_i]
bneqvc bp[_is_pr] 0 fn_is_prime_ret
movrc  rret 0
.fn_is_prime_ret:
pop    1
ret
#undef _is_pr
#undef _i
#undef _mem

#define _mem   -4
#define _i     -3
#define _lim   -2
#define _j      1
.fn_mark_non_primes:
pusha  1
mulvvv bp[_j] bp[_i] bp[_i]
.fn_mark_non_primes_loop_top:
bgrtvv bp[_j] bp[_lim] fn_mark_non_primes_loop_end
stbvvc bp[_mem] bp[_j] 0
addvvv bp[_j] bp[_j] bp[_i]
jmp    fn_mark_non_primes_loop_top
.fn_mark_non_primes_loop_end:
pop    1
ret
#undef _j
#undef _lim
#undef _i
#undef _mem

#define _mem  -3
#define _lim  -2
#define _i     1
#define _is_pr 2
.fn_get_prime:
pusha  2
movrc  rret 0
movvv  bp[_i] bp[_lim]
.fn_get_prime_loop_top:
bltvc  bp[_i] 1 fn_get_prime_loop_end
ldbvvv bp[_is_pr] bp[_mem] bp[_i]
beqvc  bp[_is_pr] 0 fn_get_prime_loop_next
movrv  rret bp[_i]
jmp    fn_get_prime_loop_end
.fn_get_prime_loop_next:
subvvc bp[_i] bp[_i] 1
jmp    fn_get_prime_loop_top
.fn_get_prime_loop_end:
pop    2
ret
#undef _is_pr
#undef _i
#undef _lim
#undef _mem

#define _sz -2
#define _mem 1
.mem_alloc:
pusha  1
malloc bp[_mem] bp[_sz]
movrv  rret bp[_mem]
pop    1
ret
#undef _mem
#undef _sz

#define _mem -2
.mem_free:
freev bp[_mem]
ret
#undef _mem

#define _lim -4
#define _mem -3
#define _val -2
.mem_init:
memset bp[_mem] bp[_val] bp[_lim]
stbvcc bp[_mem] 0 0
stbvcc bp[_mem] 1 0
ret
#undef _val
#undef _mem
#undef _lim
