#ifndef EZJIT_H
#define EZJIT_H

// ezjit is a jit implementation for generating hardware function calls.
//
// The main idea is to substitute the indirect hardware jumps which will be
// necessary to implement the execution of VM code (i.e. the switch, computed
// gotos, or calls through function pointers) with ones which are direct,
// distinct (i.e. one branch target per address) and therefore more predictable.
//
// ezjit expects that the instructions in the VM instruction set are implemented
// in separate functions, one function per instruction. Then the user specifies
// the type of each instruction and the function implementing it, compiles,
// emits the hardware instructions, and executes them.
//
// ezjit will generate each instruction as a hardware function call to the
// implementing function and then either nothing, an unconditional jump, a
// conditional jump, a function call, a function return, or a return from the
// jitted code, depending on the instruction type.
//
// The branch magnitudes of ezjit's branching instructions vary from the
// smallest and nearest possible per architecture to absolute. If a branch is
// absolute, it will be indirect. These are best avoided since they'll be
// bigger and slower but exist so working hardware code can always be compiled
// regardless of the jump distances.
//
// Absolute function calls to the functions which implement the instructions can
// be avoided by reserving a large static buffer inside the binary because that
// will be close to the .text section.
// Absolute conditional/unconditional branches inside the jitted code will not
// happen except for a practically impossible pathological case - e.g. a jump
// to more than 2 GB away internal to the jitted code, assuming Intel.
//
// The resulting jitted code will execute as if the program was hard coded in C
// without any function inlining.
//
// Note that depending on hardware architecture, micro architecture, compiler,
// optimization level, VM design, the code patterns of the resulting binary,
// code alignment, the position of the Sun, the starts, the curse of the
// Pharaoh, and who knows what else, the ezjitted code may, or may not, be
// faster. In general, it will be faster than function pointers while offering
// the same extensibility. You'll have to measure to really know.

#include "impl/ezjit_priv_decl.h"

#include <string.h>
#include <stddef.h>
#include <stdbool.h>

// ezj_void_fn for instructions other than conditional jumps
// ezj_bool_fn for conditional jumps
typedef void (*ezj_void_fn)(void);
typedef bool (*ezj_bool_fn)(void);

typedef union ezj_fn_tbl_entry {
	ezj_void_fn void_fn;
	ezj_bool_fn bool_fn;
} ezj_fn_tbl_entry;

// use directly; built by the user
// max len is fixed at 512 because of riscv64
#define EZJ_FN_TBL_MAX_LEN 512
typedef struct ezj_fn_tbl {
	ezj_fn_tbl_entry * tbl;
	unsigned int len;
} ezj_fn_tbl;

// do not use directly; built by ezj_ir_*() calls
typedef struct ezj_ir_instr {
	unsigned int offs_in_code;
	unsigned int fn_tbl_entry;
	int jmp_offs_num_of_words;
	// declared branch magnitudes
	unsigned char bmag_decl_instr_fn_call;
	unsigned char bmag_decl_post_call_branch;
	// discovered branch magnitudes
	unsigned char bmag_disc_instr_fn_call;
	unsigned char bmag_disc_post_call_branch;
	// type and size
	unsigned char instr_ir_type;
	unsigned char call_target;
	unsigned char size_instr_fn_call;
	unsigned char size_post_call_branch;
} ezj_ir_instr;

// returned by ezj_err()
typedef enum ezj_err_code {
	EZJ_EC_NONE = 0,
	EZJ_EC_OFFS_OVERFLOW,
	EZJ_EC_BMAG_CONFLICT,

	EZJ_EC_END // sentinel
} ezj_err_code;

// do not use directly
// allocate and zero out instr_arr, pass to ezj_ir_init()
typedef struct ezj_ir {
	ezj_ir_instr * instr_arr;
	const char * err_str;
	unsigned int cap;
	unsigned int len;
	unsigned int err_instr;
	unsigned char err_code;
	bool has_abs_fn_call;
} ezj_ir;

#include "impl/ezjit_priv_inline.h"

