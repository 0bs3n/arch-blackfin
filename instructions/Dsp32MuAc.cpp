#include "Dsp32MuAc.h"
#include "Dsp32.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
Dsp32MuAcInstruction::Disassemble(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel) 
{
	/* dsp32mac
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 0 | 0 |.M.| 0 | 0 |.mmod..........|.MM|.P.|.w1|.op1...|
	|.h01|.h11|.w0|.op0...|.h00|.h10|.dst.......|.src0......|.src1..|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int op1  = ((instructionWordHigh >> (DSP32Mac_op1_bits - 16)) & DSP32Mac_op1_mask);
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
	int op0  = ((instructionWordLow >> DSP32Mac_op0_bits) & DSP32Mac_op0_mask);
	int h11  = ((instructionWordLow >> DSP32Mac_h11_bits) & DSP32Mac_h11_mask);
	int h01  = ((instructionWordLow >> DSP32Mac_h01_bits) & DSP32Mac_h01_mask);

	if (w0 == 0 && w1 == 0 && op1 == 3 && op0 == 3)
		return false;

	if (op1 == 3 && MM)
		return false;

	if ((w1 || w0) && mmod == M_W32)
		return false;

	if (((1 << mmod) & (P ? 0x131b : 0x1b5f)) == 0)
		return false;

    instr.operation = OP_DSPMAC;
    instr.operand_count = 0;
	if (w1 == 1 || op1 != 3)
	{
		if (w1) {
            instr.operands[instr.operand_count] = { .cls = REG, .reg = P ? dregs(dst + 1) : dregs_hi(dst) };
            instr.operand_count = instr.operand_count + 1;
			// OUTS (outf, P ? dregs (dst + 1) : dregs_hi (dst));
        }

		if (op1 == 3) {
            instr.operands[instr.operand_count] = { .cls = OPERATOR, .operat = OPER_EQ };
            instr.operands[instr.operand_count + 1] = { .cls = REG, .reg = REG_A1 };
            instr.operand_count = instr.operand_count + 2;
			// OUTS (outf, " = A1");
        }
		else
		{
			if (w1) {
                instr.operands[instr.operand_count] = { .cls = OPERATOR, .operat = OPER_EQ };
                instr.operands[instr.operand_count + 1] = { .cls = OPERATOR, .operat = OPER_OPENP };
                instr.operand_count = instr.operand_count + 2;
				// OUTS (outf, " = (");
            }

			decode_macfunc (1, op1, h01, h11, src0, src1, instr);
			if (w1) {
                instr.operands[instr.operand_count] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
                instr.operand_count = instr.operand_count + 1;
				// OUTS (outf, ")");
            }
		}

		if (w0 == 1 || op0 != 3)
		{
			if (MM) {
                instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_MFLAG };
                instr.operand_count += 1;
                instr.flags.M = true;
				// OUTS (outf, " (M)");
            }
            instr.operands[instr.operand_count] = { .cls = OPERATOR, .operat = OPER_COMMA };
            instr.operand_count += 1;
			// OUTS (outf, ", ");
		}
	}

	if (w0 == 1 || op0 != 3)
	{
		/* Clear MM option since it only matters for MAC1, and if we made
		it this far, we've already shown it or we want to ignore it.  */
		MM = 0;

		if (w0) {
            instr.operands[instr.operand_count] = { .cls = REG, .reg = P ? dregs(dst) : dregs_lo(dst) };
            instr.operand_count += 1;
			// OUTS (outf, P ? dregs (dst) : dregs_lo (dst));
        }

		if (op0 == 3) {
            instr.operands[instr.operand_count] = { .cls = OPERATOR, .operat = OPER_EQ };
            instr.operands[instr.operand_count + 1] = { .cls = REG, .reg = REG_A0 };
            instr.operand_count += 2;
			// OUTS (outf, " = A0");
        }
		else
		{
			if (w0) {
                instr.operands[instr.operand_count] = { .cls = OPERATOR, .operat = OPER_EQ };
                instr.operands[instr.operand_count + 1] = { .cls = OPERATOR, .operat = OPER_OPENP };
                instr.operand_count += 2;
				// OUTS (outf, " = (");
            }
			
			decode_macfunc (0, op0, h00, h10, src0, src1, instr);
			
			if (w0) {
                instr.operands[instr.operand_count] = { .cls = OPERATOR, .operat = OPER_CLOSEP };
				// OUTS (outf, ")");
            }
		}
	}

	decode_optmode (mmod, MM, instr);

	return 4;
}
