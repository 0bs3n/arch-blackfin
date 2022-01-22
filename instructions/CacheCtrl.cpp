#include "CacheCtrl.h"

bool 
CacheCtrlInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) 
{
	int a   = ((instructionWord >> CaCTRL_a_bits) & CaCTRL_a_mask);
	int op  = ((instructionWord >> CaCTRL_op_bits) & CaCTRL_op_mask);
	int reg = ((instructionWord >> CaCTRL_reg_bits) & CaCTRL_reg_mask);

    if (parallel) return false;

    instr.operands[0].cls = blackfin::MNEMOMIC;
    instr.operands[1] = { .cls = blackfin::MEM_ACCESS, .mem_access = {
        .mode = blackfin::MEM_REGMOD,
        .ptr_reg = pregs(reg),
        .width = 4
    }};
    instr.operation = blackfin::OP_CACHE;

    switch (op) {
    case 0:
        instr.operands[0].mnemonic = blackfin::OL_PREFETCH;
        break;
    case 1:
        instr.operands[0].mnemonic = blackfin::OL_FLUSHINV;
        break;
    case 2:
        instr.operands[0].mnemonic = blackfin::OL_FLUSH;
        break;
    case 3:
        instr.operands[0].mnemonic = blackfin::OL_IFLUSH;
        break;
    }

    if (a) instr.operands[1].mem_access.post_inc = true;

    return true;
}