// <public>
// <init>

// Returns, in bytes, the size necessary for a buffer of instr_count number of
// ir instructions.
static inline size_t ezj_ir_buff_size(unsigned int instr_count)
{
	return sizeof(ezj_ir_instr) * instr_count;
}

// Returns an initialized ezj_ir.
static inline ezj_ir ezj_ir_init(ezj_ir_instr * ir_buff, unsigned int cap)
{
	ezj_ir ir;
	// C/C++ portable 0 initialization
	memset(&ir, 0, sizeof(ir));
	ir.instr_arr = ir_buff;
	ir.cap = cap;
	return ir;
}
// </init>

// <instruction-ir>

// Creates a non-branching ir instruction implemented by the function pointed to
// by fn_tbl[fn_tbl_entry].
// fn_tbl[fn_tbl_entry] must be of type ezj_void_fn.
// fn_tbl is defined elsewhere by the user.
// Returns a pointer to the created ir instruction.
static inline ezj_ir_instr * ezj_ir_make_instr(
	ezj_ir * ir,
	unsigned int fn_tbl_entry
)
{
	return pezj_ir_make_instr(ir, fn_tbl_entry, PEZJ_IR_INSTR, PEZJ_BMAG_NONE,
		0);
}

// Creates an unconditional ir jump instructions.
// fn_tbl[fn_tbl_entry] must be of type ezj_void_fn.
// jmp_offs_num_of_words is the number of ir instructions to jump over in either
// direction.
// Returns a pointer to the created ir instruction.
static inline ezj_ir_instr * ezj_ir_make_ujmp(
	ezj_ir * ir,
	unsigned int fn_tbl_entry,
	int jmp_offs_num_of_words
)
{
	return pezj_ir_make_instr(ir, fn_tbl_entry, PEZJ_IR_UJMP, PEZJ_BMAG_NONE,
		jmp_offs_num_of_words);
}

// Creates a conditional ir jump instruction.
// fn_tbl[fn_tbl_entry] must be of type ezj_bool_fn.
// The bool result indicates whether the jump is taken or not.
// jmp_offs_num_of_words is the number of ir instructions to jump over in either
// direction.
// Returns a pointer to the created ir instruction.
static inline ezj_ir_instr * ezj_ir_make_cjmp(
	ezj_ir * ir,
	unsigned int fn_tbl_entry,
	int jmp_offs_num_of_words
)
{
	return pezj_ir_make_instr(ir, fn_tbl_entry, PEZJ_IR_CJMP, PEZJ_BMAG_NONE,
		jmp_offs_num_of_words);
}

// Creates a function call ir instruction.
// fn_tbl[fn_tbl_entry] must be of type ezj_void_fn.
// jmp_offs_num_of_words is the number of ir instructions to jump over in either
// direction.
// Returns a pointer to the created instruction.
static inline ezj_ir_instr * ezj_ir_make_call(
	ezj_ir * ir,
	unsigned int fn_tbl_entry,
	int jmp_offs_num_of_words
)
{
	ezj_ir_instr * instr =
		pezj_ir_make_instr(ir, fn_tbl_entry, PEZJ_IR_CALL, PEZJ_BMAG_NONE,
			jmp_offs_num_of_words);
	pezj_instr_mark_call_target(instr + jmp_offs_num_of_words);
	return instr;
}

// Creates a return ir instruction.
// fn_tbl[fn_tbl_entry] must be of type ezj_void_fn.
// Returns a pointer to the created instruction.
static inline ezj_ir_instr * ezj_ir_make_ret(
	ezj_ir * ir,
	unsigned int fn_tbl_entry
)
{
	return pezj_ir_make_instr(ir, fn_tbl_entry, PEZJ_IR_RET, PEZJ_BMAG_NONE, 0);
}

