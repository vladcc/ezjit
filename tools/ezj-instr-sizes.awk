#!/usr/bin/awk -f

function is_cls(nm)     {return match(nm, "^[A-Z]+$")}
function is_sub_cls(nm) {return match(nm, "^[A-Z][A-Z_]+$")}
function is_instr(nm)   {return match(nm, "^[a-z][.a-z_0-9<>]+$")}
function is_node(nm)    {return is_cls(nm) || is_sub_cls(nm) || is_instr(nm)}
function is_num(str)    {return match(str, "^[0-9]+$")}

# <err>
function ferrq(msg) {errq(sprintf("%s:%d: %s", FILENAME, FNR, msg))}
function errq(msg) {
	_B_error_happened = 1
	print sprintf("error: %s", msg) > "/dev/stderr"
	exit(1)
}
function ent_errq(msg) {
	errq(msg)
}
function was_err() {return _B_error_happened}
# </err>

# <structs-ent>
# structs:
#
# prefix ent
#
# type list
# has  head node
# has  tail node
# has  count
#
# type val
# has  num
#
# type node
# has  data
# has  next_ node
#
# <private>
function _ent_set(k, v) {_STRUCTS_ent_db[k] = v}
function _ent_get(k) {return _STRUCTS_ent_db[k]}
function _ent_type_chk(ent, texp) {
	if (ent_type_of(ent) == texp)
		return
	ent_errq(sprintf("entity '%s' expected type '%s', actual type '%s'", \
		 ent, texp, ent_type_of(ent)))
}
# <\private>

function ent_clear() {
	delete _STRUCTS_ent_db
	_ent_set("gen", _ent_get("gen")+1)
}
function ent_is(ent) {return (ent in _STRUCTS_ent_db)}
function ent_type_of(ent) {
	if (ent in _STRUCTS_ent_db)
		return _STRUCTS_ent_db[ent]
	ent_errq(sprintf("'%s' not an entity", ent))
}
function ent_new(type,    _ent) {
	_ent_set("ents", (_ent = _ent_get("ents")+1))
	_ent = ("_ent-" _ent_get("gen")+0 "-" _ent)
	_ent_set(_ent, type)
	return _ent
}
# <types>
# <type-list>
function ENT_LIST() {return "list"}

function ent_list_make(head, tail, count,     _ent) {
	_ent = ent_new("list")
	ent_list_set_head(_ent, head)
	ent_list_set_tail(_ent, tail)
	ent_list_set_count(_ent, count)
	return _ent
}

function ent_list_set_head(ent, head) {
	_ent_type_chk(ent, "list")
	if (head)
		_ent_type_chk(head, "node")
	_ent_set(("head=" ent), head)
}
function ent_list_get_head(ent) {
	_ent_type_chk(ent, "list")
	return _ent_get(("head=" ent))
}

function ent_list_set_tail(ent, tail) {
	_ent_type_chk(ent, "list")
	if (tail)
		_ent_type_chk(tail, "node")
	_ent_set(("tail=" ent), tail)
}
function ent_list_get_tail(ent) {
	_ent_type_chk(ent, "list")
	return _ent_get(("tail=" ent))
}

function ent_list_set_count(ent, count) {
	_ent_type_chk(ent, "list")
	_ent_set(("count=" ent), count)
}
function ent_list_get_count(ent) {
	_ent_type_chk(ent, "list")
	return _ent_get(("count=" ent))
}

# <\type-list>
# <type-val>
function ENT_VAL() {return "val"}

function ent_val_make(num,     _ent) {
	_ent = ent_new("val")
	ent_val_set_num(_ent, num)
	return _ent
}

function ent_val_set_num(ent, num) {
	_ent_type_chk(ent, "val")
	_ent_set(("num=" ent), num)
}
function ent_val_get_num(ent) {
	_ent_type_chk(ent, "val")
	return _ent_get(("num=" ent))
}

# <\type-val>
# <type-node>
function ENT_NODE() {return "node"}

function ent_node_make(data, next_,     _ent) {
	_ent = ent_new("node")
	ent_node_set_data(_ent, data)
	ent_node_set_next_(_ent, next_)
	return _ent
}

