#ifndef EZJIT_PRIV_DECL_H
#define EZJIT_PRIV_DECL_H

typedef enum pezj_ir_type {
	PEZJ_IR_NONE,   //  0
	PEZJ_IR_INSTR,  //  1
	PEZJ_IR_UJMP,   //  2
	PEZJ_IR_CJMP,   //  3
	PEZJ_IR_CALL,   //  4
	PEZJ_IR_RET,    //  5
	PEZJ_IR_FINISH, //  6
	PEZJ_IR_END     //  7
} pezj_ir_type;

// branch magnitudes
typedef enum pezj_bmag {
	PEZJ_BMAG_NONE,  //  0
	PEZJ_BMAG_NEAR,  //  1
	PEZJ_BMAG_FAR,   //  2
	PEZJ_BMAG_ABS,   //  3
	PEZJ_BMAG_END    //  4
} pezj_bmag;

#endif
