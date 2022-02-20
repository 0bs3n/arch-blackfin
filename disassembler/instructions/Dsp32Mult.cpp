#include "Instructions.h"
#include "Dsp32.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
BlackfinInstruction::DisassembleDsp32Mult(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel) 
{
	/* dsp32mult
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 0 | 0 |.M.| 0 | 1 |.mmod..........|.MM|.P.|.w1|.op1...|
	|.h01|.h11|.w0|.op0...|.h00|.h10|.dst.......|.src0......|.src1..|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int w1   = ((instructionWordHigh >> (DSP32Mac_w1_bits - 16)) & DSP32Mac_w1_mask);
	int P    = ((instructionWordHigh >> (DSP32Mac_p_bits - 16)) & DSP32Mac_p_mask);
	int MM   = ((instructionWordHigh >> (DSP32Mac_MM_bits - 16)) & DSP32Mac_MM_mask);
	int mmod = ((instructionWordHigh >> (DSP32Mac_mmod_bits - 16)) & DSP32Mac_mmod_mask);
	int w0   = ((instructionWordLow >> DSP32Mac_w0_bits) & DSP32Mac_w0_mask);
	int src0 = ((instructionWordLow >> DSP32Mac_src0_bits) & DSP32Mac_src0_mask);
	int src1 = ((instructionWordLow >> DSP32Mac_src1_bits) & DSP32Mac_src1_mask);
	int dst  = ((instructionWordLow >> DSP32Mac_dst_bits) & DSP32Mac_dst_mask);
	int h10  = ((instructionWordLow >> DSP32Mac_h10_bits) & DSP32Mac_h10_mask);
	int h00  = ((instructionWordLow >> DSP32Mac_h00_bits) & DSP32Mac_h00_mask);
	int h11  = ((instructionWordLow >> DSP32Mac_h11_bits) & DSP32Mac_h11_mask);
	int h01  = ((instructionWordLow >> DSP32Mac_h01_bits) & DSP32Mac_h01_mask);

	if (w1 == 0 && w0 == 0)
		return 0;

    // what is this? from libopcodes 
	if (((1 << mmod) & (P ? 0x313 : 0x1b57)) == 0)
		return 0;

    instr.operand_count = 0;
    instr.operation = OP_DSPMUL;

	if (w1)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = P ? dregs(dst + 1) : dregs_hi(dst) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
		// OUTS (outf, P ? dregs (dst + 1) : dregs_hi (dst));
		// OUTS (outf, " = ");
		decode_multfunc(h01, h11, src0, src1, instr);

		if (w0)
		{
			if (MM) {
                instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_MFLAG };
				// OUTS (outf, " (M)");
            }

			MM = 0;
            instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_COMMA };
			// OUTS (outf, ", ");
		}
	}

	if (w0)
	{
        instr.operands[instr.operand_count++] = { .cls = REG, .reg = P ? dregs(dst) : dregs_lo(dst) };
        instr.operands[instr.operand_count++] = { .cls = OPERATOR, .operat = OPER_EQ };
		// OUTS (outf, P ? dregs (dst) : dregs_lo (dst));
		// OUTS (outf, " = ");
		decode_multfunc (h00, h10, src0, src1, instr);
	}

	decode_optmode (mmod, MM, instr);
	return true;
}
