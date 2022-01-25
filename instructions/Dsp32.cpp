#include "Dsp32.h"

using namespace blackfin;

void decode_optmode (int mod, int MM, struct blackfin::Instruction &instr)
{
	if (mod == 0 && MM == 0)
		return;

	if (MM && !mod)
	{
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_MFLAG };
        instr.flags.M = true;
        instr.operand_count += 1;
		// OUTS (outf, "M)");
		return;
	}

	if (MM) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_MFLAG };
        instr.operand_count += 1;
        instr.flags.M = true;
		// OUTS (outf, "M, ");
    }

	if (mod == M_S2RND) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_S2RNDFLAG };
        instr.operand_count += 1;
        instr.flags.S2RND = true;
		// OUTS (outf, "S2RND");
    } else if (mod == M_T) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_TFLAG };
        instr.operand_count += 1;
        instr.flags.T = true;
		// OUTS (outf, "T");
    }
	else if (mod == M_W32) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_W32FLAG };
        instr.operand_count += 1;
        instr.flags.W32 = true;
		// OUTS (outf, "W32");
    }
	else if (mod == M_FU) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_FUFLAG };
        instr.operand_count += 1;
        instr.flags.FU = true;
		// OUTS (outf, "FU");
    }
	else if (mod == M_TFU) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_TFUFLAG };
        instr.operand_count += 1;
        instr.flags.TFU = true;
		// OUTS (outf, "TFU");
    }
	else if (mod == M_IS) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_ISFLAG };
        instr.operand_count += 1;
        instr.flags.IS = true;
		// OUTS (outf, "IS");
    }
	else if (mod == M_ISS2) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_ISS2FLAG };
        instr.operand_count += 1;
        instr.flags.ISS2 = true;
		// OUTS (outf, "ISS2");
    }
	else if (mod == M_IH) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_IHFLAG };
        instr.operand_count += 1;
        instr.flags.IH = true;
		// OUTS (outf, "IH");
    }
	else if (mod == M_IU) {
        instr.operands[instr.operand_count] = { .cls = MNEMOMIC, .mnemonic = OL_IUFLAG };
        instr.operand_count += 1;
        instr.flags.IU = true;
		// OUTS (outf, "IU");
    }
	else
		abort ();
}

int decode_multfunc (int h0, int h1, int src0, int src1, struct blackfin::Instruction &instr)
{
    enum Register s0, s1;

	if (h0)
		s0 = dregs_hi(src0);
	else
		s0 = dregs_lo(src0);

	if (h1)
		s1 = dregs_hi(src1);
	else
		s1 = dregs_lo(src1);

    instr.operands[instr.operand_count] = { .cls = REG, .reg = s0 };
    instr.operands[instr.operand_count + 1] = { .cls = OPERATOR, .operat = OPER_MUL };
    instr.operands[instr.operand_count + 2] = { .cls = REG, .reg = s1 };
    instr.operand_count = instr.operand_count + 3;
	// OUTS (outf, s0);
	// OUTS (outf, " * ");
	// OUTS (outf, s1);
	return 0;
}

int decode_macfunc (int which, int op, int h0, int h1, int src0, int src1, struct blackfin::Instruction &instr)
{
	// const char *a;
	// const char *sop = "<unknown op>";
    enum Operator sop;

    enum Register a;

	if (which)
        a = REG_A1;
		// a = "A1";
	else
        a = REG_A0;
		// a = "A0";

	if (op == 3)
	{
        instr.operands[instr.operand_count] = { .cls = REG, .reg = a };
        instr.operand_count = instr.operand_count + 1;
		// OUTS (outf, a);
		return 0;
	}

	switch (op)
	{
		case 0: sop = OPER_EQ;      break;
		case 1: sop = OPER_PLUSEQ;  break;
		case 2: sop = OPER_MINUSEQ; break;
		default: break;
	}

    instr.operands[instr.operand_count] = { .cls = REG, .reg = a };
    instr.operands[instr.operand_count + 1] = { .cls = OPERATOR, .operat = sop };
    instr.operand_count = instr.operand_count + 2;
	// OUTS (outf, a);
	// OUTS (outf, sop);
	decode_multfunc (h0, h1, src0, src1, instr);

	return 0;
}