function ent_node_set_data(ent, data) {
	_ent_type_chk(ent, "node")
	_ent_set(("data=" ent), data)
}
function ent_node_get_data(ent) {
	_ent_type_chk(ent, "node")
	return _ent_get(("data=" ent))
}

function ent_node_set_next_(ent, next_) {
	_ent_type_chk(ent, "node")
	if (next_)
		_ent_type_chk(next_, "node")
	_ent_set(("next_=" ent), next_)
}
function ent_node_get_next_(ent) {
	_ent_type_chk(ent, "node")
	return _ent_get(("next_=" ent))
}

# <\type-node>
# <\types>
# <\structs-ent>

# <list>
function list_make() {
	return ent_list_make()
}
function list_is_empty(list) {
	return !ent_list_get_count(list)
}
function list_push_num(list, num) {
	if (is_num(num))
		list_push_val(list, ent_val_make(num))
	else
		errq(sprintf("list_push_num(): '%s' not a number", num))
}
function list_push_list(list_a, list_b) {
	list_push_node(list_a, ent_node_make(list_b))
}
function list_push_val(list, val) {
	list_push_node(list, ent_node_make(val))
}
function list_push_node(list, node) {
	if (list_is_empty(list)) {
		ent_list_set_head(list, node)
		ent_list_set_tail(list, node)
	} else {
		ent_node_set_next_(ent_list_get_tail(list), node)
		ent_list_set_tail(list, node)
	}
	ent_list_set_count(list, ent_list_get_count(list) + 1)
}

function list_val_sum(val, num) {
	ent_val_set_num(val, ent_val_get_num(val) + num)
}
function list_nodes_sum(node, num,    _type, _data) {
	if (!node)
		return

	_data = ent_node_get_data(node)
	_type = ent_type_of(_data)
	if (ENT_LIST() == _type)
		list_sum(_data, num)
	else if (ENT_VAL() == _type)
		list_val_sum(_data, num)

	list_nodes_sum(ent_node_get_next_(node), num)
}
function list_sum(list, num) {
	if (!list)
		return
	if (!is_num(num))
		errq(sprintf("list_sum(): '%s' not a number", num))

	if (list_is_empty(list))
		list_push_num(list, num)
	else
		list_nodes_sum(ent_list_get_head(list), num)
}

function list_print_val(val) {
	printf("%d", ent_val_get_num(val))
}
function list_print_nodes(node,    _data, _type, _next, _len) {
	if (!node)
		return

	_data = ent_node_get_data(node)
	_type = ent_type_of(_data)
	_next = ent_node_get_next_(node)
	if (ENT_LIST() == _type) {
		_len = ent_list_get_count(_data)
		if (_len > 1)
			printf("(")
		list_print(_data)
		if (_len > 1)
			printf(")")
	} else if (ENT_VAL() == _type) {
		list_print_val(_data)
	}

	if (_next)
		printf("|")

	list_print_nodes(_next)
}
function list_print(list) {
	if (!list)
		return
	list_print_nodes(ent_list_get_head(list))
}

function list_copy_val(val) {
	return ent_val_make(ent_val_get_num(val))
}
function list_copy_nodes(list_dest, node,    _data, _type) {
	if (!node)
		return

	_data = ent_node_get_data(node)
	_type = ent_type_of(_data)
	if (ENT_LIST() == _type)
		list_push_list(list_dest, list_copy(_data))
	else if (ENT_VAL() == _type)
		list_push_val(list_dest, list_copy_val(_data))

	list_copy_nodes(list_dest, ent_node_get_next_(node))
}
function list_copy_list(list_dest, list_src) {
	if (!list_dest || !list_src)
		return
	list_copy_nodes(list_dest, ent_list_get_head(list_src))
	return list_dest
}
function list_copy(list) {
	return list_copy_list(list_make(), list)
}
# </list>

