#include "RegMv.h"

bool RegMvInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel)
{
	int gs  = ((instructionWord >> RegMv_gs_bits) & RegMv_gs_mask);
	int gd  = ((instructionWord >> RegMv_gd_bits) & RegMv_gd_mask);
	int src = ((instructionWord >> RegMv_src_bits) & RegMv_src_mask);
	int dst = ((instructionWord >> RegMv_dst_bits) & RegMv_dst_mask);
    
    // This logic is a bit complicated so I'm just ripping exactly what libopcodes 
    // has for it and replacing the gotos with a bool
    
    bool valid = false;

	/* Reserved slots cannot be a src/dst.  */
	if (IS_RESERVEDREG (gs, src) || IS_RESERVEDREG (gd, dst))
        return false;

	/* Standard register moves  */
	if ((gs < 2) ||                               /* Dregs/Pregs as source  */
		(gd < 2) ||                               /* Dregs/Pregs as dest    */
		(gs == 4 && src < 4) ||                   /* Accumulators as source */
		(gd == 4 && dst < 4 && (gs < 4)) ||       /* Accumulators as dest   */
		(gs == 7 && src == 7 && !(gd == 4 && dst < 4)) || /* EMUDAT as src  */
		(gd == 7 && dst == 7))                    /* EMUDAT as dest         */
        valid = true;

	/* dareg = dareg (IMBL) */
	if (gs < 4 && gd < 4)
        valid = true;

	/* USP can be src to sysregs, but not dagregs.  */
	if ((gs == 7 && src == 0) && (gd >= 4))
        valid = true;

	/* USP can move between genregs (only check Accumulators).  */
	if (((gs == 7 && src == 0) && (gd == 4 && dst < 4)) ||
	((gd == 7 && dst == 0) && (gs == 4 && src < 4)))
        valid = true;

	/* Still here ?  Invalid reg pair.  */
    if (!valid) return false;

    instr.operation = blackfin::OP_MV;
    instr.operand_count = 3;
    instr.operands[0] = { .cls = blackfin::REG,      .reg = allregs(dst, gd) };
    instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
    instr.operands[2] = { .cls = blackfin::REG,      .reg = allregs(src, gs) };

	return 2;
}
