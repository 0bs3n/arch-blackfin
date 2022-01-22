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
    /*
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
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = BYTEOP2P (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ") (RNDH");
		if (s == 1)
			OUTS (outf, ", R)");
		else
			OUTS (outf, ")");
	}
	else if (aop == 0 && aopcde == 22 && HL == 0)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = BYTEOP2P (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ") (RNDL");
		if (s == 1)
			OUTS (outf, ", R)");
		else
			OUTS (outf, ")");
	}
	else if (aop == 0 && s == 0 && aopcde == 8)
		OUTS (outf, "A0 = 0");
	else if (aop == 0 && s == 1 && aopcde == 8)
		OUTS (outf, "A0 = A0 (S)");
	else if (aop == 1 && s == 0 && aopcde == 8)
		OUTS (outf, "A1 = 0");
	else if (aop == 1 && s == 1 && aopcde == 8)
		OUTS (outf, "A1 = A1 (S)");
	else if (aop == 2 && s == 0 && aopcde == 8)
		OUTS (outf, "A1 = A0 = 0");
	else if (aop == 2 && s == 1 && aopcde == 8)
		OUTS (outf, "A1 = A1 (S), A0 = A0 (S)");
	else if (aop == 3 && s == 0 && aopcde == 8)
		OUTS (outf, "A0 = A1");
	else if (aop == 3 && s == 1 && aopcde == 8)
		OUTS (outf, "A1 = A0");
	else if (aop == 1 && aopcde == 9 && s == 0)
	{
		OUTS (outf, "A0.X = ");
		OUTS (outf, dregs_lo (src0));
	}
	else if (aop == 1 && HL == 0 && aopcde == 11)
	{
		OUTS (outf, dregs_lo (dst0));
		OUTS (outf, " = (A0 += A1)");
	}
	else if (aop == 3 && HL == 0 && aopcde == 16)
		OUTS (outf, "A1 = ABS A1, A0 = ABS A0");
	else if (aop == 0 && aopcde == 23 && HL == 1)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = BYTEOP3P (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ") (HI");

		if (s == 1)
			OUTS (outf, ", R)");
		else
			OUTS (outf, ")");
	}
	else if (aop == 3 && aopcde == 9 && s == 0)
	{
		OUTS (outf, "A1.X = ");
		OUTS (outf, dregs_lo (src0));
	}
	else if (aop == 1 && HL == 1 && aopcde == 16)
		OUTS (outf, "A1 = ABS A1");
	else if (aop == 0 && HL == 1 && aopcde == 16)
		OUTS (outf, "A1 = ABS A0");
	else if (aop == 2 && aopcde == 9 && s == 1)
	{
		OUTS (outf, "A1 = ");
		OUTS (outf, dregs (src0));
	}
	else if (HL == 0 && aop == 3 && aopcde == 12)
	{
		OUTS (outf, dregs_lo (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " (RND)");
	}
	else if (aop == 1 && HL == 0 && aopcde == 16)
		OUTS (outf, "A0 = ABS A1");
	else if (aop == 0 && HL == 0 && aopcde == 16)
		OUTS (outf, "A0 = ABS A0");
	else if (aop == 3 && HL == 0 && aopcde == 15)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = -");
		OUTS (outf, dregs (src0));
		OUTS (outf, " (V)");
	}
	else if (aop == 3 && s == 1 && HL == 0 && aopcde == 7)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = -");
		OUTS (outf, dregs (src0));
		OUTS (outf, " (S)");
	}
	else if (aop == 3 && s == 0 && HL == 0 && aopcde == 7)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = -");
		OUTS (outf, dregs (src0));
		OUTS (outf, " (NS)");
	}
	else if (aop == 1 && HL == 1 && aopcde == 11)
	{
		OUTS (outf, dregs_hi (dst0));
		OUTS (outf, " = (A0 += A1)");
	}
	else if (aop == 2 && aopcde == 11 && s == 0)
		OUTS (outf, "A0 += A1");
	else if (aop == 2 && aopcde == 11 && s == 1)
		OUTS (outf, "A0 += A1 (W32)");
	else if (aop == 3 && HL == 0 && aopcde == 14)
		OUTS (outf, "A1 = -A1, A0 = -A0");
	else if (HL == 1 && aop == 3 && aopcde == 12)
	{
		OUTS (outf, dregs_hi (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " (RND)");
	}
	else if (aop == 0 && aopcde == 23 && HL == 0)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = BYTEOP3P (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ") (LO");
		if (s == 1)
			OUTS (outf, ", R)");
		else
			OUTS (outf, ")");
	}
	else if (aop == 0 && HL == 0 && aopcde == 14)
		OUTS (outf, "A0 = -A0");
	else if (aop == 1 && HL == 0 && aopcde == 14)
		OUTS (outf, "A0 = -A1");
	else if (aop == 0 && HL == 1 && aopcde == 14)
		OUTS (outf, "A1 = -A0");
	else if (aop == 1 && HL == 1 && aopcde == 14)
		OUTS (outf, "A1 = -A1");
	else if (aop == 0 && aopcde == 12)
	{
		OUTS (outf, dregs_hi (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs_lo (dst0));
		OUTS (outf, " = SIGN (");
		OUTS (outf, dregs_hi (src0));
		OUTS (outf, ") * ");
		OUTS (outf, dregs_hi (src1));
		OUTS (outf, " + SIGN (");
		OUTS (outf, dregs_lo (src0));
		OUTS (outf, ") * ");
		OUTS (outf, dregs_lo (src1));
	}
	else if (aop == 2 && aopcde == 0)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " -|+ ");
		OUTS (outf, dregs (src1));
		amod0 (s, x, outf);
	}
	else if (aop == 1 && aopcde == 12)
	{
		OUTS (outf, dregs (dst1));
		OUTS (outf, " = A1.L + A1.H, ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = A0.L + A0.H");
	}
	else if (aop == 2 && aopcde == 4)
	{
		OUTS (outf, dregs (dst1));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " + ");
		OUTS (outf, dregs (src1));
		OUTS (outf, ", ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " - ");
		OUTS (outf, dregs (src1));
		amod1 (s, x, outf);
	}
	else if (HL == 0 && aopcde == 1)
	{
		OUTS (outf, dregs (dst1));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " +|+ ");
		OUTS (outf, dregs (src1));
		OUTS (outf, ", ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " -|- ");
		OUTS (outf, dregs (src1));
		amod0amod2 (s, x, aop, outf);
	}
	else if (aop == 0 && aopcde == 11)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = (A0 += A1)");
	}
	else if (aop == 0 && aopcde == 10)
	{
		OUTS (outf, dregs_lo (dst0));
		OUTS (outf, " = A0.X");
	}
	else if (aop == 1 && aopcde == 10)
	{
		OUTS (outf, dregs_lo (dst0));
		OUTS (outf, " = A1.X");
	}
	else if (aop == 1 && aopcde == 0)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " +|- ");
		OUTS (outf, dregs (src1));
		amod0 (s, x, outf);
	}
	else if (aop == 3 && aopcde == 0)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " -|- ");
		OUTS (outf, dregs (src1));
		amod0 (s, x, outf);
	}
	else if (aop == 1 && aopcde == 4)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " - ");
		OUTS (outf, dregs (src1));
		amod1 (s, x, outf);
	}
	else if (aop == 0 && aopcde == 17)
	{
		OUTS (outf, dregs (dst1));
		OUTS (outf, " = A1 + A0, ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = A1 - A0");
		amod1 (s, x, outf);
	}
	else if (aop == 1 && aopcde == 17)
	{
		OUTS (outf, dregs (dst1));
		OUTS (outf, " = A0 + A1, ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = A0 - A1");
		amod1 (s, x, outf);
	}
	else if (aop == 0 && aopcde == 18)
	{
		OUTS (outf, "SAA (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ")");
		aligndir (s, outf);
	}
	else if (aop == 3 && aopcde == 18)
		OUTS (outf, "DISALGNEXCPT");
	else if (aop == 0 && aopcde == 20)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = BYTEOP1P (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ")");
		aligndir (s, outf);
	}
	else if (aop == 1 && aopcde == 20)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = BYTEOP1P (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ") (T");
		if (s == 1)
			OUTS (outf, ", R)");
		else
			OUTS (outf, ")");
	}
	else if (aop == 0 && aopcde == 21)
	{
		OUTS (outf, "(");
		OUTS (outf, dregs (dst1));
		OUTS (outf, ", ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, ") = BYTEOP16P (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ")");
		aligndir (s, outf);
	}
	else if (aop == 1 && aopcde == 21)
	{
		OUTS (outf, "(");
		OUTS (outf, dregs (dst1));
		OUTS (outf, ", ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, ") = BYTEOP16M (");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src1));
		OUTS (outf, ")");
		aligndir (s, outf);
	}
	else if (aop == 2 && aopcde == 7)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ABS ");
		OUTS (outf, dregs (src0));
	}
	else if (aop == 1 && aopcde == 7)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = MIN (");
		OUTS (outf, dregs (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1));
		OUTS (outf, ")");
	}
	else if (aop == 0 && aopcde == 7)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = MAX (");
		OUTS (outf, dregs (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1));
		OUTS (outf, ")");
	}
	else if (aop == 2 && aopcde == 6)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ABS ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " (V)");
	}
	else if (aop == 1 && aopcde == 6)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = MIN (");
		OUTS (outf, dregs (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1));
		OUTS (outf, ") (V)");
	}
	else if (aop == 0 && aopcde == 6)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = MAX (");
		OUTS (outf, dregs (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1));
		OUTS (outf, ") (V)");
	}
	else if (HL == 1 && aopcde == 1)
	{
		OUTS (outf, dregs (dst1));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " +|- ");
		OUTS (outf, dregs (src1));
		OUTS (outf, ", ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " -|+ ");
		OUTS (outf, dregs (src1));
		amod0amod2 (s, x, aop, outf);
	}
	else if (aop == 0 && aopcde == 4)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " + ");
		OUTS (outf, dregs (src1));
		amod1 (s, x, outf);
	}
	else if (aop == 0 && aopcde == 0)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " +|+ ");
		OUTS (outf, dregs (src1));
		amod0 (s, x, outf);
	}
	else if (aop == 0 && aopcde == 24)
	{
		OUTS (outf, dregs (dst0));
		OUTS (outf, " = BYTEPACK (");
		OUTS (outf, dregs (src0));
		OUTS (outf, ", ");
		OUTS (outf, dregs (src1));
		OUTS (outf, ")");
	}
	else if (aop == 1 && aopcde == 24)
	{
		OUTS (outf, "(");
		OUTS (outf, dregs (dst1));
		OUTS (outf, ", ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, ") = BYTEUNPACK ");
		OUTS (outf, dregs (src0 + 1));
		OUTS (outf, ":");
		OUTS (outf, imm5d (src0));
		aligndir (s, outf);
	}
	else if (aopcde == 13)
	{
		OUTS (outf, "(");
		OUTS (outf, dregs (dst1));
		OUTS (outf, ", ");
		OUTS (outf, dregs (dst0));
		OUTS (outf, ") = SEARCH ");
		OUTS (outf, dregs (src0));
		OUTS (outf, " (");
		searchmod (aop, outf);
		OUTS (outf, ")");
	}
	else
		return 0;

	return 4;
*/
}
