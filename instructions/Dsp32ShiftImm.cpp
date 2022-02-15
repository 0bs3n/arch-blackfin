#include "Dsp32ShiftImm.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
Dsp32ShiftImmInstruction::Disassemble(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel) 
{
	/* dsp32shiftimm
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 0 | 0 |.M.| 1 | 1 | 0 | 1 | - | - |.sopcde............|
	|.sop...|.HLs...|.dst0......|.immag.................|.src1......|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int src1     = ((instructionWordLow >> DSP32ShiftImm_src1_bits) & DSP32ShiftImm_src1_mask);
	int sop      = ((instructionWordLow >> DSP32ShiftImm_sop_bits) & DSP32ShiftImm_sop_mask);
	int bit8     = ((instructionWordLow >> 8) & 0x1);
	int immag    = ((instructionWordLow >> DSP32ShiftImm_immag_bits) & DSP32ShiftImm_immag_mask);
	int newimmag = (-(instructionWordLow >> DSP32ShiftImm_immag_bits) & DSP32ShiftImm_immag_mask);
	int dst0     = ((instructionWordLow >> DSP32ShiftImm_dst0_bits) & DSP32ShiftImm_dst0_mask);
	int sopcde   = ((instructionWordHigh >> (DSP32ShiftImm_sopcde_bits - 16)) & DSP32ShiftImm_sopcde_mask);
	int HLs      = ((instructionWordLow >> DSP32ShiftImm_HLs_bits) & DSP32ShiftImm_HLs_mask);

    instr.operand_count = 0;
    instr.operation = OP_MVSHIFTED;
	if (sop == 0 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 2) ? dregs_hi(dst0) : dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 1) ? dregs_hi(src1) : dregs_lo(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_ASHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm4(newimmag) };
	}
	else if (sop == 1 && sopcde == 0 && bit8 == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 2) ? dregs_hi(dst0) : dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 1) ? dregs_hi(src1) : dregs_lo(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTL };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm4(immag) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (sop == 1 && sopcde == 0 && bit8 == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 2) ? dregs_hi(dst0) : dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 1) ? dregs_hi(src1) : dregs_lo(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_ASHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm4(newimmag) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (sop == 2 && sopcde == 0 && bit8 == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 2) ? dregs_hi(dst0) : dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 1) ? dregs_hi(src1) : dregs_lo(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTL };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm4(immag) };
	}
	else if (sop == 2 && sopcde == 0 && bit8 == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 2) ? dregs_hi(dst0) : dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = (HLs & 1) ? dregs_hi(src1) : dregs_lo(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm4(newimmag) };
	}
	else if (sop == 2 && sopcde == 3 && HLs == 1)
	{
        instr.operation = OP_ROT;
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_ROT };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = imm6(immag) };
	}
	else if (sop == 0 && sopcde == 3 && HLs == 0 && bit8 == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTL };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(immag) };
	}
	else if (sop == 0 && sopcde == 3 && HLs == 0 && bit8 == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_ASHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(newimmag) };
	}
	else if (sop == 0 && sopcde == 3 && HLs == 1 && bit8 == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTL };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(immag) };
	}
	else if (sop == 0 && sopcde == 3 && HLs == 1 && bit8 == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(newimmag) };
	}
	else if (sop == 1 && sopcde == 3 && HLs == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(newimmag) };
	}
	else if (sop == 1 && sopcde == 3 && HLs == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(newimmag) };
	}
	else if (sop == 2 && sopcde == 3 && HLs == 0)
	{
        instr.operation = OP_ROT;
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_ROT };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = imm6(immag) };
	}
	else if (sop == 1 && sopcde == 1 && bit8 == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTL };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(immag) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.S = true;
        instr.flags.V = true;
	}
	else if (sop == 1 && sopcde == 1 && bit8 == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_ASHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = imm5(-immag) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.S = true;
        instr.flags.V = true;
	}
	else if (sop == 2 && sopcde == 1 && bit8 == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(newimmag) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (sop == 2 && sopcde == 1 && bit8 == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTL };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = imm5(immag) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (sop == 0 && sopcde == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_ASHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(newimmag) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (sop == 1 && sopcde == 2)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTL };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(immag) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (sop == 2 && sopcde == 2 && bit8 == 1)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(newimmag) };
	}
	else if (sop == 2 && sopcde == 2 && bit8 == 0)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_LSHIFTL };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(immag) };
	}
	else if (sop == 3 && sopcde == 2)
	{
        instr.operation = OP_ROT;
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_ROT };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = imm6(immag) };
	}
	else if (sop == 0 && sopcde == 2)
	{
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = {.cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = {.cls = OPERATOR, .operat = OPER_ASHIFTR };
        instr.operands[instr.operand_count++] = {.cls = IMM, .imm = uimm5(newimmag) };
	}
	else
		return false;

	return true;;
}