void amod0 (int s0, int x0, struct blackfin::Instruction &instr)
{
	if (s0 == 1 && x0 == 0) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
		// OUTS (outf, " (S)");
    }
	else if (s0 == 0 && x0 == 1) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_COMOD };
        instr.flags.CO = true;
		// OUTS (outf, " (CO)");
    }
	else if (s0 == 1 && x0 == 1) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SCOMOD };
        instr.flags.SCO = true;
		// OUTS (outf, " (SCO)");
    }
}

void amod1 (int s0, int x0, struct blackfin::Instruction &instr)
{
	if (s0 == 0 && x0 == 0) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_NSMOD };
        instr.flags.NS = true;
		// OUTS (outf, " (NS)");
    }
	else if (s0 == 1 && x0 == 0) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
		// OUTS (outf, " (S)");
    }
}

void amod0amod2 (int s0, int x0, int aop0, struct blackfin::Instruction &instr)
{
	if (s0 == 1 && x0 == 0 && aop0 == 0) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.flags.S = true;
		// OUTS (outf, " (S)");
    }
	else if (s0 == 0 && x0 == 1 && aop0 == 0) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_COMOD };
        instr.flags.CO = true;
		// OUTS (outf, " (CO)");
    }
	else if (s0 == 1 && x0 == 1 && aop0 == 0) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SCOMOD };
        instr.flags.SCO = true;
		// OUTS (outf, " (SCO)");
    }
	else if (s0 == 0 && x0 == 0 && aop0 == 2) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASRMOD };
        instr.flags.ASR = true;
		// OUTS (outf, " (ASR)");
    }
	else if (s0 == 1 && x0 == 0 && aop0 == 2) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASRMOD };
        instr.flags.S = true;
        instr.flags.ASR = true;
		// OUTS (outf, " (S, ASR)");
    }
	else if (s0 == 0 && x0 == 1 && aop0 == 2) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_COMOD };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASRMOD };
        instr.flags.CO = true;
        instr.flags.ASR = true;
		// OUTS (outf, " (CO, ASR)");
    }
	else if (s0 == 1 && x0 == 1 && aop0 == 2) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SCOMOD };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASRMOD };
        instr.flags.SCO = true;
        instr.flags.ASR = true;
		// OUTS (outf, " (SCO, ASR)");
    }
	else if (s0 == 0 && x0 == 0 && aop0 == 3) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASLMOD };
        instr.flags.ASL = true;
		// OUTS (outf, " (ASL)");
    }
	else if (s0 == 1 && x0 == 0 && aop0 == 3) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SMOD };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASLMOD };
        instr.flags.S = true;
        instr.flags.ASL = true;
		// OUTS (outf, " (S, ASL)");
    }
	else if (s0 == 0 && x0 == 1 && aop0 == 3) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_COMOD };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASLMOD };
        instr.flags.CO = true;
        instr.flags.ASL = true;
		// OUTS (outf, " (CO, ASL)");
    }
	else if (s0 == 1 && x0 == 1 && aop0 == 3) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_SCOMOD };
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_ASLMOD };
        instr.flags.SCO = true;
        instr.flags.ASL = true;
		// OUTS (outf, " (SCO, ASL)");
    }
}

void searchmod (int r0, struct blackfin::Instruction &instr)
{
	if (r0 == 0) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_GT };
    }
	else if (r0 == 1) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_GE };
    }
	else if (r0 == 2) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_LT };
    }
	else if (r0 == 3) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_LE };
    }
}

void aligndir (int r0, struct blackfin::Instruction &instr)
{
	if (r0 == 1) {
        instr.operands[instr.operand_count++] = { .cls = MNEMOMIC, .mnemonic = OL_RMOD };
        instr.flags.R = true;
		// OUTS (outf, " (R)");
    }
}
