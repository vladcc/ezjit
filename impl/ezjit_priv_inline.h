#ifndef EZJIT_PRIV_INLINE_H
#define EZJIT_PRIV_INLINE_H
// <fn_tbl>
static inline const ezj_fn_tbl_entry * pezj_fn_tbl_get_tbl_addr(
	const ezj_fn_tbl * fn_tbl
)
{
	return (fn_tbl->tbl);
}

static inline unsigned int pezj_fn_tbl_get_len(const ezj_fn_tbl * fn_tbl)
{
	return (fn_tbl->len);
}

static inline size_t pezj_fn_tbl_get_size_in_bytes(const ezj_fn_tbl * fn_tbl)
{
	return pezj_fn_tbl_get_len(fn_tbl) * sizeof(*(fn_tbl->tbl));
}

static inline void * pezj_fn_tbl_get_entry(
	const ezj_fn_tbl * fn_tbl,
	size_t ind
)
{
	return (void *)pezj_fn_tbl_get_tbl_addr(fn_tbl)[ind].void_fn;
}
// </fn_tbl>
// <instr>
// <offs_in_code>
static inline void pezj_instr_set_offs_in_code(
	ezj_ir_instr * instr,
	unsigned int offs_in_code
)
{
	instr->offs_in_code = offs_in_code;
}
static inline unsigned int pezj_instr_get_offs_in_code(
	const ezj_ir_instr * instr
)
{
	return instr->offs_in_code;
}
static inline void pezj_instr_add_to_offs_in_code(ezj_ir_instr * instr, int val)
{
	pezj_instr_set_offs_in_code(
		instr,
		pezj_instr_get_offs_in_code(instr) + (unsigned int)val
	);
}
// </offs_in_code>

// <fn_tbl_entry>
static inline void pezj_instr_set_fn_tbl_entry(
	ezj_ir_instr * instr,
	unsigned int fn_tbl_entry
)
{
	instr->fn_tbl_entry = fn_tbl_entry;
}
static inline unsigned int pezj_instr_get_fn_tbl_entry(
	const ezj_ir_instr * instr
)
{
	return instr->fn_tbl_entry;
}
// </fn_tbl_entry>

// <jmp_offs_num_of_words>
static inline void pezj_instr_set_jmp_offs(
	ezj_ir_instr * instr,
	int num_of_instr_words
)
{
	instr->jmp_offs_num_of_words = num_of_instr_words;
}
static inline int pezj_instr_get_jmp_offs(const ezj_ir_instr * instr)
{
	return instr->jmp_offs_num_of_words;
}
// </jmp_offs_num_of_words>

// <bmags>
static inline void pezj_instr_set_bmag_decl_instr_fn_call(
	ezj_ir_instr * instr,
	pezj_bmag bmag
)
{
	instr->bmag_decl_instr_fn_call = (unsigned char)bmag;
}
static inline unsigned char pezj_instr_get_bmag_decl_instr_fn_call(
	const ezj_ir_instr * instr
)
{
	return instr->bmag_decl_instr_fn_call;
}

static inline void pezj_instr_set_bmag_decl_post_call_branch(
	ezj_ir_instr * instr,
	pezj_bmag bmag
)
{
	instr->bmag_decl_post_call_branch = (unsigned char)bmag;
}
static inline unsigned char pezj_instr_get_bmag_decl_post_call_branch(
	const ezj_ir_instr * instr
)
{
	return instr->bmag_decl_post_call_branch;
}

static inline void pezj_instr_set_bmag_disc_instr_fn_call(
	ezj_ir_instr * instr,
	pezj_bmag bmag
)
{
	instr->bmag_disc_instr_fn_call = (unsigned char)bmag;
}
static inline unsigned char pezj_instr_get_bmag_disc_instr_fn_call(
	const ezj_ir_instr * instr
)
{
	return instr->bmag_disc_instr_fn_call;
}

