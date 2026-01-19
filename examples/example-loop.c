#include "example-common.ic"

typedef struct vm_init_data {
	uint val;
	uint step;
} vm_init_data;

static vm_code * vm_make_code(void)
{
	static vm_instr loop[] = {
		{PUSHV,   -3,  0,   0,   NULL},
		{PUSHV,   -2,  0,   0,   NULL},
		{CALL,    3,   0,   0,   NULL},
		{POP,     2,   0,   0,   NULL},
		{FINISH,  0,   0,   0,   NULL},
		{PUSHA,   1,   0,   0,   NULL},
		{BGEQVV,  1,   -2,  3,   NULL},
		{ADDVVV,  1,   1,   -3,  NULL},
		{JMP,     -2,  0,   0,   NULL},
		{MOVRV,   0,   1,   0,   NULL},
		{POP,     1,   0,   0,   NULL},
		{RET,     0,   0,   0,   NULL},
	};

	static vm_code ret = {loop, sizeof(loop)/sizeof(*loop)};
	return &ret;
}

static void vm_init(const vm_init_data * data)
{
	memset(&vm, 0, sizeof(vm));
	vm.code = vm_make_code();
	vm.stack[0] = data->step;
	vm.stack[1] = data->val;
	vm.sp = vm.stack + 3;
	vm.bp = vm.sp;
	vm.ip = vm.code->code;
}

// <execute>
// <compiled>
static uint compiled(const vm_init_data * data)
{
	uint count = 0;
	uint val = data->val;
	uint step = data->step;
	while (count < val)
		count += step;
	return count;
}
// </compiled>

// <native>
#define NOIL __attribute__ ((noinline))

NOIL static void fn_loop(void)
{
	vm_i_pusha();
fn_loop_top:
	if (vm_i_bgeqvv())
		goto fn_loop_end;
	vm_i_addvvv();
	vm_i_jmp();
	goto fn_loop_top;
fn_loop_end:
	vm_i_movrv();
	vm_i_pop();
	vm_i_ret();
}
NOIL uint native(const vm_init_data * data)
{
	vm_init(data);

	vm_i_pushv();
	vm_i_pushv();
	vm_i_call();
	fn_loop();
	vm_i_pop();
	vm_i_finish();

	return (uint)vm.rret;
}
// </native>

int main(int argc, char * argv[])
{
	if (argc != 3)
		err_quit("Use: <prog> <val> <step>");

	uint val = 0;
	if (sscanf(argv[1], "%u", &val) != 1)
		err_quit("bad value");

	uint step = 0;
	if (sscanf(argv[2], "%u", &step) != 1)
		err_quit("bad step");

	vm_init_data data_ = {val, step};
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
