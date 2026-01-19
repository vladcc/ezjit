#include "example-common.ic"

typedef struct vm_init_data {
	uint n;
} vm_init_data;

static vm_code * vm_make_code(void)
{
	static vm_instr fib[] = {
		{PUSHV,   -2,  0,   0,   NULL},
		{CALL,    3,   0,   0,   NULL},
		{POP,     1,   0,   0,   NULL},
		{FINISH,  0,   0,   0,   NULL},
		{BLTVC,   -2,  2,   16,  NULL},
		{PUSHA,   2,   0,   0,   NULL},
		{SUBVVC,  1,   -2,  1,   NULL},
		{PUSHV,   1,   0,   0,   NULL},
		{CALL,    -4,  0,   0,   NULL},
		{MOVVR,   1,   0,   0,   NULL},
		{POP,     1,   0,   0,   NULL},
		{SUBVVC,  2,   -2,  2,   NULL},
		{PUSHV,   2,   0,   0,   NULL},
		{CALL,    -9,  0,   0,   NULL},
		{MOVVR,   2,   0,   0,   NULL},
		{POP,     1,   0,   0,   NULL},
		{ADDVVV,  1,   1,   2,   NULL},
		{MOVRV,   0,   1,   0,   NULL},
		{POP,     2,   0,   0,   NULL},
		{RET,     0,   0,   0,   NULL},
		{MOVRV,   0,   -2,  0,   NULL},
		{RET,     0,   0,   0,   NULL},
	};

	static vm_code ret = {fib, sizeof(fib)/sizeof(*fib)};
	return &ret;
}
static void vm_init(const vm_init_data * data)
{
	memset(&vm, 0, sizeof(vm));
	vm.code = vm_make_code();
	vm.stack[0] = data->n;
	vm.sp = vm.stack + 2;
	vm.bp = vm.sp;
	vm.ip = vm.code->code;
}
// <execute>
// <compiled>
static uint fib(uint n)
{
	return (n < 2) ? n : fib(n-1) + fib(n-2);
}
static uint compiled(const vm_init_data * data)
{
	return fib(data->n);
}
// </compiled>
// <native>
#define NOIL __attribute__ ((noinline))

NOIL static void fn_fib(void)
{
	if (vm_i_bltvc())
		goto fn_fib_base;
	vm_i_pusha();
	vm_i_subvvc();
	vm_i_pushv();
	vm_i_call();
	fn_fib();
	vm_i_movvr();
	vm_i_pop();
	vm_i_subvvc();
	vm_i_pushv();
	vm_i_call();
	fn_fib();
	vm_i_movvr();
	vm_i_pop();
	vm_i_addvvv();
	vm_i_movrv();
	vm_i_pop();
	vm_i_ret();
	return;
fn_fib_base:
	vm_i_movrv();
	vm_i_ret();
	return;
}
NOIL static uint native(const vm_init_data * data)
{
	vm_init(data);

	vm_i_pushv();
	vm_i_call();
	fn_fib();
	vm_i_pop();
	vm_i_finish();

	return (uint)vm.rret;
}
// </native>
int main(int argc, char * argv[])
{
	if (argc != 2)
		err_quit("Use: <prog> <n>");

	uint n = 0;
	if (sscanf(argv[1], "%u", &n) != 1)
		err_quit("bad value");

	vm_init_data data_ = {n};
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