# <cls>
function cls_save(name, defn,    _i, _len, _arr, _sub_cls, _list) {
	_B_cls_arr[++_B_cls_arr_len] = name
	_len = split(defn, _arr, "|")
	_B_cls_tbl[sprintf("%s.sub_cls_count", name)] = _len
	for (_i = 1; _i <= _len ; ++_i) {
		_sub_cls = _arr[_i]
		if (!is_sub_cls(_sub_cls))
			ferrq(sprintf("'%s' not a sub_cls", _sub_cls))
		_B_cls_tbl[sprintf("%s.%d.sub_cls_name", name, _i)] = _arr[_i]
	}
	_B_cls_tbl[sprintf("%s.list", name)] = list_make()
}
function cls_get_count() {
	return _B_cls_arr_len
}
function cls_get_name(n) {
	return _B_cls_arr[n]
}
function cls_get_sub_cls_count(name) {
	return _B_cls_tbl[sprintf("%s.sub_cls_count", name)]
}
function cls_get_sub_cls_name(name, n) {
	return _B_cls_tbl[sprintf("%s.%d.sub_cls_name", name, n)]
}
function cls_push_list(name, list) {
	list_push_list(_B_cls_tbl[sprintf("%s.list", name)], list)
}
function cls_list_get(name) {
	return _B_cls_tbl[sprintf("%s.list", name)]
}
function cls_mark_done(name) {
	_B_cls_tbl[name]
}
function cls_is_done(name) {
	return (name in _B_cls_tbl)
}
# </cls>

# <sub_cls>
function sub_cls_save(name, defn,    _i, _len, _arr) {
	_B_sub_cls_arr[++_B_sub_cls_arr_len] = name
	_len = split(defn, _arr, "+")
	_B_sub_cls_tbl[sprintf("%s.node_count", name)] = _len
	for (_i = 1; _i <= _len ; ++_i) {
		_node = _arr[_i]
		if (!is_node(_node))
			ferrq(sprintf("'%s' not a node", _node))
		_B_sub_cls_tbl[sprintf("%s.%d", name, _i)] = _arr[_i]
	}
	_B_sub_cls_tbl[sprintf("%s.list", name)] = list_make()
}
function sub_cls_count() {
	return _B_sub_cls_arr_len
}
function sub_cls_get_name(n) {
	return _B_sub_cls_arr[n]
}
function sub_cls_get_node_count(name) {
	return _B_sub_cls_tbl[sprintf("%s.node_count", name)]
}
function sub_cls_get_node_name(name, n) {
	return _B_sub_cls_tbl[sprintf("%s.%d", name, n)]
}
function sub_cls_list_set(name, list) {
	if (name in _B_sub_cls_list_is_set)
		errq(sprintf("sub_cls list already set for '%s'", name))
	else
		_B_sub_cls_list_is_set[name]
	_B_sub_cls_tbl[sprintf("%s.list", name)] = list
}
function sub_cls_list_get(name) {
	return _B_sub_cls_tbl[sprintf("%s.list", name)]
}
function sub_cls_mark_done(name) {
	_B_sub_cls_tbl[name]
}
function sub_cls_is_done(name) {
	return (name in _B_sub_cls_tbl)
}
# </sub_cls>

# <instr>
function instr_save(name, size) {
	if (!is_num(size))
		ferrq(sprintf("'%s' not a number", size))
	_B_instr_tbl[name] = size
}
function instr_get_size(name) {
	if (!(name in _B_instr_tbl))
		errq(sprintf("'%s' not a defined instruction", name))
	return _B_instr_tbl[name]
}
# </instr>

function input(str,    _arr, _len, _head, _tail) {
	gsub("[[:space:]]+", "", str)
	_len = split(str, _arr, "=")
	if (_len != 2)
		ferrq("format not <a> = <b>")

	_head = _arr[1]
	_tail = _arr[2]

	if (is_cls(_head))
		cls_save(_head, _tail)
	else if (is_sub_cls(_head))
		sub_cls_save(_head, _tail)
	else if (is_instr(_head))
		instr_save(_head, _tail)
}

