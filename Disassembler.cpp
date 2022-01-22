#include "Disassembler.h"
#include "arch_blackfin.h"
#include "binaryninjaapi.h"
#include <stdint.h>

#define LOG BinaryNinja::LogInfo

uint16_t get_instruction_word(uint32_t instructionCandidate, bool canonical_high) {
    if (canonical_high) return instructionCandidate & 0x0000ffff;
    else return (instructionCandidate & 0xffff0000) >> 16;
}

uint32_t get_uint32_be(uint8_t *data, int addr) {
    // FIXME: disgusting endianess confusion here.
    return (data[addr + 3] << 24) | (data[addr + 2] << 16) | (data[addr + 1] << 8) | (data[addr]);
}

static int _disasm_insn_bfin(uint32_t instructionCandidate, uint32_t addr, blackfin::Instruction &result, bool parallel)
{
	uint32_t iw0;
	uint32_t iw1;
	int rv = 0;

    iw0 = get_instruction_word(instructionCandidate, true);
    iw1 = get_instruction_word(instructionCandidate, false);
    // LOG("InstructionCandidate: %08x", instructionCandidate);

    // BinaryNinja::LogInfo("iw0: %04x, iw1: %04x", iw0, iw1);

    result.operation = blackfin::OP_ILLEGAL;
    	
	if ((iw0 & 0xf7ff) == 0xc003 && iw1 == 0x1800)
	{
		if (parallel)
		{
            //ILLEGAL
            result.operation = blackfin::OP_ILLEGAL;
			return 0;
		}

        // MNOP, just return length (do we want MNOPS/NOPS in the disasm? probably)
        result.operation = blackfin::OP_NOP;
		return 4;
	}
	else if ((iw0 & 0xff00) == 0x0000) {
        result.opcode = iw0;
        result.length = 2;
        ProgCtrlInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xffc0) == 0x0240) {
        result.opcode = iw0;
        result.length = 2;
        CacheCtrlInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xff80) == 0x0100) {
        result.opcode = iw0;
        result.length = 2;
        PushPopInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xfe00) == 0x0400) {
        result.opcode = iw0;
        result.length = 2;
        PushPopMultipleInstruction::Disassemble(iw0, result, parallel);
    } 
	else if ((iw0 & 0xfe00) == 0x0600) {
        result.opcode = iw0;
        result.length = 2;
        CCMoveInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xf800) == 0x0800) {
        result.opcode = iw0;
        result.length = 2;
        CCFlagInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xffe0) == 0x0200) {
        result.opcode = iw0;
        result.length = 2;
        CC2DregInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xff00) == 0x0300) {
        result.opcode = iw0;
        result.length = 2;
        CC2StatInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xf000) == 0x1000) {
        result.opcode = iw0;
        result.length = 2;
        BranchCCInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xf000) == 0x2000) {
        result.opcode = iw0;
        result.length = 2;
        UJumpInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xf000) == 0x3000) {
        result.opcode = iw0;
        result.length = 2;
        RegMvInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xfc00) == 0x4000) {
        result.opcode = iw0;
        result.length = 2;
        ALU2opInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xfe00) == 0x4400) {
        result.opcode = iw0;
        result.length = 2;
        Ptr2OpInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xf800) == 0x4800) {
        result.opcode = iw0;
        result.length = 2;
        Logic2OpInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xf000) == 0x5000) {
        result.opcode = iw0;
        result.length = 2;
        Comp3OpInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xf800) == 0x6000) {
        result.opcode = iw0;
        result.length = 2;
        CompI2OpDInstruction::Disassemble(iw0, result, parallel);
		// rv = decode_COMPI2opD_0 (iw0, outf);
    }
	else if ((iw0 & 0xf800) == 0x6800) {
        result.opcode = iw0;
        result.length = 2;
        CompI2OpPInstruction::Disassemble(iw0, result, parallel);
    } 
	else if ((iw0 & 0xf000) == 0x8000) {
        result.opcode = iw0;
        result.length = 2;
        LoadStorePmodInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xff60) == 0x9e60) {
        result.opcode = iw0;
        result.length = 2;
        DagModIRegMRegInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xfff0) == 0x9f60) {
        result.opcode = iw0;
        result.length = 2;
        DagModIRegImmInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xfc00) == 0x9c00) {
        result.opcode = iw0;
        result.length = 2;
        DspLoadStoreInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xf000) == 0x9000) {
        result.opcode = iw0;
        result.length = 2;
        LoadStoreInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xfc00) == 0xb800) {
        result.opcode = iw0;
        result.length = 2;
        LoadStoreFPOffsetInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xe000) == 0xA000) {
        result.opcode = iw0;
        result.length = 2;
        LoadStoreOffsetInstruction::Disassemble(iw0, result, parallel);
    }
	else if ((iw0 & 0xff80) == 0xe080 && (iw1 & 0x0C00) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        LoopSetupInstruction::Disassemble(iw0, iw1, result, parallel);
    }
	else if ((iw0 & 0xff00) == 0xe100 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        LdImmHwInstruction::Disassemble(iw0, iw1, result, parallel);
    }
	else if ((iw0 & 0xfe00) == 0xe200 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        JumpCallLongInstruction::Disassemble(iw0, iw1, result, parallel);
    }
	else if ((iw0 & 0xfc00) == 0xe400 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        LoadStoreOffset16Instruction::Disassemble(iw0, iw1, result, parallel);
    }
	else if ((iw0 & 0xfffe) == 0xe800 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        LinkageInstruction::Disassemble(iw0, iw1, result, parallel);
    }
	else if ((iw0 & 0xf600) == 0xc000 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        Dsp32MuAcInstruction::Disassemble(iw0, iw1, result, parallel);
    }
	else if ((iw0 & 0xf600) == 0xc200 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        Dsp32MultInstruction::Disassemble(iw0, iw1, result, parallel);
    }
	else if ((iw0 & 0xf7c0) == 0xc400 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        result.operation = blackfin::OP_UNSUPPORTED;
		// rv = decode_dsp32alu_0 (iw0, iw1, outf);
    }
	else if ((iw0 & 0xf780) == 0xc600 && (iw1 & 0x01c0) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        result.operation = blackfin::OP_UNSUPPORTED;
		// rv = decode_dsp32shift_0 (iw0, iw1, outf);
    }
	else if ((iw0 & 0xf780) == 0xc680 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        result.operation = blackfin::OP_UNSUPPORTED;
		// rv = decode_dsp32shiftimm_0 (iw0, iw1, outf);
    }
	else if ((iw0 & 0xff00) == 0xf800) {
        result.opcode = iw0;
        result.length = 2;
        result.operation = blackfin::OP_UNSUPPORTED;
		// rv = decode_pseudoDEBUG_0 (iw0, outf);
    }
	else if ((iw0 & 0xFF00) == 0xF900) {
        result.opcode = iw0;
        result.length = 2;
        result.operation = blackfin::OP_UNSUPPORTED;
		// rv = decode_pseudoOChar_0 (iw0, outf);
    }
	else if ((iw0 & 0xFF00) == 0xf000 && (iw1 & 0x0000) == 0x0000) {
        result.opcode = (iw0 << 16) + iw1;
        result.length = 4;
        result.operation = blackfin::OP_UNSUPPORTED;
		// rv = decode_pseudodbg_assert_0 (iw0, iw1, outf);
    } else {
        result.operation = blackfin::OP_ILLEGAL;
    }

	if (result.length == 0)
	{
        result.operation = blackfin::OP_ILLEGAL;
        return 0;
	}

	return result.length;
}

