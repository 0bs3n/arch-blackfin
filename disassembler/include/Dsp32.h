#include "arch_blackfin.h"

void decode_optmode (int mod, int MM, struct blackfin::Instruction &instr);
int decode_multfunc (int h0, int h1, int src0, int src1, struct blackfin::Instruction &instr);
int decode_macfunc (int which, int op, int h0, int h1, int src0, int src1, struct blackfin::Instruction &instr);
void amod0 (int s0, int x0, struct blackfin::Instruction &instr);
void amod1 (int s0, int x0, struct blackfin::Instruction &instr);
void amod0amod2 (int s0, int x0, int aop0, struct blackfin::Instruction &instr);
void searchmod (int r0, struct blackfin::Instruction &instr);
void aligndir (int r0, struct blackfin::Instruction &instr);
