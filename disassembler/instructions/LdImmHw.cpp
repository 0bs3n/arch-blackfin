#include "Instructions.h"
#include "binaryninjaapi.h"

bool 
BlackfinInstruction::DisassembleLoadImmHalfWord(uint16_t instructionWordHigh, uint16_t instructionWordLow, 
        struct blackfin::Instruction &instr, bool parallel) 
{
	/* LDIMMhalf
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 1 | 0 | 0 | 0 | 0 | 1 |.Z.|.H.|.S.|.grp...|.reg.......|
	|.hword.........................................................|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */

	int H = ((instructionWordHigh >> (LDIMMhalf_H_bits - 16)) & LDIMMhalf_H_mask);
	int Z = ((instructionWordHigh >> (LDIMMhalf_Z_bits - 16)) & LDIMMhalf_Z_mask);
	int S = ((instructionWordHigh >> (LDIMMhalf_S_bits - 16)) & LDIMMhalf_S_mask);
	int reg = ((instructionWordHigh >> (LDIMMhalf_reg_bits - 16)) & LDIMMhalf_reg_mask);
	int grp = ((instructionWordHigh >> (LDIMMhalf_grp_bits - 16)) & LDIMMhalf_grp_mask);
	int hword = ((instructionWordLow >> LDIMMhalf_hword_bits) & LDIMMhalf_hword_mask);

    instr.operation = blackfin::OP_LDIMMHW;
    instr.operand_count = 3;
    instr.operands[1].cls = blackfin::OPERATOR;
    instr.operands[1].operat = blackfin::OPER_EQ;

    if (!Z && !S) {
        instr.operands[2].cls = blackfin::IMM;
        instr.operands[2].imm = uimm16(hword);
        if (H) {
            instr.operands[0].cls = blackfin::REG;
            if (grp == 0) instr.operands[0].reg = dregs_hi(reg);
            else instr.operands[0].reg = regs_hi(reg, grp);
        } else {
            instr.operands[0].cls = blackfin::REG;
            if (grp == 0) instr.operands[0].reg = dregs_lo(reg);
            else instr.operands[0].reg = regs_lo(reg, grp);
        }
    } else {
        if (Z && S) return false;
        instr.operands[0].cls = blackfin::REG;
        if (grp == 0) instr.operands[0].reg = dregs(reg);
        else instr.operands[0].reg = regs(reg, grp);
    }

    if (S) {
        instr.operands[2].cls = blackfin::IMM;
        instr.operands[2].imm = imm16(hword);
        instr.operands[2].flags.sign_extended = true;
        return true;
    } else if (Z) {
        instr.operands[2].cls = blackfin::UIMM;
        instr.operands[2].uimm = uimm16(hword);
        instr.operands[2].flags.zero_extended = true;
        return true;
    }
    return false;
}