static inline void pezj_instr_set_bmag_disc_post_call_branch(
	ezj_ir_instr * instr,
	pezj_bmag bmag
)
{
	instr->bmag_disc_post_call_branch = (unsigned char)bmag;
}
static inline unsigned char pezj_instr_get_bmag_disc_post_call_branch(
	const ezj_ir_instr * instr
)
{
	return instr->bmag_disc_post_call_branch;
}
// </bmags>

// <instr_ir_type>
static inline void pezj_instr_set_ir_type(
	ezj_ir_instr * instr,
	pezj_ir_type type
)
{
	instr->instr_ir_type = (unsigned char)type;
}
static inline unsigned char pezj_instr_get_ir_type(const ezj_ir_instr * instr)
{
	return instr->instr_ir_type;
}
// </instr_ir_type>

// <call_target>
static inline unsigned char pezj_instr_set_call_target_size(
	ezj_ir_instr * instr,
	unsigned char size
)
{
	return instr->call_target = size;
}
static inline unsigned char pezj_instr_get_call_target_size(
	const ezj_ir_instr * instr
)
{
	return instr->call_target;
}
static inline void pezj_instr_mark_call_target(ezj_ir_instr * instr)
{
	pezj_instr_set_call_target_size(instr, 1);
}
static inline bool pezj_instr_is_call_target(const ezj_ir_instr * instr)
{
	return (pezj_instr_get_call_target_size(instr) != 0);
}
// </call_target>

// <size_instr_fn_call>
static inline void pezj_instr_set_size_instr_fn_call(
	ezj_ir_instr * instr,
	unsigned char size
)
{
	instr->size_instr_fn_call = size;
}
static inline unsigned char pezj_instr_get_size_instr_fn_call(
	const ezj_ir_instr * instr
)
{
	return instr->size_instr_fn_call;
}
// </size_instr_fn_call>

// <size_post_call_branch>
static inline void pezj_instr_set_size_post_call_branch(
	ezj_ir_instr * instr,
	unsigned char size
)
{
	instr->size_post_call_branch = size;
}
static inline unsigned char pezj_instr_get_size_post_call_branch(
	const ezj_ir_instr * instr
)
{
	return instr->size_post_call_branch;
}
// </size_post_call_branch>

static inline unsigned int pezj_instr_get_offs_of_target(
	const ezj_ir_instr * instr
)
{
	return pezj_instr_get_offs_in_code(instr + pezj_instr_get_jmp_offs(instr));
}

static inline unsigned int pezj_instr_get_half_size(const ezj_ir_instr * instr)
{
	return (pezj_instr_get_call_target_size(instr)
		+ pezj_instr_get_size_instr_fn_call(instr));
}
static inline unsigned int pezj_instr_get_full_size(const ezj_ir_instr * instr)
{

	return (pezj_instr_get_half_size(instr) +
		pezj_instr_get_size_post_call_branch(instr));
}

static inline unsigned int pezj_instr_get_fn_call_offset(
	const ezj_ir_instr * instr
)
{
	return pezj_instr_get_offs_in_code(instr)
		+ pezj_instr_get_call_target_size(instr);
}

static inline unsigned int pezj_instr_get_post_branch_offset(
	const ezj_ir_instr * instr
)
{
	return pezj_instr_get_offs_in_code(instr) + pezj_instr_get_half_size(instr);
}

static inline bool pezj_instr_has_post_branch(const ezj_ir_instr * instr)
{
// 0111 1100 = 0x7C
#define HAS_POST_BRANCH_BSET 0x7C
	return ((HAS_POST_BRANCH_BSET >> pezj_instr_get_ir_type(instr)) & 0x01);
#undef HAS_POST_BRANCH_BSET
}

// </instr>

// <ir>
static inline ezj_ir_instr * pezj_ir_get_instr_arr(const ezj_ir * ir)
{
	return ir->instr_arr;
}

