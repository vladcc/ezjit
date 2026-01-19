#include "example-common.ic"

typedef struct vm_init_data {
	uint lim;
} vm_init_data;

static vm_code * vm_make_code(void)
{
	static vm_instr sieve[] = {
		{PUSHV,   -2,   0,   0,   NULL},
		{CALL,    3,    0,   0,   NULL},
		{POP,     1,    0,   0,   NULL},
		{FINISH,  0,    0,   0,   NULL},
		{PUSHA,   4,    0,   0,   NULL},
		{PUSHV,   -2,   0,   0,   NULL},
		{CALL,    81,   0,   0,   NULL},
		{POP,     1,    0,   0,   NULL},
		{MOVVR,   1,    0,   0,   NULL},
		{PUSHV,   -2,   0,   0,   NULL},
		{PUSHV,   1,    0,   0,   NULL},
		{PUSHC,   1,    0,   0,   NULL},
		{CALL,    82,   0,   0,   NULL},
		{POP,     3,    0,   0,   NULL},
		{MOVVC,   2,    2,   0,   NULL},
		{BGRTVV,  2,    -2,  14,  NULL},
		{PUSHV,   1,    0,   0,   NULL},
		{PUSHV,   2,    0,   0,   NULL},
		{PUSHV,   -2,   0,   0,   NULL},
		{CALL,    21,   0,   0,   NULL},
		{POP,     3,    0,   0,   NULL},
		{BEQRC,   0,    0,   6,   NULL},
		{PUSHV,   1,    0,   0,   NULL},
		{PUSHV,   2,    0,   0,   NULL},
		{PUSHV,   -2,   0,   0,   NULL},
		{CALL,    42,   0,   0,   NULL},
		{POP,     3,    0,   0,   NULL},
		{ADDVVC,  2,    2,   1,   NULL},
		{JMP,     -13,  0,   0,   NULL},
		{PUSHV,   1,    0,   0,   NULL},
		{PUSHV,   -2,   0,   0,   NULL},
		{CALL,    44,   0,   0,   NULL},
		{MOVVR,   4,    0,   0,   NULL},
		{POP,     2,    0,   0,   NULL},
		{PUSHV,   1,    0,   0,   NULL},
		{CALL,    57,   0,   0,   NULL},
		{POP,     1,    0,   0,   NULL},
		{MOVRV,   0,    4,   0,   NULL},
		{POP,     4,    0,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
		{MOVRC,   0,    0,   0,   NULL},
		{PUSHV,   -3,   0,   0,   NULL},
		{PUSHV,   -2,   0,   0,   NULL},
		{CALL,    10,   0,   0,   NULL},
		{POP,     2,    0,   0,   NULL},
		{BEQRC,   0,    0,   7,   NULL},
		{PUSHV,   -4,   0,   0,   NULL},
		{PUSHV,   -3,   0,   0,   NULL},
		{CALL,    12,   0,   0,   NULL},
		{POP,     2,    0,   0,   NULL},
		{BEQRC,   0,    0,   2,   NULL},
		{MOVRC,   0,    1,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
		{PUSHA,   1,    0,   0,   NULL},
		{MOVRC,   0,    1,   0,   NULL},
		{MULVVV,  1,    -3,  -3,  NULL},
		{BLEQVV,  1,    -2,  2,   NULL},
		{MOVRC,   0,    0,   0,   NULL},
		{POP,     1,    0,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
		{PUSHA,   1,    0,   0,   NULL},
		{MOVRC,   0,    1,   0,   NULL},
		{LDBVVV,  1,    -3,  -2,  NULL},
		{BNEQVC,  1,    0,   2,   NULL},
		{MOVRC,   0,    0,   0,   NULL},
		{POP,     1,    0,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
		{PUSHA,   1,    0,   0,   NULL},
		{MULVVV,  1,    -3,  -3,  NULL},
		{BGRTVV,  1,    -2,  4,   NULL},
		{STBVVC,  -4,   1,   0,   NULL},
		{ADDVVV,  1,    1,   -3,  NULL},
		{JMP,     -3,   0,   0,   NULL},
		{POP,     1,    0,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
		{PUSHA,   2,    0,   0,   NULL},
		{MOVRC,   0,    0,   0,   NULL},
		{MOVVV,   1,    -2,  0,   NULL},
		{BLTVC,   1,    1,   7,   NULL},
		{LDBVVV,  2,    -3,  1,   NULL},
		{BEQVC,   2,    0,   3,   NULL},
		{MOVRV,   0,    1,   0,   NULL},
		{JMP,     3,    0,   0,   NULL},
		{SUBVVC,  1,    1,   1,   NULL},
		{JMP,     -6,   0,   0,   NULL},
		{POP,     2,    0,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
		{PUSHA,   1,    0,   0,   NULL},
		{MALLOC,  1,    -2,  0,   NULL},
		{MOVRV,   0,    1,   0,   NULL},
		{POP,     1,    0,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
		{FREEV,   -2,   0,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
		{MEMSET,  -3,   -2,  -4,  NULL},
		{STBVCC,  -3,   0,   0,   NULL},
		{STBVCC,  -3,   1,   0,   NULL},
		{RET,     0,    0,   0,   NULL},
	};

	static vm_code ret = {sieve, sizeof(sieve)/sizeof(*sieve)};
	return &ret;
}
static void vm_init(const vm_init_data * data)
{
	memset(&vm, 0, sizeof(vm));
	vm.code = vm_make_code();
	vm.stack[0] = data->lim;
	vm.sp = vm.stack + 2;
	vm.bp = vm.sp;
	vm.ip = vm.code->code;
}

// <execute>
// <compiled>
static byte * c_sieve_mem_alloc(word sz)
{
	return (byte *)vm_malloc((size_t)sz);
}
static void c_sieve_mem_free(byte * mem)
{
	free(mem);
}
static void c_sieve_mem_init(byte val, byte * mem, word sz)
{
	vm_mem_init(val, mem, sz);
	mem[0] = 0;
	mem[1] = 0;
}
static bool c_sieve_is_in_range(word lim, word i)
{
	// makes sense only if typeof(i) < typeof(lim) but here for completeness
	return (i*i <= lim);
}
static bool c_sieve_is_prime(word i, byte * mem)
{
	return (mem[i] != 0);
}
static bool c_sieve_should_mark_non_primes(word lim, word i, byte * mem)
{
	return (c_sieve_is_in_range(lim, i) && c_sieve_is_prime(i, mem));
}
static void c_sieve_mark_non_primes(word lim, word i, byte * mem)
{
	for (word j = i*i; j <= lim; j += i)
		mem[j] = 0;
}

static word c_sieve_get_prime(word lim, byte * mem)
{
	word ret = 0;
	for (word i = lim; i > 0; --i)
	{
		if (mem[i] != 0)
		{
			ret = i;
			break;
		}
	}
	return ret;
}
static word c_sieve(word lim)
{
	byte * mem = c_sieve_mem_alloc(lim);
	c_sieve_mem_init(1, mem, lim);

	for (word i = 2; i <= lim; ++i)
	{
		if (c_sieve_should_mark_non_primes(lim, i, mem))
			c_sieve_mark_non_primes(lim, i, mem);
	}

	word prime = c_sieve_get_prime(lim, mem);
	c_sieve_mem_free(mem);
	return prime;
}
static word c_sieve_enter(word lim)
{
	return c_sieve(lim);
}

static uint compiled(const vm_init_data * data)
{
	return (uint)c_sieve_enter((word)data->lim);
}
// </compiled>

// <native>
#define NOIL __attribute__ ((noinline))

NOIL static void n_sieve_mem_alloc(void)
{
	vm_i_pusha();
	vm_i_malloc();
	vm_i_movrv();
	vm_i_pop();
	vm_i_ret();
}
NOIL static void n_sieve_mem_free(void)
{
	vm_i_freev();
	vm_i_ret();
}
NOIL static void n_sieve_mem_init(void)
{
	vm_i_memset();
	vm_i_stbvcc();
	vm_i_stbvcc();
	vm_i_ret();
}
NOIL static void n_sieve_fn_is_in_range(void)
{
	vm_i_pusha();
	vm_i_movrc();
	vm_i_mulvvv();
	if (vm_i_bleqvv())
		goto fn_is_in_range_ret;
	vm_i_movrc();
fn_is_in_range_ret:
	vm_i_pop();
	vm_i_ret();
}
NOIL static void n_sieve_fn_is_prime(void)
{
	vm_i_pusha();
	vm_i_movrc();
	vm_i_ldbvvv();
	if (vm_i_bneqvc())
		goto fn_is_prime_ret;
	vm_i_movrc();
fn_is_prime_ret:
	vm_i_pop();
	vm_i_ret();
}
NOIL static void n_sieve_fn_mark_non_primes(void)
{
	vm_i_pusha();
	vm_i_mulvvv();
fn_mark_non_primes_loop_top:
	if (vm_i_bgrtvv())
		goto fn_mark_non_primes_loop_end;
	vm_i_stbvvc();
	vm_i_addvvv();
	vm_i_jmp();
	goto fn_mark_non_primes_loop_top;
fn_mark_non_primes_loop_end:
	vm_i_pop();
	vm_i_ret();
}
NOIL static void n_sieve_fn_get_prime(void)
{
	vm_i_pusha();
	vm_i_movrc();
	vm_i_movvv();
fn_get_prime_loop_top:
	if (vm_i_bltvc())
		goto fn_get_prime_loop_end;
	vm_i_ldbvvv();
	if (vm_i_beqvc())
		goto fn_get_prime_loop_next;
	vm_i_movrv();
	vm_i_jmp();
	goto fn_get_prime_loop_end;
fn_get_prime_loop_next:
	vm_i_subvvc();
	vm_i_jmp();
	goto fn_get_prime_loop_top;
fn_get_prime_loop_end:
	vm_i_pop();
	vm_i_ret();
}
NOIL static void n_sieve_fn_should_mark_non_primes(void)
{
	vm_i_movrc();
	vm_i_pushv();
	vm_i_pushv();
	vm_i_call();
	n_sieve_fn_is_in_range();
	vm_i_pop();
	if (vm_i_beqrc())
		goto fn_should_mark_non_primes_ret;
	vm_i_pushv();
	vm_i_pushv();
	vm_i_call();
	n_sieve_fn_is_prime();
	vm_i_pop();
	if (vm_i_beqrc())
		goto fn_should_mark_non_primes_ret;
	vm_i_movrc();
fn_should_mark_non_primes_ret:
	vm_i_ret();
}
NOIL static void n_sieve_fn_sieve(void)
{
	vm_i_pusha();
	vm_i_pushv();
	vm_i_call();
	n_sieve_mem_alloc();
	vm_i_pop();
	vm_i_movvr();
	vm_i_pushv();
	vm_i_pushv();
	vm_i_pushc();
	vm_i_call();
	n_sieve_mem_init();
	vm_i_pop();
	vm_i_movvc();
fn_sieve_loop_top:
	if (vm_i_bgrtvv())
		goto fn_sieve_loop_end;
	vm_i_pushv();
	vm_i_pushv();
	vm_i_pushv();
	vm_i_call();
	n_sieve_fn_should_mark_non_primes();
	vm_i_pop();
	if (vm_i_beqrc())
		goto fn_sieve_loop_bottom;
	vm_i_pushv();
	vm_i_pushv();
	vm_i_pushv();
	vm_i_call();
	n_sieve_fn_mark_non_primes();
	vm_i_pop();
fn_sieve_loop_bottom:
	vm_i_addvvc();
	vm_i_jmp();
	goto fn_sieve_loop_top;
fn_sieve_loop_end:
	vm_i_pushv();
	vm_i_pushv();
	vm_i_call();
	n_sieve_fn_get_prime();
	vm_i_movvr();
	vm_i_pop();
	vm_i_pushv();
	vm_i_call();
	n_sieve_mem_free();
	vm_i_pop();
	vm_i_movrv();
	vm_i_pop();
	vm_i_ret();
}
NOIL static void n_sieve_main(void)
{
	vm_i_pushv();
	vm_i_call();
	n_sieve_fn_sieve();
	vm_i_pop();
	vm_i_finish();
}

NOIL static uint native(const vm_init_data * data)
{
	vm_init(data);
	n_sieve_main();
	return (uint)vm.rret;
}
// </native>

int main(int argc, char * argv[])
{
	if (argc != 2)
		err_quit("Use: <prog> <limit>");

	uint lim = 0;
	if (sscanf(argv[1], "%u", &lim) != 1)
		err_quit("bad value");

	vm_init_data data_ = {lim};
	vm_init_data * data = &data_;

	time_code(compiled,      data, "compiled  ");
	time_code(native,        data, "native    ");
	time_code(switch_loop,   data, "switch    ");
	time_code(threaded,      data, "threaded  ");
	time_code(dthreaded,     data, "dthreaded ");
	time_code(fptrs,         data, "fptrs     ");

	if (ezj_is_available())
	{
		time_code(ezj,     data,   "ezj       ");
		time_code(ezj_fc,  data,   "ezj_fc    ");
		time_code(ezj_fcj, data,   "ezj_fcj   ");
		time_code(ezj_ac,  data,   "ezj_ac    ");
		time_code(ezj_acj, data,   "ezj_acj   ");
	}

	return 0;
}
