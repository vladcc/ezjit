#!/usr/bin/awk -f

# <prog>
function SCRIPT_NAME()    {return "asm"}
function SCRIPT_VERSION() {return "1.0"}

function exit_failure() {exit(1)}

function pstderr(str) {print str > "/dev/stderr"}
function perr(str) {
	pstderr(sprintf("%s: error: %s", SCRIPT_NAME(), str))
	_B_err_happened = 1
}
function errq(str) {
	perr(str)
	exit_failure()
}
function ferrq(str) {
	errq(sprintf("%s: %d: %s\n%s", FILENAME, FNR, str, $0))
}
function err_happened() {
	return _B_err_happened
}
function has_num_fld(n) {
	if (NF != n)
		ferrq(sprintf("%s fields expected, got %s", n, NF))
}
# </prog>

# <defn>
function defn_init() {
	_B_defn_tbl["rret"] = 0
}
function do_define() {
	has_num_fld(3)
	defn($2, $3)
}
function do_undef() {
	has_num_fld(2)
	undef($2)
}
function defn(name, val) {
	if (name in _B_defn_tbl)
		ferrq("name '%s' redefined")
	_B_defn_tbl[name] = val
}
function undef(name) {
	if (!(name in _B_defn_tbl))
		ferrq("name '%s' not defined", name)
	delete _B_defn_tbl[name]
}
function defn_get(name) {
	if (!name)
		return ""
	gsub("bp\\[|\\]", "", name)
	return (name in _B_defn_tbl) ? _B_defn_tbl[name] : name
}
# </defn>

# <lbl>
function do_lbl() {
	has_num_fld(1)
	gsub("[.:]", "")
	# lbl points to the *next* instr
	_B_lbl_tbl[$0] = instr_num()+1
}
function lbl_get(lbl) {
	return _B_lbl_tbl[lbl]
}
# </lbl>

# <instr>
# <tbl>
function instr_fld_tbl_init() {
	_B_instr_fld_tbl["pusha"]  = 2
	_B_instr_fld_tbl["pushv"]  = 2
	_B_instr_fld_tbl["call"]   = 2
	_B_instr_fld_tbl["pop"]    = 2
	_B_instr_fld_tbl["movvr"]  = 3
	_B_instr_fld_tbl["pushc"]  = 2
	_B_instr_fld_tbl["movvc"]  = 3
	_B_instr_fld_tbl["bgeqvv"] = 4
	_B_instr_fld_tbl["bgrtvv"] = 4
	_B_instr_fld_tbl["beqrc"]  = 4
	_B_instr_fld_tbl["addvvc"] = 4
	_B_instr_fld_tbl["jmp"]    = 2
	_B_instr_fld_tbl["finish"] = 1
	_B_instr_fld_tbl["ret"]    = 1
	_B_instr_fld_tbl["movrc"]  = 3
	_B_instr_fld_tbl["mulvvv"] = 4
	_B_instr_fld_tbl["bleqvv"] = 4
	_B_instr_fld_tbl["ldbvvv"] = 4
	_B_instr_fld_tbl["bneqvc"] = 4
	_B_instr_fld_tbl["beqvc"]  = 4
	_B_instr_fld_tbl["stbvvc"] = 4
	_B_instr_fld_tbl["addvvv"] = 4
	_B_instr_fld_tbl["movvv"]  = 3
	_B_instr_fld_tbl["bltvc"]  = 4
	_B_instr_fld_tbl["subvvc"] = 4
	_B_instr_fld_tbl["malloc"] = 3
	_B_instr_fld_tbl["movrv"]  = 3
	_B_instr_fld_tbl["freev"]  = 2
	_B_instr_fld_tbl["memset"] = 4
	_B_instr_fld_tbl["stbvcc"] = 4
}
function instr_check() {
	if (!($1 in _B_instr_fld_tbl))
		ferrq(sprintf("unknown instruction: '%s'", $1))
	if (_B_instr_fld_tbl[$1] != NF)
		ferrq(sprintf("%s fields expected, got %s", _B_instr_fld_tbl[$1], NF))
	++_B_instr_num
}
# </tbl>

function decode_op(n, op) {
	if (match(op, "^[a-z_]+$"))
		return (lbl_get(op) - n)
	return op+0
}

function instr_save(type, op_a, op_b, op_c,    _num) {
	_num = instr_num()
	_B_instr_save[sprintf("%d.type", _num)] = type
	_B_instr_save[sprintf("%d.op_a", _num)] = defn_get(op_a)
	_B_instr_save[sprintf("%d.op_b", _num)] = defn_get(op_b)
	_B_instr_save[sprintf("%d.op_c", _num)] = defn_get(op_c)
}
function instr_get(n, arr_out) {
	arr_out[1] = _B_instr_save[sprintf("%d.type", n)]
	arr_out[2] = _B_instr_save[sprintf("%d.op_a", n)]
	arr_out[3] = _B_instr_save[sprintf("%d.op_b", n)]
	arr_out[4] = _B_instr_save[sprintf("%d.op_c", n)]
}

function do_instr() {
	instr_check()
	instr_save($1, $2, $3, $4)
}

function instr_num() {
	return _B_instr_num
}

function instr_generate(    _i, _end, _arr) {
	_end = instr_num()
	for (_i = 1; _i <= _end; ++_i) {
		instr_get(_i, _arr)
		print sprintf("{%s, %s, %s, %s, NULL},", \
			toupper(_arr[1]),                    \
			decode_op(_i, _arr[2]),              \
			decode_op(_i, _arr[3]),              \
			decode_op(_i, _arr[4]))
	}
}
# </instr>

function init() {
	defn_init()
	instr_fld_tbl_init()
}

# <awk>
BEGIN {
	init()
}

/^[[:space:]]*(;|$)/ {
	# comment or empty line
	next
}

/^#define / {
	do_define()
	next
}
/^#undef / {
	do_undef()
	next
}

/^\.[a-zA-Z0-9_]+:/ {
	do_lbl()
	next
}

{
	do_instr()
	next
}

END {
	if (!err_happened())
		instr_generate()
}
# </awk>

