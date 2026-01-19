#!/bin/bash

function run_test
{
	local CMPL="gcc ezjit.c examples/example-${1}.c -o ${1}.bin -I. -Wall $4 -fomit-frame-pointer"

	cat /etc/os-release | tr -d '"' | grep PRETTY_ | awk -F'=' '{print $NF}'
	uname -srv
	lscpu | grep 'Model name:' | awk -F':[[:space:]]+' '{print $NF}'
	gcc --version | head -n1

	echo "${CMPL}"
	${CMPL}
	./${1}.bin $2 > /dev/null # warm up

	echo "./${1}.bin $3"

	./${1}.bin $3                  \
	| awk -f ./tools/hist.awk      \
	| awk -f ./tools/perc.awk "$5" \
	| column -t

	echo ""
}

function main
{
	local JMP_EST=""

	if [[ $# -gt 0 ]]; then
		JMP_EST="-vJmpEst=1"
	fi

	echo ""
	echo "====================================================================="
	echo "=== No Optimizations, Never Inline VM Instructions =================="
	echo "====================================================================="
	run_test "loop"   "1000 1"  "500000000 1"  "-O0 -DNEVER_INLINE" "$JMP_EST"
	run_test "fib"    "10"      "37"           "-O0 -DNEVER_INLINE" "$JMP_EST"
	run_test "sieve"  "1000"    "50000000"     "-O0 -DNEVER_INLINE" "$JMP_EST"

	echo ""
	echo "====================================================================="
	echo "=== No Optimizations, Always Inline VM Instructions ================="
	echo "====================================================================="
	run_test "loop"   "1000 1"  "500000000 1"  "-O0 -DALWAYS_INLINE" "$JMP_EST"
	run_test "fib"    "10"      "37"           "-O0 -DALWAYS_INLINE" "$JMP_EST"
	run_test "sieve"  "1000"    "50000000"     "-O0 -DALWAYS_INLINE" "$JMP_EST"

	echo ""
	echo "====================================================================="
	echo "=== Full Optimizations, Never Inline VM Instructions ================"
	echo "====================================================================="
	run_test "loop"   "1000 1"  "500000000 1"  "-O3 -DNEVER_INLINE" "$JMP_EST"
	run_test "fib"    "10"      "37"           "-O3 -DNEVER_INLINE" "$JMP_EST"
	run_test "sieve"  "1000"    "50000000"     "-O3 -DNEVER_INLINE" "$JMP_EST"

	echo ""
	echo "====================================================================="
	echo "=== Full Optimizations, Always Inline VM Instructions ==============="
	echo "====================================================================="
	run_test "loop"   "1000 1"  "500000000 1"  "-O3 -DALWAYS_INLINE" "$JMP_EST"
	run_test "fib"    "10"      "37"           "-O3 -DALWAYS_INLINE" "$JMP_EST"
	run_test "sieve"  "1000"    "50000000"     "-O3 -DALWAYS_INLINE" "$JMP_EST"
}

main "$@"
