#include "Dsp32ALU.h"
#include "Dsp32.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
Dsp32ALUInstruction::Disassemble(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel) 
{
	/* dsp32alu
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 0 | 0 |.M.| 1 | 0 | - | - | - |.HL|.aopcde............|
	|.aop...|.s.|.x.|.dst0......|.dst1......|.src0......|.src1......|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int s    = ((instructionWordLow >> DSP32Alu_s_bits) & DSP32Alu_s_mask);
	int x    = ((instructionWordLow >> DSP32Alu_x_bits) & DSP32Alu_x_mask);
	int aop  = ((instructionWordLow >> DSP32Alu_aop_bits) & DSP32Alu_aop_mask);
	int src0 = ((instructionWordLow >> DSP32Alu_src0_bits) & DSP32Alu_src0_mask);
	int src1 = ((instructionWordLow >> DSP32Alu_src1_bits) & DSP32Alu_src1_mask);
	int dst0 = ((instructionWordLow >> DSP32Alu_dst0_bits) & DSP32Alu_dst0_mask);
	int dst1 = ((instructionWordLow >> DSP32Alu_dst1_bits) & DSP32Alu_dst1_mask);
	int HL   = ((instructionWordHigh >> (DSP32Alu_HL_bits - 16)) & DSP32Alu_HL_mask);
	int aopcde = ((instructionWordHigh >> (DSP32Alu_aopcde_bits - 16)) & DSP32Alu_aopcde_mask);

    instr.operation = OP_DSPALU;
    instr.operand_count = 0;
	if (aop == 0 && aopcde == 9 && HL == 0 && s == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0L };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (aop == 2 && aopcde == 9 && HL == 1 && s == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1H };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
	}
	else if (aop == 2 && aopcde == 9 && HL == 0 && s == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1L };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (aop == 0 && aopcde == 9 && HL == 1 && s == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0H };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
	}
	else if (x == 1 && HL == 1 && aop == 3 && aopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RND20 };
	}
	else if (x == 1 && HL == 1 && aop == 2 && aopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RND20 };
	}
	else if (x == 0 && HL == 0 && aop == 1 && aopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RND12 };
	}
	else if (x == 0 && HL == 0 && aop == 0 && aopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RND12 };
	}
	else if (x == 1 && HL == 0 && aop == 3 && aopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RND20 };
	}
	else if (x == 0 && HL == 1 && aop == 0 && aopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RND12 };
	}
	else if (x == 1 && HL == 0 && aop == 2 && aopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RND20 };
	}
	else if (x == 0 && HL == 1 && aop == 1 && aopcde == 5)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RND12 };
	}
	else if (HL == 1 && aop == 0 && aopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 1 && aop == 1 && aopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 1 && aop == 2 && aopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 1 && aop == 3 && aopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aop == 0 && aopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aop == 1 && aopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aop == 3 && aopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 1 && aop == 0 && aopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 1 && aop == 1 && aopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 1 && aop == 2 && aopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 1 && aop == 3 && aopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aop == 2 && aopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aop == 1 && aopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aop == 2 && aopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aop == 3 && aopcde == 3)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aop == 0 && aopcde == 2)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
		amod1 (s, x, instr);
	}
	else if (aop == 0 && aopcde == 9 && s == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
	}
	else if (aop == 3 && aopcde == 11 && s == 0) 
    {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUSEQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
    }
	else if (aop == 3 && aopcde == 11 && s == 1) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUSEQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_W32FLAG };
        instr.flags.W32 = true;
    }

	else if (aop == 1 && aopcde == 22 && HL == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP2P };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_THMOD };
        instr.flags.TH = true;
		if (s == 1) {
            instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RMOD };
            instr.flags.R = true;
        }
	}
	else if (aop == 1 && aopcde == 22 && HL == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP2P };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_TLMOD };
        instr.flags.TL = true;
		if (s == 1) {
            instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RMOD };
            instr.flags.R = true;
        }
	}
	else if (aop == 0 && aopcde == 22 && HL == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP2P };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RNDHMOD };
        instr.flags.RNDH = true;
		if (s == 1) {
            instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RMOD };
            instr.flags.R = true;
        }
	}
	else if (aop == 0 && aopcde == 22 && HL == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP2P };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RNDLMOD };
        instr.flags.RNDL = true;
		if (s == 1) {
            instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RMOD };
            instr.flags.R = true;
        }
	}
	else if (aop == 0 && s == 0 && aopcde == 8) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = 0 };
    }
	else if (aop == 0 && s == 1 && aopcde == 8) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
    }
	else if (aop == 1 && s == 0 && aopcde == 8) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = 0 };
    }
	else if (aop == 1 && s == 1 && aopcde == 8) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
    }
	else if (aop == 2 && s == 0 && aopcde == 8) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = 0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = 0 };
    }
	else if (aop == 2 && s == 1 && aopcde == 8) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
    }
	else if (aop == 3 && s == 0 && aopcde == 8) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
    }
	else if (aop == 3 && s == 1 && aopcde == 8) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
    }
	else if (aop == 1 && aopcde == 9 && s == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1x };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (aop == 1 && HL == 0 && aopcde == 11)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSEQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
	}
	else if (aop == 3 && HL == 0 && aopcde == 16) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ABS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };

        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };

        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ABS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
    }
	else if (aop == 0 && aopcde == 23 && HL == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP2P };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_HIMOD };
        instr.flags.HI = true;
		if (s == 1) {
            instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RMOD };
            instr.flags.R = true;
        }
	}
	else if (aop == 3 && aopcde == 9 && s == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1x };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
	}
	else if (aop == 1 && HL == 1 && aopcde == 16) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ABS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
    }
	else if (aop == 0 && HL == 1 && aopcde == 16) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ABS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
    }
	else if (aop == 2 && aopcde == 9 && s == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
	}
	else if (HL == 0 && aop == 3 && aopcde == 12)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RNDMOD };
        instr.flags.RND = true;
	}
	else if (aop == 1 && HL == 0 && aopcde == 16) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ABS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
    }
	else if (aop == 0 && HL == 0 && aopcde == 16) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ABS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
    }
	else if (aop == 3 && HL == 0 && aopcde == 15)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (aop == 3 && s == 1 && HL == 0 && aopcde == 7)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
	}
	else if (aop == 3 && s == 0 && HL == 0 && aopcde == 7)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_NSMOD };
        instr.flags.NS = true;
	}
	else if (aop == 1 && HL == 1 && aopcde == 11)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSEQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
	}
	else if (aop == 2 && aopcde == 11 && s == 0) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSEQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
    }
	else if (aop == 2 && aopcde == 11 && s == 1) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSEQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_W32FLAG };
        instr.flags.W32 = true;
    }
	else if (aop == 3 && HL == 0 && aopcde == 14) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
    }
	else if (HL == 1 && aop == 3 && aopcde == 12)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RNDMOD };
        instr.flags.RND = true;
	}
	else if (aop == 0 && aopcde == 23 && HL == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP3P };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_LOMOD };
        instr.flags.LO = true;
		if (s == 1) {
            instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RMOD };
            instr.flags.R = true;
        }
	}
	else if (aop == 0 && HL == 0 && aopcde == 14) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
    }
	else if (aop == 1 && HL == 0 && aopcde == 14) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };

    }
	else if (aop == 0 && HL == 1 && aopcde == 14) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
    }
	else if (aop == 1 && HL == 1 && aopcde == 14) {
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQNEG };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
    }
	else if (aop == 0 && aopcde == 12)
	{
        // dreg_hi = dreg_lo = SIGN (dreg_hi) * dreg_hi + SIGN (dreg_lo) * dreg_lo
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic =  OL_SIGN };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MUL };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_hi(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic =  OL_SIGN };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MUL };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(src1) };
	}
	else if (aop == 2 && aopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUSORPLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
		amod0 (s, x, instr);
	}
	else if (aop == 1 && aopcde == 12)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1L };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1H };

        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };

        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0L };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0H };
	}
	else if (aop == 2 && aopcde == 4)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };

        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };

        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
		amod1 (s, x, instr);
	}
	else if (HL == 0 && aopcde == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSORPLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };

        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };

        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUSORMINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
		amod0amod2 (s, x, aop, instr);
	}
	else if (aop == 0 && aopcde == 11)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSEQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
	}
	else if (aop == 0 && aopcde == 10)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0x };
	}
	else if (aop == 1 && aopcde == 10)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs_lo(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1x };
	}
	else if (aop == 1 && aopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSORMINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
		amod0 (s, x, instr);
	}
	else if (aop == 3 && aopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUSORMINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
		amod0 (s, x, instr);
	}
	else if (aop == 1 && aopcde == 4)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
		amod1 (s, x, instr);
	}
	else if (aop == 0 && aopcde == 17)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };

        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };

        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
		amod1 (s, x, instr);
	}
	else if (aop == 1 && aopcde == 17)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };

        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };

        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A0 };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = REG_A1 };
		amod1 (s, x, instr);
	}
	else if (aop == 0 && aopcde == 18)
	{
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SAA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
		aligndir (s, instr);
	}
	else if (aop == 3 && aopcde == 18) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_DISALGNEXCPT };
    }
	else if (aop == 0 && aopcde == 20)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP1P };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
		aligndir (s, instr);
	}
	else if (aop == 1 && aopcde == 20)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP1P };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_OPENP };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_TMOD };
        instr.flags.T = true;
        if (s) {
            instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RMOD };
            instr.flags.R = true;
        }
	}
	else if (aop == 0 && aopcde == 21)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP16P };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
		aligndir (s, instr);
	}
	else if (aop == 1 && aopcde == 21)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEOP16M };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src1) };
		aligndir (s, instr);
	}
	else if (aop == 2 && aopcde == 7)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ABS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
	}
	else if (aop == 1 && aopcde == 7)
	{
        instr.operation = OP_MIN;
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_MIN };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
	}
	else if (aop == 0 && aopcde == 7)
	{
        instr.operation = OP_MAX;
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_MAX };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
	}
	else if (aop == 2 && aopcde == 6)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ABS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (aop == 1 && aopcde == 6)
	{
        instr.operation = OP_MIN;
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_MIN };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (aop == 0 && aopcde == 6)
	{
        instr.operation = OP_MAX;
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_MAX };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_VMOD };
        instr.flags.V = true;
	}
	else if (HL == 1 && aopcde == 1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSORMINUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };

        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };

        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_MINUSORPLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };

		amod0amod2 (s, x, aop, instr);
	}
	else if (aop == 0 && aopcde == 4)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };

		amod1 (s, x, instr);
	}
	else if (aop == 0 && aopcde == 0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_PLUSORPLUS };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
		amod0 (s, x, instr);
	}
	else if (aop == 0 && aopcde == 24)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEPACK };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src1) };
	}
	else if (aop == 1 && aopcde == 24)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_BYTEUNPACK };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0 + 1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COLON };
        instr.operands[instr.operand_count++] = { .cls = IMM, .imm = imm5(src0) };
		aligndir (s, instr);
	}
	else if (aopcde == 13)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst1) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(dst0) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SEARCH };
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = dregs(src0) };
		searchmod (aop, instr);
	}
	else {
        instr.operation = OP_UNSUPPORTED;
		return false;
    }

	return true;
}