static inline unsigned int pezj_ir_get_len(const ezj_ir * ir)
{
	return ir->len;
}

static inline unsigned int pezj_ir_get_cap(const ezj_ir * ir)
{
	return ir->cap;
}

static inline unsigned int pezj_ir_next_pos(ezj_ir * ir)
{
	return ir->len++;
}

static inline void pezj_ir_set_has_abs_fn_call(ezj_ir * ir)
{
	ir->has_abs_fn_call = true;
}
static inline bool pezj_ir_has_abs_fn_call(const ezj_ir * ir)
{
	return ir->has_abs_fn_call;
}

static inline ezj_ir_instr * pezj_ir_next_instr(ezj_ir * ir)
{
	return (pezj_ir_get_instr_arr(ir) + pezj_ir_next_pos(ir));
}

static inline ezj_ir_instr * pezj_ir_make_instr(
	ezj_ir * ir,
	unsigned int fn_tbl_entry,
	pezj_ir_type instr_type,
	pezj_bmag bmag_type,
	int jmp_offs_num_of_instr_words
)
{
	ezj_ir_instr * instr = pezj_ir_next_instr(ir);
	pezj_instr_set_jmp_offs(instr, jmp_offs_num_of_instr_words);
	pezj_instr_set_fn_tbl_entry(instr, fn_tbl_entry);
	pezj_instr_set_bmag_decl_instr_fn_call(instr, bmag_type);
	pezj_instr_set_ir_type(instr, instr_type);
	return instr;
}

static inline ezj_ir_instr * pezj_ir_get_instr(
	const ezj_ir * ir,
	unsigned int n
)
{
	return (pezj_ir_get_instr_arr(ir) + n);
}

static inline ezj_ir_instr * pezj_ir_get_last_instr(const ezj_ir * ir)
{
	return pezj_ir_get_instr(ir, pezj_ir_get_len(ir)-1);
}

static inline unsigned int pezj_ir_get_code_size(const ezj_ir * ir)
{
	ezj_ir_instr * last = pezj_ir_get_last_instr(ir);
	return (pezj_instr_get_offs_in_code(last)
		+ pezj_instr_get_full_size(last));
}

static inline void pezj_ir_set_err_instr(ezj_ir * ir, unsigned int instr_num)
{
	ir->err_instr = instr_num;
}
static inline unsigned int pezj_ir_get_err_instr(const ezj_ir * ir)
{
	return ir->err_instr;
}

static inline void pezj_ir_set_err_code(ezj_ir * ir, ezj_err_code code)
{
	ir->err_code = (unsigned char)code;
}
static inline ezj_err_code pezj_ir_get_err_code(const ezj_ir * ir)
{
	return (ezj_err_code)ir->err_code;
}

static inline void pezj_ir_set_err_str(ezj_ir * ir, ezj_err_code code)
{
	static const char * err_str[EZJ_EC_END] = {
		"",
		"compiled instructions offset overflow",
		"discovered branch magnitude larger than declared",
	};

	ir->err_str = err_str[(unsigned char)code];
}
static inline const char * pezj_ir_get_err_str(const ezj_ir * ir)
{
	return ir->err_str;
}

static inline void pezj_ir_set_err(
	ezj_ir * ir,
	ezj_err_code code,
	unsigned int instr_num
)
{

	pezj_ir_set_err_code(ir, code);
	pezj_ir_set_err_instr(ir, instr_num);
	pezj_ir_set_err_str(ir, code);
}

static inline void pezj_ir_set_err_offs_overflow(
	ezj_ir * ir,
	unsigned int instr_num
)
{
	pezj_ir_set_err(ir, EZJ_EC_OFFS_OVERFLOW, instr_num);
}
static inline void pezj_ir_set_err_bmag_conflict(
	ezj_ir * ir,
	unsigned int instr_num
)
{
	pezj_ir_set_err(ir, EZJ_EC_BMAG_CONFLICT, instr_num);
}
// </ir>
#endif