// Creates a finish ir instruction.
// fn_tbl[fn_tbl_entry] must be of type ezj_void_fn.
// This instruction returns from the jit execution, therefore it must be
// executed last.
// Returns a pointer to the created instruction.
static inline ezj_ir_instr * ezj_ir_make_finish(
	ezj_ir * ir,
	unsigned int fn_tbl_entry
)
{
	return pezj_ir_make_instr(ir, fn_tbl_entry, PEZJ_IR_FINISH, PEZJ_BMAG_NONE,
		0);
}
// </instruction-ir>

// <branch-magnitude>

// Sets the branch magnitude of the function call to the function which
// implements the ir instruction to default.
// Default chooses the smallest function call possible. This is decided by how
// far away the function to call is in memory.
static inline void ezj_ir_set_bmag_default(ezj_ir_instr * instr)
{
	pezj_instr_set_bmag_decl_instr_fn_call(instr, PEZJ_BMAG_NONE);
}

// Sets the branch magnitude of the function call to the function which
// implements the ir instruction to near.
// Near is the smallest and fastest function call. It usually involves a single
// hardware instruction. If it cannot work because the function it calls is too
// far away in memory, compilation fails.
static inline void ezj_ir_set_bmag_near(ezj_ir_instr * instr)
{
	pezj_instr_set_bmag_decl_instr_fn_call(instr, PEZJ_BMAG_NEAR);
}

// Sets the branch magnitude of the function call to the function which
// implements the ir instruction to far.
// Far is the medium sized function call. On some architectures it's the same as
// near (e.g. Intel), on others it could be more than one hardware instruction,
// so it could be slower than near but has more range. It never involves a
// memory read. If it cannot work, compilation fails.
static inline void ezj_ir_set_bmag_far(ezj_ir_instr * instr)
{
	pezj_instr_set_bmag_decl_instr_fn_call(instr, PEZJ_BMAG_FAR);
}

// Sets the branch magnitude of the function call to the function which
// implements the ir instruction to absolute.
// An absolute function call will always work but will be slower since it
// involves an indirect call to a pointer loaded from fn_tbl.
static inline void ezj_ir_set_bmag_abs(ezj_ir_instr * instr)
{
	pezj_instr_set_bmag_decl_instr_fn_call(instr, PEZJ_BMAG_ABS);
}
// </branch-magnitude>

// <compiler>

// Returns true if ezjit is available, false otherwise.
// ezjit is not available if it's compiled on an unsupported architecture.
bool ezj_is_available(void);

// Returns, in bytes, the size the binary buffer (the buffer which will hold the
// actual jitted code) needs to be. This size is not exact and it's expected to
// always be enough. There exists a pathological case in which it wouldn't be
// enough, which should not occur in practice. It is possible to know how many
// bytes of buffer the compilation will need more precisely, but only after
// supplying the address of the buffer and compiling. Hence this approximate
// value, which is somewhat larger than necessary, should be good enough.
// If the compilation wants more space, double it and try again.
size_t ezj_code_buff_size(unsigned int ir_instr_count);

// Compiles the ir instructions. "Compiles" here means making them ready to be
// emitted. E.g. finding sizes, checking ranges and so on.
// Returns the number of bytes needed to contain the emitted instructions. I.e.
// the size 'code' needs to be.
// Returns 0 on error.
size_t ezj_compile(
	ezj_ir * ir,
	const ezj_fn_tbl * fn_tbl,
	const unsigned char * code
);

// Emits the hardware instructions which implement the ir.
void ezj_emit(
	const ezj_ir * ir,
	const ezj_fn_tbl * fn_tbl,
	unsigned char * code
);

// If ezj_compile() returned 0, this function provides more information about
// the error.
// Returns the error code.
// On return out_err_instr_num holds the index of the offending instruction in
// the ir.
// On return out_err_msg holds a pointer to a string describing the error.
ezj_err_code ezj_err(
	const ezj_ir * ir,
	unsigned int * out_err_instr_num,
	const char ** out_err_msg
);
// </compiler>
// </public>

// <version>

// Returns the major version number.
unsigned int ezj_major_ver(void);

// Returns the minor version number.
unsigned int ezj_minor_ver(void);
// </version>
#endif
