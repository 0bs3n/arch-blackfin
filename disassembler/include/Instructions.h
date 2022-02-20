#include "arch_blackfin.h"

class BlackfinInstruction {
public:
    static bool DisassembleALU2Op(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleBranchCC(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleCacheCtrl(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleCC2Dreg(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleCC2Stat(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleCCFlag(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleCCMove(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleComp3Op(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleCompI2OpD(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleCompI2OpP(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleDagModIRegImm(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleDagModIRegMReg(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleDsp32ALU(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleDsp32MuAc(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleDsp32Mult(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleDsp32Shift(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleDsp32ShiftImm(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleDspLoadStore(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleJumpCallLong(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLoadImmHalfWord(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLinkage(uint16_t instructionWordHigh, u_int16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLoadStoreFPOffset(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLoadStore(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLoadStoreOffset16(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLoadStoreOffset(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLoadStorePmod(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLogic2Op(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleLoopSetup(uint16_t instructionWordHigh, uint16_t InstructionWordLow, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleProgCtrl(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassemblePtr2Op(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassemblePushPop(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassemblePushPopMultiple(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleRegMv(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
    static bool DisassembleUJump(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
};
