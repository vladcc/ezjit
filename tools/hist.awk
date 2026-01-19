# Expected input, e.g.:
# 1000 compiled   0.00s
# 1000 native     0.00s
# 1000 switch     0.00s
# 1000 threaded   0.00s
# 1000 dthreaded  0.00s
# 1000 fptrs      0.00s
# 1000 ezj        0.01s
# 1000 ezj_fc     0.00s
# 1000 ezj_fcj    0.00s
# 1000 ezj_ac     0.00s
# 1000 ezj_acj    0.00s

function get_val()      {return $1}
function get_how()      {return $2}
function get_time_str() {return $3}
function get_time_num() {return get_time_str()+0}

function EPS() {return 0.0001}

function hprint(val, how, time, perc,   _max, _i, _end, _str) {
	_str = "#"
	_max = 49
	_end = int(perc*_max)
	for (_i = 1; _i <= _end; ++_i)
		_str = (_str "#")
	print sprintf("%s %s %s %s", val, how, _str, time)
}

function process(max,   _time, _i) {

	if (!max)
		max = EPS()

	for (_i = 1; _i <= FNR; ++_i) {
		if (!(_time = G_time[_i]+0))
			_time = EPS()
		hprint(G_val[_i], G_how[_i], G_time[_i], _time/max)
	}
}

BEGIN {
	G_max = 0
}

{
	if (G_max < get_time_num())
		G_max = get_time_num()

	G_val[FNR]  = get_val()
	G_how[FNR]  = get_how()
	G_time[FNR] = get_time_str()
}

END {
	process(G_max)
}
