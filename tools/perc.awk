# Expected input, e.g.:
# 9227465  compiled   #                                                   0.01s
# 9227465  native     ####################                                0.25s
# 9227465  switch     ########################                            0.31s
# 9227465  threaded   ###########################################         0.55s
# 9227465  dthreaded  ##########################################          0.54s
# 9227465  fptrs      ##################################################  0.64s

function get_val()      {return $1}
function get_how()      {return $2}
function get_time_str() {return $4}
function get_time_num() {return get_time_str()+0}

function abs(n) {return (n < 0) ? -n : n}

function EPS() {return 0.0001}

function process(max,   _time, _i, _perc, _diff) {

	if (!max)
		max = EPS()

	for (_i = 1; _i <= FNR; ++_i) {
		if (!(_time = G_time[_i]))
			_time = EPS()
		_perc = (_time/max) * 100
		_diff = 100 - _perc
		print sprintf("%s %.2f%% %s%.2f%%", \
			G_line[_i], _perc, (_diff <= 0) ? "+" : "-", abs(_diff))
	}
}

BEGIN {
	G_max = 0
}

{
	if (JmpEst) {
		if ("ezj" == get_how())
			G_max = get_time_num()
	} else if (G_max < get_time_num()) {
		G_max = get_time_num()
	}

	G_line[FNR] = $0
	G_time[FNR] = get_time_num()
}

END {
	process(G_max)
}