# <rec-guard>
function rg_push(name,    _i, _prev) {
	for (_i = 1; _i <= _B_rg_stack_len; ++_i) {
		if (name == _B_rg_stack[_i])
			errq(sprintf("circular definitions %s -> %s", \
				_B_rg_stack_path[_B_rg_stack_len], name))
	}
	_B_rg_stack[++_B_rg_stack_len] = name
	_prev = _B_rg_stack_path[_B_rg_stack_len-1]
	_B_rg_stack_path[_B_rg_stack_len] = \
		(_prev ? ( _prev " -> " name) : name)
}
function rg_pop() {--_B_rg_stack_len}
# </rec-guard>

# <process>
function process_sub_cls_all_nodes(name,    _i, _end, _node) {
	_end = sub_cls_get_node_count(name)
	for (_i = 1; _i <= _end; ++_i) {
		_node = sub_cls_get_node_name(name, _i)
		if (is_cls(_node)) {
			process_cls(_node)
			sub_cls_list_set(name, list_copy(cls_list_get(_node)))
		} else if (is_sub_cls(_node)) {
			process_sub_cls(_node)
			sub_cls_list_set(name, list_copy(sub_cls_list_get(_node)))
		} else if (is_instr(_node)) {
			list_sum(sub_cls_list_get(name), instr_get_size(_node))
		}
	}
}
function process_sub_cls(name) {
	if (sub_cls_is_done(name))
		return
	rg_push(name)
	process_sub_cls_all_nodes(name)
	rg_pop()
	sub_cls_mark_done(name)
}
function process_cls_all_sub_cls(name,    _i, _end, _nm) {
	_end = cls_get_sub_cls_count(name)
	for (_i = 1; _i <= _end; ++_i) {
		_nm = cls_get_sub_cls_name(name, _i)
		process_sub_cls(_nm)
		cls_push_list(name, list_copy(sub_cls_list_get(_nm)))
	}
}
function process_cls(name) {
	if (cls_is_done(name))
		return
	rg_push(name)
	process_cls_all_sub_cls(name)
	rg_pop()
	cls_mark_done(name)
}
function process_all_cls(    _i, _end) {
	_end = cls_get_count()
	for (_i = 1; _i <= _end; ++_i)
		process_cls(cls_get_name(_i))
}

function process() {
	process_all_cls()
}
# </process>

# <output>
function mark_out(str) {_B_out_tbl[str]}
function is_out(str)   {return (str in _B_out_tbl)}

function output_sub_cls(name,    _i, _end, _str) {
	if (is_out(name))
		return

	mark_out(name)
	print sprintf("Instr : %s", name)
	printf("Compos: ")
	_end = sub_cls_get_node_count(name)
	for (_i = 1; _i <= _end; ++_i) {
		_str = sub_cls_get_node_name(name, _i)
		if (is_instr(_str))
			printf("%s:%d", _str, instr_get_size(_str))
		else
			printf("%s", _str)
		if (_i < _end)
			printf(" + ")
	}
	print ""
	printf("Size  : ")
	list_print(sub_cls_list_get(name))
	print ""
	print ""
}
function output_cls(name,    _i, _end, _str) {
	if (is_out(name))
		return

	mark_out(name)
	print sprintf("Instr : %s", name)
	printf("Compos: ")
	_end = cls_get_sub_cls_count(name)
	for (_i = 1; _i <= _end; ++_i) {
		_str = cls_get_sub_cls_name(name, _i)
		printf("%s", _str)
		if (_i < _end)
			printf(" | ")
	}
	print ""
	printf("Size  : ")
	list_print(cls_list_get(name))
	print ""
	print ""
	for (_i = 1; _i <= _end; ++_i) {
		_str = cls_get_sub_cls_name(name, _i)
		output_sub_cls(_str)
	}
}
function output_all_cls(    _i, _end, _str) {
	_end = cls_get_count()
	for (_i = 1; _i <= _end; ++_i)
		output_cls(cls_get_name(_i))
}
function output() {
	output_all_cls()
}
# </output>

# <awk>
$0 && !match($0, "^[[:space:]]*#") {
	input($0)
}

END {
	if (!was_err()) {
		process()
		output()
	}
}
# </awk>