int disasm_insn_bfin(uint8_t *data, uint32_t addr, blackfin::Instruction &instr)
{
	uint8_t buf[4];
	uint32_t iw1;
	uint32_t iw0;
	int count = 0;
    int parallel = 0;

    for (int i = 0; i < 4; i ++) {
        // LOG("%02x", data[i]);
    }

	uint32_t instructionCandidate = get_uint32_be(data, 0);
    // LOG("instructionCandidate: %08x", instructionCandidate);
	iw0 = get_instruction_word(instructionCandidate, true);
	iw1 = get_instruction_word(instructionCandidate, false);

    count += _disasm_insn_bfin(instructionCandidate, addr, instr, parallel = false);

    /* FIXME: Disabling parallel instruction display for now,
     * shouldn't matter to binja IL, will add some kind
     * of marker for them later 
     */
	/* Proper display of multiple issue instructions.  */
    /*
	if (count == 4 && (iw0 & 0xc000) == 0xc000 && (iw0 & BIT_MULTI_INS)
	&& ((iw0 & 0xe800) != 0xe800 )) // Not Linkage
	{
		int legal = 1;
		int len;

		parallel = 1;
		OUTS (outf, " || ");
		len = _print_insn_bfin (pc + 4, outf, data_buf);
		OUTS (outf, " || ");
		
		if (len != 2)
			legal = 0;
		
		len = _print_insn_bfin (pc + 6, outf, data_buf);
		
		if (len != 2)
			legal = 0;

		if (legal)
			count = 8;
		else
		{
			OUTC (outf, "ILLEGAL PARALLEL INSTRUCTION");
			SET_FEATURE(outf,CF_STOP);
			count = 0;
		}

		parallel = 0;
	}
    */

	if (count == 0) return 2;
	return count;
}
