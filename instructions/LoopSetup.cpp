#include "LoopSetup.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
LoopSetupInstruction::Disassemble(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel) 
{
	/* LoopSetup
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 1 |.rop...|.c.|.soffset.......|
	|.reg...........| - | - |.eoffset...............................|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int c   = ((instructionWordHigh >> (LoopSetup_c_bits - 16)) & LoopSetup_c_mask);
	int reg = ((instructionWordLow>> LoopSetup_reg_bits) & LoopSetup_reg_mask);
	int rop = ((instructionWordHigh >> (LoopSetup_rop_bits - 16)) & LoopSetup_rop_mask);
	int soffset = ((instructionWordHigh >> (LoopSetup_soffset_bits - 16)) & LoopSetup_soffset_mask);
	int eoffset = ((instructionWordLow>> LoopSetup_eoffset_bits) & LoopSetup_eoffset_mask);

	if (parallel)
		return false;

	if (reg > 7)
		return false;

    instr.operation = OP_LSETUP;
    instr.operand_count = 7;
    instr.operands[0] = { .cls = MNEMOMIC, .mnemonic = OL_LSETUP };
    instr.operands[1] = { .cls = OPERATOR, .operat = OPER_OPENP };
    instr.operands[2] = { .cls = IMM, .imm = pcrel4(soffset) };
    instr.operands[2].flags.pcrelative = true;
    instr.operands[3] = { .cls = OPERATOR, .operat = OPER_COMMA };
    instr.operands[4] = { .cls = IMM, .imm = lppcrel10(eoffset) };
    instr.operands[4].flags.pcrelative = true;
    instr.operands[5] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
    instr.operands[6] = { .cls = REG, .reg = counters(c) };

    if (rop == 1 || rop == 3) {
        instr.operand_count = 9;
        instr.operands[7] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[8] = { .cls = REG, .reg = pregs(reg) };
        if (rop == 3) {
            instr.operand_count = 11;
            instr.operands[9] = { .cls = OPERATOR, .operat = OPER_LSHIFTR };
            instr.operands[10] = { .cls = IMM, .imm = 1 };
        }
    } 
	return true;
}
