#ifndef EZJIT_ARCH_H
#define EZJIT_ARCH_H

#if defined(__x86_64__)
	#define EZJIT_ARCH_X64
#elif defined(__i386__)
	#define EZJIT_ARCH_X86
#elif defined(__aarch64__)
	#define EZJIT_ARCH_AARCH64
#elif defined(__arm__)
	#define EZJIT_ARCH_AARCH32
#elif defined(__riscv) && (__riscv_xlen == 64)
	#define EZJIT_ARCH_RISCV64
#else
	#define EZJIT_ARCH_NONE
#endif

#endif
