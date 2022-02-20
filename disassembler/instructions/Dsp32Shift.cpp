#include "Instructions.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
BlackfinInstruction::DisassembleDsp32Shift(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel) 
{
	/* dsp32shift
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 0 | 0 |.M.| 1 | 1 | 0 | 0 | - | - |.sopcde............|
	|.sop...|.HLs...|.dst0......| - | - | - |.src0......|.src1......|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int HLs  = ((instructionWordLow >> DSP32Shift_HLs_bits) & DSP32Shift_HLs_mask);
	int sop  = ((instructionWordLow >> DSP32Shift_sop_bits) & DSP32Shift_sop_mask);
	int src0 = ((instructionWordLow >> DSP32Shift_src0_bits) & DSP32Shift_src0_mask);
	int src1 = ((instructionWordLow >> DSP32Shift_src1_bits) & DSP32Shift_src1_mask);
	int dst0 = ((instructionWordLow >> DSP32Shift_dst0_bits) & DSP32Shift_dst0_mask);
	int sopcde = ((instructionWordHigh >> (DSP32Shift_sopcde_bits - 16)) & DSP32Shift_sopcde_mask);
	enum Register acc01 = (HLs & 1) == 0 ? REG_A0 : REG_A1;

    instr.operand_count = 0;
    instr.operation = OP_DSPSHIFT;
	if (HLs == 0 && sop == 0 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (HLs == 1 && sop == 0 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (HLs == 2 && sop == 0 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (HLs == 3 && sop == 0 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (HLs == 0 && sop == 1 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (HLs == 1 && sop == 1 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (HLs == 2 && sop == 1 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (HLs == 3 && sop == 1 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (sop == 2 && sopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = (HLs & 2) == 0 ? dregs_lo(dst0) : dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_LSHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = (HLs & 1) == 0 ? dregs_lo(src1) : dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 0 && sopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = acc01 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = acc01 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 1 && sopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = acc01 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_LSHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = acc01 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 2 && sopcde == 3)
	{
        instr.operation = OP_ROT;
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = acc01 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ROT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = acc01 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 3 && sopcde == 3)
	{
        // FIXME: This isn't an actual instruction, why was this in libopcodes?
        // should we keep it in?
        instr.operation = OP_ROT;
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ROT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 1 && sopcde == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.V = true;
        instr.flags.S = true;
	}
	else if (sop == 0 && sopcde == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (sop == 0 && sopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 1 && sopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (sop == 2 && sopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_LSHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 3 && sopcde == 2)
	{
        instr.operation = OP_ROT;
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ROT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 2 && sopcde == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_LSHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BY };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (sop == 0 && sopcde == 4)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_PACK };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 1 && sopcde == 4)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_PACK };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
	}
	else if (sop == 2 && sopcde == 4)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_PACK };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 3 && sopcde == 4)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_PACK };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
	}
	else if (sop == 0 && sopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SIGNBITS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
	}
	else if (sop == 1 && sopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SIGNBITS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
	}
	else if (sop == 2 && sopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SIGNBITS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
	}
	else if (sop == 0 && sopcde == 6)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SIGNBITS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
	}
	else if (sop == 1 && sopcde == 6)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SIGNBITS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
	}
	else if (sop == 3 && sopcde == 6)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ONES };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
	}
	else if (sop == 0 && sopcde == 7)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_EXPADJ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 1 && sopcde == 7)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_EXPADJ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (sop == 2 && sopcde == 7)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_EXPADJ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 3 && sopcde == 7)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_EXPADJ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (sop == 0 && sopcde == 8)
	{
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BITMUX };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASRMOD };
        instr.flags.ASR = true;
	}
	else if (sop == 1 && sopcde == 8)
	{
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BITMUX };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASLMOD };
        instr.flags.ASL = true;
	}
	else if (sop == 0 && sopcde == 9)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VITMAX };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASLMOD };
        instr.flags.ASL = true;
	}
	else if (sop == 1 && sopcde == 9)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VITMAX };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASRMOD };
        instr.flags.ASR = true;
	}
	else if (sop == 2 && sopcde == 9)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VITMAX };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASLMOD };
        instr.flags.ASL = true;
	}
	else if (sop == 3 && sopcde == 9)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VITMAX };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASRMOD };
        instr.flags.ASR = true;
	}
	else if (sop == 0 && sopcde == 10)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_EXTRACT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .flags = { .zero_extended = true }, .reg = dregs_lo(src0)};
	}
	else if (sop == 1 && sopcde == 10)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_EXTRACT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .flags = { .sign_extended = true }, .reg = dregs_lo(src0)};
	}
	else if (sop == 2 && sopcde == 10)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_DEPOSIT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0)};
	}
	else if (sop == 3 && sopcde == 10)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_DEPOSIT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .flags = { .sign_extended = true }, .reg = dregs(src0)};
	}
	else if (sop == 0 && sopcde == 11)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_CC };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BXORSHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
	}
	else if (sop == 1 && sopcde == 11)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_CC };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BXOR };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
	}
	else if (sop == 0 && sopcde == 12) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BXORSHIFT };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_CC };
    }
	else if (sop == 1 && sopcde == 12)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_CC };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BXOR };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_CC };
	}
	else if (sop == 0 && sopcde == 13)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ALIGN8 };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
	}
	else if (sop == 1 && sopcde == 13)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ALIGN16 };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
	}
	else if (sop == 2 && sopcde == 13)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ALIGN24 };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
	}
	else
		return false;

	return true;
}
