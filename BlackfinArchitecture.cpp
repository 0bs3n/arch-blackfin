#include "BlackfinArchitecture.h"
#include "binaryninjaapi.h"
#include "binaryninjacore.h"
#include "lowlevelilinstruction.h"
#include "arch_blackfin.h"
#include "Disassembler.h"
#include "Relocations.h"
#include <string.h>

#define LOG BinaryNinja::LogInfo
using namespace std;
using namespace BinaryNinja;

static BNRegisterInfo RegisterInfo(uint32_t fullWidthReg, size_t offset, size_t size, bool zeroExtend = false)
{
    BNRegisterInfo result;
    result.fullWidthRegister = fullWidthReg;
    result.offset = offset;
    result.size = size;
    result.extend = zeroExtend ? ZeroExtendToFullWidth : NoExtend;
    return result;
}


BlackfinArchitecture::BlackfinArchitecture(const char *name, BNEndianness _endian) : BinaryNinja::Architecture(name) {
    endian = _endian;
}

BNEndianness BlackfinArchitecture::GetEndianness() const { return BNEndianness::LittleEndian; }

size_t BlackfinArchitecture::GetAddressSize() const
{
    return 4;
}

size_t BlackfinArchitecture::GetDefaultIntegerSize() const
{
    return 4;
}

size_t BlackfinArchitecture::GetInstructionAlignment() const
{
    return 2;
}

size_t BlackfinArchitecture::GetMaxInstructionLength() const
{
    return 8;
}

string 
BlackfinArchitecture::GetIntrinsicName(uint32_t intrinsic)
{
    switch (intrinsic)  {
    case BFIN_INTRINSIC_LSETUP:
        return "LoopSetup";
    case BFIN_INTRINSIC_RAISE:
        return "Raise";
    default:
        return "";
    }
}

vector<uint32_t> 
BlackfinArchitecture::GetAllIntrinsics()
{
    return vector<uint32_t> {
        BFIN_INTRINSIC_LSETUP,
        BFIN_INTRINSIC_RAISE
    };
}

vector<NameAndType> BlackfinArchitecture::GetIntrinsicInputs(uint32_t intrinsic)
{
    switch (intrinsic)
    {
    case BFIN_INTRINSIC_LSETUP:
        return {
            NameAndType("loopStart", Type::PointerType(4, Confidence(Type::VoidType(), 0), Confidence(false), Confidence(false), PointerReferenceType)),
            NameAndType("loopEnd", Type::PointerType(4, Confidence(Type::VoidType(), 0), Confidence(false), Confidence(false), PointerReferenceType)),
            NameAndType("loopCount", Type::IntegerType(4, false))
        };
    case BFIN_INTRINSIC_RAISE:
        return {
            NameAndType("interruptNum", Type::IntegerType(4, false))
        };
    default:
        return vector<NameAndType>();
    }
}

vector<Confidence<Ref<Type>>> BlackfinArchitecture::GetIntrinsicOutputs(uint32_t intrinsic)
{
    switch (intrinsic)
    {
    case BFIN_INTRINSIC_LSETUP:
        return vector<Confidence<Ref<Type>>>();
    case BFIN_INTRINSIC_RAISE:
        return vector<Confidence<Ref<Type>>>();
    default:
        return vector<Confidence<Ref<Type>>>();
    }
}

std::string BlackfinArchitecture::GetFlagName(uint32_t flag) { 
    char result[32];
	switch (flag)
	{
	case IL_FLAG_AC0:
		return "ac0";
	case IL_FLAG_AC1:
		return "ac1";
	case IL_FLAG_AN:
		return "an";
	case IL_FLAG_AQ:
		return "aq";
    case IL_FLAG_AV0:
        return "av0";
    case IL_FLAG_AVS0:
        return "avs0";
    case IL_FLAG_AV1:
        return "av1";
    case IL_FLAG_AVS1:
        return "avs1";
	case IL_FLAG_AZ:
		return "az";
    case IL_FLAG_CC:
        return "cc";
	case IL_FLAG_V:
		return "v";
	case IL_FLAG_VS:
		return "vs";
	default:
		sprintf(result, "flag%" PRIu32, flag);
		return result;
	}
}

std::string BlackfinArchitecture::GetFlagWriteTypeName(uint32_t flags) {
    	switch (flags)
	{
		case IL_FLAGWRITE_ALL: return "*";
		case IL_FLAGWRITE_A0: return "a0";
		case IL_FLAGWRITE_A1: return "a1";
		default:
			return "";
	}
}

BNFlagRole BlackfinArchitecture::GetFlagRole(uint32_t flag, uint32_t semClass) {
    switch (flag)
	{
	case IL_FLAG_AC0:
    case IL_FLAG_AC1:
		return CarryFlagRole;
	case IL_FLAG_CC:
    case IL_FLAG_AQ:
		return SpecialFlagRole;
	case IL_FLAG_AZ:
		return ZeroFlagRole;
	case IL_FLAG_V:
    case IL_FLAG_AV0:
    case IL_FLAG_AV1:
    case IL_FLAG_AVS0:
    case IL_FLAG_AVS1:
		return OverflowFlagRole;
	default:
		return SpecialFlagRole;
	}
}

std::vector<uint32_t> BlackfinArchitecture::GetFlagsWrittenByFlagWriteType(uint32_t flags) {
    switch (flags)
	{
	case IL_FLAGWRITE_ALL:
		return vector<uint32_t> { 
            IL_FLAG_AC0, 
            IL_FLAG_AC1, 
            IL_FLAG_AN, 
            IL_FLAG_AQ, 
            IL_FLAG_AV0, 
            IL_FLAG_AVS0, 
            IL_FLAG_AV1, 
            IL_FLAG_AVS1,
            IL_FLAG_AZ,
            IL_FLAG_CC,
            IL_FLAG_V,
            IL_FLAG_VS
        };
	case IL_FLAGWRITE_A0:
		return vector<uint32_t> { IL_FLAG_AC0, IL_FLAG_AV0, IL_FLAG_AVS0, IL_FLAG_AN, IL_FLAG_AQ, IL_FLAG_AZ };
	case IL_FLAGWRITE_A1:
		return vector<uint32_t> { IL_FLAG_AC1, IL_FLAG_AV1, IL_FLAG_AVS1, IL_FLAG_AN, IL_FLAG_AQ, IL_FLAG_AZ };
	default:
		return vector<uint32_t> {};
	}
}

std::vector<uint32_t> BlackfinArchitecture::GetFlagsRequiredForFlagCondition(BNLowLevelILFlagCondition cond, uint32_t semClass) {
    switch (cond)
	{
	case LLFC_E:
	case LLFC_NE:
		return vector<uint32_t>{ IL_FLAG_AZ };
	case LLFC_SLT:
	case LLFC_SGE:
		return vector<uint32_t>{ IL_FLAG_AN, IL_FLAG_V };
	case LLFC_ULT:
	case LLFC_UGE:
		return vector<uint32_t>{ IL_FLAG_AC0, IL_FLAG_AC1 };
	case LLFC_SLE:
	case LLFC_SGT:
		return vector<uint32_t>{ IL_FLAG_AZ, IL_FLAG_AN, IL_FLAG_V, IL_FLAG_AV0, IL_FLAG_AV1 };
	case LLFC_ULE:
	case LLFC_UGT:
		return vector<uint32_t>{ IL_FLAG_AC0, IL_FLAG_AC1, IL_FLAG_AZ };
	case LLFC_NEG:
	case LLFC_POS:
		return vector<uint32_t>{ IL_FLAG_AN };
	case LLFC_O:
	case LLFC_NO:
		return vector<uint32_t>{ IL_FLAG_V, IL_FLAG_AV0, IL_FLAG_AV1 };
	default:
		return vector<uint32_t>();
	}
}

size_t BlackfinArchitecture::GetFlagWriteLowLevelIL(BNLowLevelILOperation op, size_t size, uint32_t flagWriteType, uint32_t flag,
        BNRegisterOrConstant* operands, size_t operandCount, BinaryNinja::LowLevelILFunction& il) 
{
    // FIXME: To be implemented
    return 0;
}

std::string BlackfinArchitecture::GetRegisterName(uint32_t reg) {
    if (reg >= REG_RL0 && reg < REG_LASTREG) {
        return blackfin::get_register_name(static_cast<enum Register>(reg));
    } else {
        return "unknown";
    }
}

std::vector<uint32_t> BlackfinArchitecture::GetFullWidthRegisters() {
    return std::vector<uint32_t> {
        REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
        REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, REG_SP, REG_FP, 
        REG_A0, REG_A1, REG_I0, REG_I1, REG_I2, REG_I3, REG_M0, REG_M1,
        REG_M2, REG_M3, REG_B0, REG_B1, REG_B2, REG_B3, REG_L0, REG_L1, REG_L2, REG_L3,
        REG_LC0, REG_LC1, REG_ASTAT, REG_RETS, REG_LT0, REG_LB0, REG_LT1, REG_LB1,
        REG_CYCLES, REG_CYCLES2, REG_USP, REG_SEQSTAT, REG_SYSCFG, REG_RETI, REG_RETX, REG_RETN,
        REG_RETE, REG_EMUDAT, 
    };
}

std::vector<uint32_t> BlackfinArchitecture::GetAllRegisters() {
    return std::vector<uint32_t> {
        REG_RL0, REG_RL1, REG_RL2, REG_RL3, REG_RL4, REG_RL5, REG_RL6, REG_RL7,
        REG_RH0, REG_RH1, REG_RH2, REG_RH3, REG_RH4, REG_RH5, REG_RH6, REG_RH7,
        REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
        REG_R1_0, REG_R3_2, REG_R5_4, REG_R7_6, 
        REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, REG_SP, REG_FP, 
        REG_A0x, REG_A1x, REG_A0w, REG_A1w,
        REG_A0, REG_A1, REG_I0, REG_I1, REG_I2, REG_I3, REG_M0, REG_M1,
        REG_M2, REG_M3, REG_B0, REG_B1, REG_B2, REG_B3, REG_L0, REG_L1, REG_L2, REG_L3,
        REG_AZ, REG_AN, REG_AC0, REG_AC1, REG_AV0, REG_AV1, REG_AV0S, REG_AV1S,
        REG_AQ, REG_V, REG_VS,
        REG_sftreset, REG_omode, REG_excause, REG_emucause, REG_idle_req, REG_hwerrcause, REG_CC, 
        REG_LC0, REG_LC1, REG_ASTAT, REG_RETS, REG_LT0, REG_LB0, REG_LT1, REG_LB1,
        REG_CYCLES, REG_CYCLES2, REG_USP, REG_SEQSTAT, REG_SYSCFG, REG_RETI, REG_RETX, REG_RETN,
        REG_RETE, REG_EMUDAT, 
        REG_BR0, REG_BR1, REG_BR2, REG_BR3, REG_BR4, REG_BR5, REG_BR6,
        REG_BR7, REG_PL0, REG_PL1, REG_PL2, REG_PL3, REG_PL4, REG_PL5, REG_SLP, REG_FLP,
        REG_PH0, REG_PH1, REG_PH2, REG_PH3, REG_PH4, REG_PH5, REG_SHP, REG_FHP,
        REG_IL0, REG_IL1, REG_IL2, REG_IL3, REG_ML0, REG_ML1, REG_ML2, REG_ML3,
        REG_BL0, REG_BL1, REG_BL2, REG_BL3, REG_LL0, REG_LL1, REG_LL2, REG_LL3,
        REG_IH0, REG_IH1, REG_IH2, REG_IH3, REG_MH0, REG_MH1, REG_MH2, REG_MH3,
        REG_BH0, REG_BH1, REG_BH2, REG_BH3, REG_LH0, REG_LH1, REG_LH2, REG_LH3,
        REG_AC0_COPY, REG_V_COPY, REG_RND_MOD, REG_A0H, REG_A0L, REG_A1H, REG_A1L,
        REG_LASTREG,
    };
}

std::vector<uint32_t> BlackfinArchitecture::GetAllFlags() {
    return vector<uint32_t>{
        IL_FLAG_AC0, 
        IL_FLAG_AC1, 
        IL_FLAG_AN, 
        IL_FLAG_AQ, 
        IL_FLAG_AV0, 
        IL_FLAG_AVS0, 
        IL_FLAG_AV1, 
        IL_FLAG_AVS1,
        IL_FLAG_AZ,
        IL_FLAG_CC,
        IL_FLAG_V,
        IL_FLAG_VS
	};
}

std::vector<uint32_t> BlackfinArchitecture::GetAllFlagWriteTypes() {
    return vector<uint32_t>{
		IL_FLAGWRITE_ALL,
		IL_FLAGWRITE_A0,
        IL_FLAGWRITE_A1
	};
}

BNRegisterInfo BlackfinArchitecture::GetRegisterInfo(uint32_t reg) 
{
    switch(reg) {
    // 40 bit accumulator registers
    case REG_A0:
    case REG_A1:
        return RegisterInfo(reg, 0, 5);

    // top 8 bits of accumulator
    case REG_A0x:
    case REG_A1x:
        return RegisterInfo(blackfin::get_reg_for_reg_part(reg), 4, 1);

    // low 32 bits of accumulator
    case REG_A0w:
    case REG_A1w:
        return RegisterInfo(blackfin::get_reg_for_reg_part(reg), 0, 4);

    // Full-width 32 bit registers
    case REG_R0:
    case REG_R1:
    case REG_R2:
    case REG_R3:
    case REG_R4:
    case REG_R5:
    case REG_R6:
    case REG_R7:
    case REG_P0:
    case REG_P1:
    case REG_P2:
    case REG_P3:
    case REG_P4:
    case REG_P5:
    case REG_SP:
    case REG_FP:
    case REG_USP:
    case REG_I0:
    case REG_I1:
    case REG_I2:
    case REG_I3:
    case REG_M0:
    case REG_M1:
    case REG_M2:
    case REG_M3:
    case REG_B0:
    case REG_B1:
    case REG_B2:
    case REG_B3:
    case REG_L0:
    case REG_L1:
    case REG_L2:
    case REG_L3:
    case REG_LC0:
    case REG_LC1:
    case REG_LT0:
    case REG_LB0:
    case REG_LT1:
    case REG_LB1:
    case REG_CYCLES:
    case REG_CYCLES2:
    case REG_SEQSTAT:
    case REG_SYSCFG:
    case REG_RETI:
    case REG_RETX:
    case REG_RETN:
    case REG_RETE:
    case REG_RETS:
    case REG_ASTAT:
        return RegisterInfo(reg, 0, 4);

    // Low 16 bits of 32 bit register 
    case REG_RL0:
    case REG_RL1:
    case REG_RL2:
    case REG_RL3:
    case REG_RL4:
    case REG_RL5:
    case REG_RL6:
    case REG_RL7:
    case REG_PL0:
    case REG_PL1:
    case REG_PL2:
    case REG_PL3:
    case REG_PL4:
    case REG_PL5:
    case REG_SLP:
    case REG_FLP:
    case REG_IL0:
    case REG_IL1:
    case REG_IL2:
    case REG_IL3:
    case REG_ML0:
    case REG_ML1:
    case REG_ML2:
    case REG_ML3:
    case REG_BL0:
    case REG_BL1:
    case REG_BL2:
    case REG_BL3:
    case REG_LL0:
    case REG_LL1:
    case REG_LL2:
    case REG_LL3:
        return RegisterInfo(blackfin::get_reg_for_reg_part(reg), 0, 2);
    // High 16 bits of 32 bit register 
    case REG_RH0:
    case REG_RH1:
    case REG_RH2:
    case REG_RH3:
    case REG_RH4:
    case REG_RH5:
    case REG_RH6:
    case REG_RH7:
    case REG_PH0:
    case REG_PH1:
    case REG_PH2:
    case REG_PH3:
    case REG_PH4:
    case REG_PH5:
    case REG_SHP:
    case REG_FHP:
    case REG_IH0:
    case REG_IH1:
    case REG_IH2:
    case REG_IH3:
    case REG_MH0:
    case REG_MH1:
    case REG_MH2:
    case REG_MH3:
    case REG_BH0:
    case REG_BH1:
    case REG_BH2:
    case REG_BH3:
    case REG_LH0:
    case REG_LH1:
    case REG_LH2:
    case REG_LH3:
        return RegisterInfo(blackfin::get_reg_for_reg_part(reg), 2, 2);
    case REG_BR0:
    case REG_BR1:
    case REG_BR2:
    case REG_BR3:
    case REG_BR4:
    case REG_BR5:
    case REG_BR6:
    case REG_BR7:
        return RegisterInfo(blackfin::get_reg_for_reg_part(reg), 0, 1);
    case REG_CC:
        return RegisterInfo(reg, 0, 1);
    default:
        return RegisterInfo(REG_LASTREG, 0, 0); // FIXME: Better fail case?
    }
}

uint32_t BlackfinArchitecture::GetStackPointerRegister() {
    // FIXME: does this need to account for SP/USP split?
    return REG_SP; 
}

uint32_t BlackfinArchitecture::GetLinkRegister() {
    // FIXME: do we need to handle returns from interrupts etc. that use different return registers?
    return REG_RETS;
}

bool BlackfinArchitecture::CanAssemble()
{
	return false;
}


int BlackfinArchitecture::Disassemble(uint8_t *data, uint64_t addr, size_t maxLen, blackfin::Instruction &result)
{
    memset(&result, 0, sizeof(result));
    result.operation = blackfin::OP_ILLEGAL;
    return disasm_insn_bfin(data, addr, result);
}

bool BlackfinArchitecture::GetInstructionText(const uint8_t *data, uint64_t addr, size_t &len, vector<BinaryNinja::InstructionTextToken>& result) {
    blackfin::Instruction instr;
    memset(&instr, 0, sizeof(instr));
    int instr_size = Disassemble((uint8_t*)data, addr, 0, instr);

    len = instr.length;
    if (instr.operation == blackfin::OP_ILLEGAL) {
        result.emplace_back(TextToken, "ILLEGAL");
        return true;
    }

    if (instr.operation == blackfin::OP_UNSUPPORTED) {
        result.emplace_back(TextToken, "UNSUPPORTED");
        return true;
    }

    for (int i = 0; i < instr.operand_count; i++) {
        blackfin::InstructionOperand operand = instr.operands[i];
        char imm_string[60] = {0};
        if (operand.flags.cc_inverted) {
            result.emplace_back(TextToken, "!");
        }
        switch (operand.cls) {
        case blackfin::NONE:
            return true;

        case blackfin::MNEMOMIC:
            if (i > 0 && instr.operands[i - 1].cls != blackfin::OPERATOR) 
                result.emplace_back(TextToken, " ");
            result.emplace_back(InstructionToken, get_mnemonic_string(operand.mnemonic));
            result.emplace_back(TextToken, " ");
            break;

        case blackfin::REG:
        case blackfin::PREG:
        case blackfin::DREG:
        case blackfin::SREG:
        case blackfin::REGHI:
        case blackfin::REGLO:
            const char *inc_dec_symbol;
            result.emplace_back(RegisterToken, GetRegisterName(operand.reg));
            if (instr.operands[i + 1].cls == blackfin::REG) result.emplace_back(TextToken, " ");
            break; 

        case blackfin::OPERATOR:
            result.emplace_back(TextToken, blackfin::get_operator_string(operand.operat));
            break;

        case blackfin::IMM:
            if (operand.flags.pcrelative) {
                sprintf(imm_string, "%#lx", operand.imm + addr);
                result.emplace_back(PossibleAddressToken, imm_string, operand.imm + addr);
            } else {
                sprintf(imm_string, "%#x", operand.imm);
                result.emplace_back(IntegerToken, imm_string, operand.imm);
            }
            break;

        case blackfin::UIMM:
            sprintf(imm_string, "%#x", operand.uimm);
            result.emplace_back(IntegerToken, imm_string, operand.uimm);
            break;

        case blackfin::REG_RANGE:
            result.emplace_back(TextToken, "(");
            result.emplace_back(RegisterToken, GetRegisterName(operand.r_r.top));
            result.emplace_back(OperandSeparatorToken, ":");
            result.emplace_back(RegisterToken, GetRegisterName(operand.r_r.bottom));
            result.emplace_back(TextToken, ")");
            break;
        
        case blackfin::REG_RANGE_PAIR:
            result.emplace_back(TextToken, "(");
            result.emplace_back(RegisterToken, GetRegisterName(operand.r_r.top));
            result.emplace_back(OperandSeparatorToken, ":");
            result.emplace_back(RegisterToken, GetRegisterName(operand.r_r.bottom));
            result.emplace_back(OperandSeparatorToken, ", ");
            result.emplace_back(RegisterToken, GetRegisterName(operand.r_p.top));
            result.emplace_back(OperandSeparatorToken, ":");
            result.emplace_back(RegisterToken, GetRegisterName(operand.r_p.bottom));
            result.emplace_back(TextToken, ")");
            break;

        case blackfin::MEM_ACCESS:
            if (operand.mem_access.width == 1) result.emplace_back(TextToken, "b");
            if (operand.mem_access.width == 2) result.emplace_back(TextToken, "w");
            result.emplace_back(BeginMemoryOperandToken, "[");
            switch (operand.mem_access.mode) {
            case blackfin::MEM_REGMOD:
                if (operand.mem_access.pre_dec) result.emplace_back(TextToken, "--");
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.ptr_reg));
                if (operand.mem_access.post_inc) result.emplace_back(TextToken, "++");
                break;
            case blackfin::MEM_REG:
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.ptr_reg));
                break;
            case blackfin::MEM_IMM:
                sprintf(imm_string, "%#x", operand.mem_access.ptr_imm);
                result.emplace_back(IntegerToken, imm_string, operand.mem_access.ptr_imm);
                break;
            case blackfin::MEM_REGIMM:
                sprintf(imm_string, "%#x", operand.mem_access.idx_imm);
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.ptr_reg));
                result.emplace_back(TextToken, get_operator_string(operand.mem_access.oper));
                if (operand.mem_access.idx_imm != 1) result.emplace_back(IntegerToken, imm_string, operand.mem_access.idx_imm);
                break;
            case blackfin::MEM_REGREG:
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.ptr_reg));
                result.emplace_back(TextToken, get_operator_string(operand.mem_access.oper));
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.idx_reg));
                break;
            case blackfin::MEM_REGOFF:
                sprintf(imm_string, "%#x", operand.mem_access.offset);
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.ptr_reg));
                result.emplace_back(TextToken, get_operator_string(operand.mem_access.oper));
                result.emplace_back(IntegerToken, imm_string, operand.mem_access.offset);
                break;
            default:
                return false;
            }
            result.emplace_back(EndMemoryOperandToken, "]");
            break;

        default:
            return false;
        }
        if (operand.flags.sign_extended) result.emplace_back(TextToken, " (x)");
        if (operand.flags.zero_extended) result.emplace_back(TextToken, " (z)");
    }
    if ((instr.operation == blackfin::OP_DSPALU || 
         instr.operation == blackfin::OP_DSPMAC || 
         instr.operation == blackfin::OP_DSPMUL ||
         instr.operation == blackfin::OP_DSPSHIFT ||
         instr.operation == blackfin::OP_DSPSHIFTIMM)) {
            result.emplace_back(TextToken, " {DSP OP}");
    }
    return true;
}

bool BlackfinArchitecture::GetInstructionInfo(const uint8_t *data, uint64_t addr, size_t maxLen, BinaryNinja::InstructionInfo &result) {

    blackfin::Instruction instr;
    int instr_size = Disassemble((uint8_t*)data, addr, 0, instr);
    if (instr.operation == blackfin::OP_ILLEGAL) return true;
    result.length = instr_size;

    if (instr.operation == blackfin::OP_LSETUP) {
        int loopStart = instr.operands[2].imm + addr;
        int loopEnd = instr.operands[4].imm + addr;
        enum Register loop_count_reg = instr.operands[6].reg;
        this->has_loop = true;
        this->next_loopend = loopEnd;
        this->next_loopstart = loopStart;
        this->lc = loop_count_reg;
    }

    if (this->has_loop) {
        if (addr == this->next_loopend) {
            result.AddBranch(FalseBranch, addr + instr_size);
            result.AddBranch(TrueBranch, this->next_loopstart);
            this->has_loop = false;
            this->next_loopend = 0;
            this->next_loopstart = 0;
        }
    }

    switch (instr.operation) {
    case blackfin::OP_BRCC:
       result.AddBranch(FalseBranch, addr + 2);
       result.AddBranch(TrueBranch, instr.operands[3].imm + addr);
       break;
    case blackfin::OP_CALLABS:
       result.AddBranch(CallDestination, instr.operands[1].imm);
       break;
    case blackfin::OP_CALLREL:
       result.AddBranch(CallDestination, addr + instr.operands[1].imm);
       break;
    case blackfin::OP_JMPABS:
       result.AddBranch(UnconditionalBranch, instr.operands[1].imm);
       break;
    case blackfin::OP_JMPREL:
       result.AddBranch(UnconditionalBranch, addr + instr.operands[1].imm);
       break;
    case blackfin::OP_CALLREGABS:
    case blackfin::OP_CALLREGREL:
       result.AddBranch(CallDestination);
       break;
    case blackfin::OP_JMPREGABS:
    case blackfin::OP_JMPREGREL:
       result.AddBranch(UnresolvedBranch);
       break;
    }
    if (instr.operation == blackfin::OP_RET) {
        switch (instr.operands[0].mnemonic) {
        case blackfin::OL_RTS:
            result.AddBranch(FunctionReturn);
            break;
        case blackfin::OL_RTE:
        case blackfin::OL_RTI:
        case blackfin::OL_RTN:
        case blackfin::OL_RTX:
            result.AddBranch(UnresolvedBranch);
            break;
        default:
            break;
        }
    }
    return true;
}

static void ConditionalJump(Architecture* arch, LowLevelILFunction& il, 
        size_t addrSize, uint64_t t, uint64_t f, bool cc_inverted, ExprId cond = 0)
{
	BNLowLevelILLabel* trueLabel = il.GetLabelForAddress(arch, t);
	BNLowLevelILLabel* falseLabel = il.GetLabelForAddress(arch, f);

    ExprId flag = cc_inverted ? il.Not(4, il.Flag(IL_FLAG_CC)) : il.Flag(IL_FLAG_CC);

    ExprId c = cond == 0 ? flag : cond;

	if (trueLabel && falseLabel)
	{
		il.AddInstruction(il.If(c, *trueLabel, *falseLabel));
		return;
	}

	LowLevelILLabel trueCode, falseCode;

	if (trueLabel)
	{
		il.AddInstruction(il.If(c, *trueLabel, falseCode));
		il.MarkLabel(falseCode);
		il.AddInstruction(il.Jump(il.ConstPointer(addrSize, f)));
		return;
	}

	if (falseLabel)
	{
		il.AddInstruction(il.If(c, trueCode, *falseLabel));
		il.MarkLabel(trueCode);
		il.AddInstruction(il.Jump(il.ConstPointer(addrSize, t)));
		return;
	}

	il.AddInstruction(il.If(c, trueCode, falseCode));
	il.MarkLabel(trueCode);
	il.AddInstruction(il.Jump(il.ConstPointer(addrSize, t)));
	il.MarkLabel(falseCode);
	il.AddInstruction(il.Jump(il.ConstPointer(addrSize, f)));
}

ExprId
ReadILOperand(blackfin::InstructionOperand &operand, LowLevelILFunction &il, size_t addr, bool isPointer = false)
{
    switch (operand.cls) {
    case blackfin::IMM:
        if (isPointer) return il.ConstPointer(4, operand.imm);
        else return il.Const(4, operand.imm);
    case blackfin::REG:
        return il.Register(blackfin::get_register_size(operand.reg), operand.reg);
    default:
        return il.Undefined();
    }
}

using namespace blackfin;

bool 
BlackfinArchitecture::GetInstructionLowLevelIL(const uint8_t *data, uint64_t addr, size_t &len, LowLevelILFunction &il) 
{
    blackfin::Instruction instr;
    int instr_size = Disassemble((uint8_t *)data, addr, 0, instr);

    if (instr.operation == blackfin::OP_ILLEGAL) {
        il.AddInstruction(il.Undefined());
        return false;
    }

    
    len = instr_size;
    bool status = true;
    switch (instr.operation) {
    case OP_NOP:
        il.AddInstruction(il.Nop());
        break;
    case OP_RET:
        il.AddInstruction(il.Return(il.Register(4, instr.operands[1].reg)));
        break;
    case OP_JMPABS:
        il.AddInstruction(il.Jump(il.ConstPointer(4, instr.operands[1].imm)));
        break;
    case OP_JMPREL:
        il.AddInstruction(il.Jump(il.ConstPointer(4, addr + instr.operands[1].imm)));
        break;
    case OP_CALLABS:
        il.AddInstruction(il.Call(il.ConstPointer(4, instr.operands[1].imm)));
        break;
    case OP_CALLREL:
        il.AddInstruction(il.Call(il.ConstPointer(4, addr + instr.operands[1].imm)));
        break;
    case OP_CALLREGABS:
    case OP_CALLREGREL:
        il.AddInstruction(il.Call(il.Register(4, instr.operands[1].reg)));
        break;
    case OP_JMPREGABS:
    case OP_JMPREGREL:
        il.AddInstruction(il.Jump(il.Register(4, instr.operands[1].reg)));
        break;
    case OP_POP: {
        enum Register reg = instr.operands[0].reg;
        il.AddInstruction(il.SetRegister(get_register_size(reg), reg, il.Pop(get_register_size(reg))));
        break;
    }
    case OP_PUSH: {
        enum Register reg = instr.operands[2].reg;
        il.AddInstruction(il.Push(4, il.Register(get_register_size(reg), reg)));
        break;
    }
    case OP_POPMULT:
        if (instr.operands[0].cls == REG_RANGE) {
            enum Register reg_hi = instr.operands[0].r_r.top;
            enum Register reg_lo = instr.operands[0].r_r.bottom;
            for (int reg = reg_hi; reg >= reg_lo; reg--) {
                il.AddInstruction(il.SetRegister(4, reg, il.Pop(4)));
            }
        } else if (instr.operands[0].cls == REG_RANGE_PAIR) {
            enum Register dreg_hi = instr.operands[0].r_r.top;
            enum Register dreg_lo = instr.operands[0].r_r.bottom;
            enum Register preg_hi = instr.operands[0].r_p.top;
            enum Register preg_lo = instr.operands[0].r_p.bottom;
            for (int reg = preg_hi; reg >= preg_lo; reg--) {
                il.AddInstruction(il.SetRegister(4, reg, il.Pop(4)));
            }
            for (int reg = dreg_hi; reg >= dreg_lo; reg--) {
                il.AddInstruction(il.SetRegister(4, reg, il.Pop(4)));
            }
        }
        break;
    case OP_PUSHMULT:
        if (instr.operands[2].cls == REG_RANGE) {
            enum Register reg_hi = instr.operands[2].r_r.top;
            enum Register reg_lo = instr.operands[2].r_r.bottom;
            for (int reg = reg_lo; reg <= reg_hi; reg++) {
                il.AddInstruction(il.Push(4, il.Register(4, reg)));
            }
        } else if (instr.operands[2].cls == REG_RANGE_PAIR) {
            enum Register dreg_hi = instr.operands[2].r_r.top;
            enum Register dreg_lo = instr.operands[2].r_r.bottom;
            enum Register preg_hi = instr.operands[2].r_p.top;
            enum Register preg_lo = instr.operands[2].r_p.bottom;
            for (int reg = dreg_lo; reg <= dreg_hi; reg++) {
                il.AddInstruction(il.Push(4, il.Register(4, reg)));
            }
            for (int reg = preg_lo; reg <= preg_hi; reg++) {
                il.AddInstruction(il.Push(4, il.Register(4, reg)));
            }
        }
        break;
    case OP_ALU2OP: {
        struct InstructionOperand dst = instr.operands[0];
        enum Operator op = instr.operands[1].operat;
        struct InstructionOperand src = instr.operands[2];
        int src_width = GetRegisterInfo(src.reg).size;

        ExprId src_il;
        if (src.flags.sign_extended) {
            src_il = il.SignExtend(4, il.Register(src_width, src.reg));
        } else if (src.flags.zero_extended) {
            src_il = il.ZeroExtend(4, il.Register(src_width, src.reg));
        } else {
            src_il = il.Register(src_width, src.reg);
        }

        switch (instr.operands[1].operat) {
        case OPER_ASHIFTR:
            il.AddInstruction(il.SetRegister(4, 
                        dst.reg, 
                        il.ArithShiftRight(4, 
                            il.Register(4, dst.reg), 
                            src_il)));
            break;
        case OPER_LSHIFTREQ:
            il.AddInstruction(il.SetRegister(4, 
                        dst.reg, 
                        il.LogicalShiftRight(4, 
                            il.Register(4, dst.reg), 
                            src_il)));
            break;
        case OPER_LSHIFTLEQ:
            il.AddInstruction(il.SetRegister(4, 
                        dst.reg, 
                        il.ShiftLeft(4, 
                            il.Register(4, dst.reg), 
                            src_il)));
            break;
        case OPER_MULEQ:
            il.AddInstruction(il.SetRegister(4, 
                        dst.reg, 
                        il.Mult(4, 
                            il.Register(4, dst.reg), 
                            src_il)));
            break;
        case OPER_EQ:
            il.AddInstruction(il.SetRegister(4, dst.reg, src_il));
            break;
        case OPER_EQNEG:
            il.AddInstruction(il.SetRegister(4, dst.reg, il.Neg(src_width, src_il)));
            break;
        case OPER_EQNOT:
            il.AddInstruction(il.SetRegister(4, dst.reg, il.Not(src_width, src_il)));
            break;
        }
        break;
    }
    case OP_MVADDTHENSHIFT: {
        struct InstructionOperand dst = instr.operands[0];
        struct InstructionOperand src = instr.operands[4];
        int imm = instr.operands[6].imm;
        il.AddInstruction(il.SetRegister(4, 
                    dst.reg, 
                    il.ShiftLeft(4, 
                        il.Add(4, il.Register(4, dst.reg), il.Register(get_register_size(src.reg), src.reg)),
                        il.Const(1, imm))));
        break;
    }
    case OP_CCFLAG:
        switch (instr.operands[3].operat) {
        case OPER_EQEQ:
            il.AddInstruction(
                    il.SetFlag(
                        IL_FLAG_CC, 
                        il.CompareEqual(4, 
                            ReadILOperand(instr.operands[2], il, addr, false), 
                            ReadILOperand(instr.operands[4], il, addr, false))));
            break;
        case OPER_LT:
            il.AddInstruction(
                    il.SetFlag(
                        IL_FLAG_CC, 
                        il.CompareSignedLessThan(4, 
                            ReadILOperand(instr.operands[2], il, addr, false), 
                            ReadILOperand(instr.operands[4], il, addr, false))));
            break;
        case OPER_LTE:
            il.AddInstruction(
                    il.SetFlag(
                        IL_FLAG_CC, 
                        il.CompareSignedLessEqual(4, 
                            ReadILOperand(instr.operands[2], il, addr, false), 
                            ReadILOperand(instr.operands[4], il, addr, false))));
            break;
        case OPER_LTU:
            il.AddInstruction(
                    il.SetFlag(
                        IL_FLAG_CC, 
                        il.CompareUnsignedLessThan(4, 
                            ReadILOperand(instr.operands[2], il, addr, false), 
                            ReadILOperand(instr.operands[4], il, addr, false))));
            break;
        case OPER_LTEU:
            il.AddInstruction(
                    il.SetFlag(
                        IL_FLAG_CC, 
                        il.CompareUnsignedLessEqual(4, 
                            ReadILOperand(instr.operands[2], il, addr, false), 
                            ReadILOperand(instr.operands[4], il, addr, false))));
            break;
        }
        break;
    default:
        il.AddInstruction(il.Unimplemented());
        break;
    case OP_BRCC:
        ConditionalJump(this, il, 4, addr + instr.operands[3].imm, addr + 2, instr.operands[1].flags.cc_inverted);
        break;
    case OP_LDIMMHW: {
        blackfin::Instruction next_instr;
        int next_instrsz = Disassemble((uint8_t*)(data + instr.length), addr, 4, next_instr);

        enum Register reg = instr.operands[0].reg;
        int imm = instr.operands[2].imm;

        BNRegisterInfo reginfo = GetRegisterInfo(reg);
        BNRegisterInfo nextreginfo = GetRegisterInfo(next_instr.operands[0].reg);

        bool is_ldhi, is_ldlo, next_is_ldhi, next_is_ldlo;

        is_ldlo = reginfo.offset == 0;
        is_ldhi = reginfo.offset == 2;

        next_is_ldlo = nextreginfo.offset == 0 && next_instr.operation == OP_LDIMMHW;
        next_is_ldhi = nextreginfo.offset == 2 && next_instr.operation == OP_LDIMMHW;

        if (is_ldhi && next_is_ldlo && reginfo.fullWidthRegister == nextreginfo.fullWidthRegister) { // load 32 bit immediate idiom
            il.AddInstruction(
                il.SetRegister(4, 
                    reginfo.fullWidthRegister, 
                    il.ConstPointer(4, (instr.operands[2].imm << 16) | (next_instr.operands[2].imm))));
            len += next_instrsz;
            return status;
        } else if (is_ldlo && next_is_ldhi && reginfo.fullWidthRegister == nextreginfo.fullWidthRegister) { // load 32 bit immediate
            il.AddInstruction(
            il.SetRegister(4, 
                reginfo.fullWidthRegister, 
                il.ConstPointer(4, (next_instr.operands[2].imm << 16) | (instr.operands[2].imm))));
            len += next_instrsz;
            return status;
        } else {
            if (reginfo.fullWidthRegister == reg) { // Full width reg load
                il.AddInstruction(il.SetRegister(get_register_size(reg), reginfo.fullWidthRegister, il.ConstPointer(4, imm)));
            }
            else if (reginfo.offset == 0) { // low
                
                il.AddInstruction(
                    il.SetRegister(4, 
                        reginfo.fullWidthRegister, 
                        il.Add(4, 
                            il.And(4, 
                                il.Register(4, reginfo.fullWidthRegister), 
                                il.ConstPointer(4, 0xffff0000)), 
                            il.ConstPointer(2, imm))));
            }
            else if (reginfo.offset == 2) { // high
                // FIXME: This does NOT follow spec, but is here to help the VSA understand
                // when registers are being set in two .H loads. I have not seen an example of a .H load
                // happening by iself/where the low bits weren't overwritten before the next read, but if that
                // does happen, this will be !INCORRECT! il. The commented code below follows the spec.
                // but outputs ugly il, and BN can't track the final value of the register.
                il.AddInstruction(il.SetRegister(4, reginfo.fullWidthRegister, il.ConstPointer(4, imm << 16)));
                /*
                il.AddInstruction(
                    il.SetRegister(
                        4, reginfo.fullWidthRegister, 
                        il.Or(4,
                            il.And(4, il.Register(4, reginfo.fullWidthRegister), il.Const(4, 0x0000ffff)),
                            il.Const(4, imm << 16))));
                */
            }
        }
        break;
    }
    case OP_LDIMM: {
        enum Register reg = instr.operands[0].reg;
        int32_t imm = instr.operands[2].imm;
        enum Operator op = instr.operands[1].operat;
        switch (op) {
        case OPER_EQ:
            il.AddInstruction(il.SetRegister(4, reg, il.Const(4, imm)));
            break;
        case OPER_PLUSEQ:
            il.AddInstruction(il.SetRegister(4, reg, il.Add(4, il.Register(4, reg), il.Const(4, imm))));
            break;
        }
        break;
    }
    case OP_MV: {
        enum Register dst = instr.operands[0].reg;
        enum Operator op  = instr.operands[1].operat;
        enum Register src = instr.operands[2].reg;
        int dst_size = get_register_size(dst);
        int src_size = get_register_size(src);

        switch (op) {
        case OPER_EQ:
            il.AddInstruction(
                    il.SetRegister(
                        get_register_size(dst), 
                        dst, 
                        il.Register(
                            get_register_size(src), src)));
            break;
        case OPER_MINUSEQ:
            il.AddInstruction(
                    il.SetRegister(dst_size, 
                        dst,
                        il.Sub(dst_size, 
                            il.Register(dst_size, dst), 
                            il.Register(src_size, src))));
            break;
        case OPER_PLUSEQ:
            il.AddInstruction(
                    il.SetRegister(dst_size, 
                        dst,
                        il.Add(dst_size, 
                            il.Register(dst_size, dst), 
                            il.Register(src_size, src))));
            break;
        default:
            il.AddInstruction(il.Unimplemented());
            break;
        }
        break;
    }
    case OP_LINK: {
        il.AddInstruction(il.Push(4, il.Register(4, REG_RETS)));
        il.AddInstruction(il.Push(4, il.Register(4, REG_FP)));
        il.AddInstruction(il.SetRegister(4, 
                    REG_FP, 
                    il.Register(4, REG_SP)));
        il.AddInstruction(il.SetRegister(4, 
                    REG_SP, 
                    il.Sub(4, 
                        il.Register(4, REG_SP), 
                        il.Const(4, instr.operands[1].imm))));
        break;
    }
    case OP_UNLINK: {
        il.AddInstruction(il.SetRegister(4, 
                    REG_SP, 
                    il.Register(4, REG_FP)));
        il.AddInstruction(il.SetRegister(4, REG_FP, il.Pop(4)));
        il.AddInstruction(il.SetRegister(4, REG_RETS, il.Pop(4)));
        break;
    }
    case OP_LD: {
        enum Register dst = instr.operands[0].reg;
        struct MemAccess mem_src = instr.operands[2].mem_access;
        enum Register src = mem_src.ptr_reg;

        ExprId load_il;
        ExprId target_il;


        if (mem_src.mode == MEM_REGOFF) {
            target_il = il.Add(4, il.Register(get_register_size(src), src), il.Const(4, mem_src.offset));
        } else {
            target_il = il.Register(get_register_size(src), src);
        }

        if (instr.operands[2].flags.sign_extended) {
            load_il = il.SignExtend(
                get_register_size(dst),
                il.Load(
                    mem_src.width, 
                    target_il));

        } else if (instr.operands[2].flags.zero_extended) {
            load_il = il.ZeroExtend(
                get_register_size(dst),
                il.Load(
                    mem_src.width, 
                    target_il));
        } else {
            load_il = il.Load(mem_src.width, target_il);
        }
        
        switch (mem_src.mode) {
        case MEM_REGOFF:
        case MEM_REG:
            il.AddInstruction(
                il.SetRegister(
                    get_register_size(dst), 
                    dst, 
                    load_il));
            break;
        case MEM_REGIMM:
            il.AddInstruction(
                il.SetRegister(
                    get_register_size(dst), 
                    dst, 
                    load_il));

            il.AddInstruction(il.SetRegister(
                get_register_size(src), 
                src, 
                il.Add(
                    get_register_size(src), 
                    il.Register(get_register_size(src), src), 
                    il.Const(1, mem_src.idx_imm))));
            break;
        }
        break;
    }
    case OP_ST: {
        enum Register src = instr.operands[2].reg;
        struct MemAccess mem_dst = instr.operands[0].mem_access;
        enum Register dst = mem_dst.ptr_reg;

        ExprId target_il;

        if (mem_dst.mode == MEM_REGOFF) {
            target_il = il.Add(4, il.Register(get_register_size(dst), dst), il.Const(4, mem_dst.offset));
        } else {
            target_il = il.Register(get_register_size(dst), dst);
        }
        switch (mem_dst.mode) {
        case MEM_REG:
        case MEM_REGOFF:
            il.AddInstruction(
                il.Store(
                    mem_dst.width,
                    target_il, 
                    il.Register(get_register_size(src), src)));
            break;
        case MEM_REGIMM:
            il.AddInstruction(
                il.Store(
                    mem_dst.width,
                    target_il, 
                    il.Register(get_register_size(src), src)));

            il.AddInstruction(il.SetRegister(
                get_register_size(dst), 
                dst, 
                il.Add(
                    get_register_size(dst), 
                    il.Register(get_register_size(dst), dst), 
                    il.Const(1, mem_dst.idx_imm))));
            break;
        }
        break;
    }
    case OP_LDREG3: {
        enum Register dst = instr.operands[0].reg;
        enum Register src0 = instr.operands[2].reg;
        enum Register src1 = instr.operands[4].reg;
        enum Operator op = instr.operands[3].operat;

        switch (op) {
        case OPER_MINUS:
            il.AddInstruction(il.SetRegister(4,
                        dst,
                        il.Sub(4, 
                            il.Register(4, src0),
                            il.Register(4, src1))));
            break;
        case OPER_OR:
            il.AddInstruction(il.SetRegister(4,
                        dst,
                        il.Or(4, 
                            il.Register(4, src0),
                            il.Register(4, src1))));
            break;
        case OPER_AND:
            il.AddInstruction(il.SetRegister(4,
                        dst,
                        il.And(4, 
                            il.Register(4, src0),
                            il.Register(4, src1))));
            break;
        case OPER_XOR:
            il.AddInstruction(il.SetRegister(4,
                        dst,
                        il.Xor(4, 
                            il.Register(4, src0),
                            il.Register(4, src1))));
            break;
        case OPER_PLUS:
            il.AddInstruction(il.SetRegister(4,
                        dst,
                        il.Add(4, 
                            il.Register(4, src0),
                            il.Register(4, src1))));
            break;
        }
        break;
    }
    case OP_ALUADDSHIFTED: {
        enum Register dst = instr.operands[0].reg;
        enum Register src0 = instr.operands[2].reg;
        enum Register src1 = instr.operands[4].reg;
        enum Operator op = instr.operands[3].operat;
        il.AddInstruction(il.SetRegister(4,
                    dst,
                    il.Add(4, 
                        il.Register(4, src0),
                        il.ShiftLeft(4, 
                            il.Register(4, src1), 
                            il.Const(1, instr.operands[6].imm)))));
        break;
    }
    case OP_CCMOVE: {
        enum Register dst = instr.operands[2].reg;
        enum Register src = instr.operands[4].reg;
        ExprId condition = instr.operands[1].flags.cc_inverted ?  il.Not(1, il.Flag(IL_FLAG_CC)) : il.Flag(IL_FLAG_CC);

	    BNLowLevelILLabel trueLabel = LowLevelILLabel();
	    BNLowLevelILLabel falseLabel = LowLevelILLabel();
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));
        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(
                    get_register_size(dst), 
                    dst, 
                    il.Register(get_register_size(src), src)));
        il.MarkLabel(falseLabel);

        break;
    }
    case OP_LSETUP: {
        int loopStart = instr.operands[2].imm + addr;
        int loopEnd = instr.operands[4].imm + addr;
        enum Register loop_count_reg = instr.operands[6].reg;

        if (instr.operand_count == 9) {
            il.AddInstruction(il.SetRegister(4, loop_count_reg, il.Register(4, instr.operands[8].reg)));
        } else if (instr.operand_count == 11) {
            il.AddInstruction(il.SetRegister(4, 
                        loop_count_reg, 
                        il.LogicalShiftRight(4,
                            il.Register(4, instr.operands[8].reg), 
                            il.Const(1, 1))));
        }

        this->il_has_loop = true;
        this->il_next_loopend = loopEnd;
        this->il_next_loopstart = loopStart;
        this->il_lc = loop_count_reg;
        break;
    }
    case OP_RAISE: {
        int raise_no = instr.operands[1].imm;
        if (raise_no == 0) { // Syscall
            il.AddInstruction(il.SystemCall());
        } else {
            il.AddInstruction(il.Intrinsic({}, BFIN_INTRINSIC_RAISE, { il.Const(4, raise_no) }));
        }
        break;
    }
    case OP_MVSHIFTED: {
        // FIXME: this implementation is a little hard to follow, consider separating based on number of operands or something
        
        // TODO when the S flag is set, shifts which would overflow saturate instead.
        // this needs to be implemented separately. Same for the V flag, which signals
        // a vector operation.
        if (instr.flags.S || instr.flags.V) {
            il.AddInstruction(il.Unimplemented());
            break;
        }

        int shift_amt;
        enum Register dst, src;
        Operator op;

        if (instr.operands[1].operat != OPER_EQ) {
            shift_amt = instr.operands[2].imm;
            dst = instr.operands[0].reg;
            src = dst;
            op = instr.operands[1].operat;
        } else {
            shift_amt = instr.operands[4].imm;
            dst = instr.operands[0].reg;
            src = instr.operands[2].reg;
            op = instr.operands[3].operat;
        }
        int dst_size = get_register_size(dst);
        int src_size = get_register_size(src);

        switch (op) {
        case OPER_ASHIFTR:
        case OPER_ASHIFTREQ:
            il.AddInstruction(il.SetRegister(dst_size, dst, il.ArithShiftRight(src_size, il.Register(4, src), il.Const(4, shift_amt))));
            break;
        case OPER_LSHIFTR:
        case OPER_LSHIFTREQ:
            il.AddInstruction(il.SetRegister(dst_size, dst, il.LogicalShiftRight(src_size, il.Register(src_size, src), il.Const(4, shift_amt))));
            break;
        case OPER_LSHIFTL:
        case OPER_LSHIFTLEQ:
            il.AddInstruction(il.SetRegister(dst_size, dst, il.ShiftLeft(src_size, il.Register(src_size, src), il.Const(4, shift_amt))));
            break;
        } // end local switch (op)
        break;
    }
    case OP_CCBITTST: {
        ExprId bittst;
        enum Register reg = instr.operands[3].reg;
        int imm = instr.operands[5].imm;
        if (instr.operands[2].flags.cc_inverted) {
            bittst = il.CompareEqual(4,
                    il.And(4, 
                        il.Register(4, reg), 
                        il.Const(4, 1 << imm)),
                    il.Const(4, 0));
        } else {
            bittst = il.CompareNotEqual(4,
                    il.And(4, 
                        il.Register(4, reg), 
                        il.Const(4, 1 << imm)),
                    il.Const(4, 0));
        }
        il.AddInstruction(il.SetFlag(
            IL_FLAG_CC, 
            bittst));
        break;
    }
    case OP_MAX: 
    case OP_MIN:
    {
        enum Register dst = instr.operands[0].reg;
        enum Register src0 = instr.operands[3].reg;
        enum Register src1 = instr.operands[5].reg;
        LowLevelILLabel trueLabel, falseLabel, endLabel;
        ExprId cmp_il;

        if (instr.operation == OP_MAX) cmp_il = il.CompareSignedGreaterEqual(4, il.Register(4, src0), il.Register(4, src1));
        else if (instr.operation == OP_MIN) cmp_il = il.CompareSignedLessEqual(4, il.Register(4, src0), il.Register(4, src1));

        il.AddInstruction(il.If(
                    cmp_il,
                    trueLabel,
                    falseLabel));

        if (instr.flags.V) il.AddInstruction(il.SetFlag(il.Flag(IL_FLAG_V), il.Const(1, 0)));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(4, dst, il.Register(4, src0)));
        il.AddInstruction(il.Goto(endLabel));
        il.MarkLabel(falseLabel);
        il.AddInstruction(il.SetRegister(4, dst, il.Register(4, src1)));
        il.MarkLabel(endLabel);

        break;
    }
    case OP_ROT: {
        enum Register dst = instr.operands[0].reg;
        enum Register src = instr.operands[3].reg;
        int dst_size = get_register_size(dst);
        int src_size = get_register_size(src);
        InstructionOperand rot_amt = instr.operands[5];

        // lift most common cases in a more direct way
        if (rot_amt.cls == IMM && rot_amt.imm == 0) {
            il.AddInstruction(il.SetRegister(dst_size, dst, il.Register(src_size, src)));
            break;
        }
        if (rot_amt.cls == IMM && rot_amt.imm == -1) {
            il.AddInstruction(
                    il.SetRegister(dst_size,
                        dst,
                        il.Or(dst_size, // OR makes more sense but removes "pointer" type from VSA, this is equivalent
                            il.ShiftLeft(dst_size, 
                                il.Flag(IL_FLAG_CC), 
                                il.Const(dst_size, (dst_size * 8) - 1)),
                            il.LogicalShiftRight(src_size, il.Register(src_size, src), il.Const(1, 1)))));
            break;
        }
        if (rot_amt.cls == IMM && rot_amt.imm == 1) {
            il.AddInstruction(
                    il.SetRegister(dst_size,
                        dst,
                        il.Or(dst_size, // OR makes more sense but removes "pointer" type from VSA, this is equivalent
                            il.ShiftLeft(src_size, il.Register(src_size, src), il.Const(1, 1)),
                            il.Flag(IL_FLAG_CC))));
            break;
        }
        // TODO: lift other uses of ROT
        il.AddInstruction(il.Unimplemented());
        break;
        /*
        ExprId rot_amt_il;
        if (rot_amt.cls == REG) {
            rot_amt_il = il.Register(get_register_size(rot_amt.reg), rot_amt.reg);
        } else if (rot_amt.cls == IMM) {
            rot_amt_il = il.Const(4, rot_amt.imm);
        }
        */
    }
    case OP_CCDREG: {
        enum Register reg;
        int reg_size;
        if (instr.operands[0].reg == REG_CC) {
            if (instr.operands[2].reg == REG_CC) {
                il.AddInstruction(il.SetFlag(IL_FLAG_CC, il.Not(1, il.Flag(IL_FLAG_CC))));
                break;
            }
            reg = instr.operands[2].reg;
            reg_size = get_register_size(reg);
            il.AddInstruction(il.SetFlag(IL_FLAG_CC, il.CompareNotEqual(reg_size, il.Register(reg_size, reg), il.Const(reg_size, 0))));
        } else {
            reg = instr.operands[0].reg;
            reg_size = get_register_size(reg);
            il.AddInstruction(il.SetRegister(reg_size, reg, il.ZeroExtend(reg_size, il.Flag(IL_FLAG_CC))));
        }
        break;
    }
    case OP_VECTOR16: {
        enum Register dst = instr.operands[0].reg;
        enum Register src0 = instr.operands[2].reg;
        enum Register src1 = instr.operands[4].reg;
        enum Operator op = instr.operands[3].operat;
        
        switch (op) {
        case OPER_MINUSORMINUS:
            if (dst == src0 && src0 == src1) { // register clearing idiom
                il.AddInstruction(il.SetRegister(get_register_size(dst), dst, il.Const(get_register_size(dst), 0)));
                break;
            }
            // FALLTHROUGH
        default:
            il.AddInstruction(il.Unimplemented());
            break;
        }
        break;
    }
    case OP_ADD: {
        enum Register dst = instr.operands[0].reg;
        enum Register src0 = instr.operands[2].reg;
        enum Register src1 = instr.operands[4].reg;
        int dst_size = get_register_size(dst);
        int src0_size = get_register_size(src0);
        int src1_size = get_register_size(src1);

        // TODO: saturation (S flag) needs to be handled separately.
        // unimplemented for now.
        if (instr.flags.S) {
            il.AddInstruction(il.Unimplemented());
            break;
        } else if (instr.flags.NS) {
            il.AddInstruction(il.SetRegister(dst_size, 
                        dst,
                        il.Add(dst_size,
                            il.Register(src0_size, src0),
                            il.Register(src1_size, src1))));
            break;
        } else {
            il.AddInstruction(il.Unimplemented());
            break;
        }
        break;
    }
    case OP_SUB: {
        enum Register dst = instr.operands[0].reg;
        enum Register src0 = instr.operands[2].reg;
        enum Register src1 = instr.operands[4].reg;
        int dst_size = get_register_size(dst);
        int src0_size = get_register_size(src0);
        int src1_size = get_register_size(src1);

        // TODO: saturation (S flag) needs to be handled separately.
        // unimplemented for now.
        if (instr.flags.S) {
            il.AddInstruction(il.Unimplemented());
            break;
        } else if (instr.flags.NS) {
            il.AddInstruction(il.SetRegister(dst_size, 
                        dst,
                        il.Sub(dst_size,
                            il.Register(src0_size, src0),
                            il.Register(src1_size, src1))));
            break;
        } else {
            il.AddInstruction(il.Unimplemented());
            break;
        }
        break;
    }
    case OP_BITOP: {
        enum OpLiteral op = instr.operands[0].mnemonic;
        enum Register reg = instr.operands[1].reg;
        int  imm          = instr.operands[3].imm;
        int  reg_size     = get_register_size(reg);

        switch (op) {
        case OL_BITSET:
            il.AddInstruction(il.SetRegister(reg_size, reg, il.Or(reg_size, il.Register(reg_size, reg), il.Const(reg_size, 1 << imm))));
            break;
        case OL_BITCLR:
            il.AddInstruction(il.SetRegister(reg_size, reg, il.And(reg_size, il.Register(reg_size, reg), il.Const(reg_size, ~(1 << imm)))));
            break;
        case OL_BITTGL:
            il.AddInstruction(il.SetRegister(reg_size, reg, il.Xor(reg_size, il.Register(reg_size, reg), il.Const(reg_size, 1 << imm))));
            break;
        }
        break;
    }
    } // end switch(instr.operation)
    
    // FIXME: These class members tracking loopsetups are ugly and a bug waiting to happen, but work for now.
    if (this->il_has_loop) {
        if (this->il_next_loopend == addr) {
            LowLevelILLabel endLabel, continueLabel;
            il.AddInstruction(il.SetRegister(4, this->il_lc, il.Sub(4, il.Register(4, this->il_lc), il.Const(4, 1))));
            il.AddInstruction(il.If(il.CompareEqual(4, il.Register(4, this->il_lc), il.Const(4, 0)), endLabel, continueLabel));
            il.MarkLabel(continueLabel);
            il.AddInstruction(il.Jump(il.Const(4, this->il_next_loopstart)));
            il.MarkLabel(endLabel);
            this->il_next_loopend = 0;
            this->il_next_loopstart = 0;
            this->il_has_loop = false;
        }
    }
    return status;
}

class BlackfinLinuxSystemCallingConvention: public CallingConvention
{
public:
	BlackfinLinuxSystemCallingConvention(Architecture* arch): CallingConvention(arch, "blackfin-linux-syscall")
	{
	}

	virtual vector<uint32_t> GetIntegerArgumentRegisters() override
	{
		return vector<uint32_t>{
			REG_P0, REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5
		};
	}

	virtual vector<uint32_t> GetCallerSavedRegisters() override
	{
		return vector<uint32_t>{
            REG_R0
		};
	}


	virtual vector<uint32_t> GetCalleeSavedRegisters() override
	{
		return vector<uint32_t>{ 
            REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
            REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, 
            REG_FP, REG_RETS
        };
	}

	virtual uint32_t GetIntegerReturnValueRegister() override
	{
		return REG_R0;
	}

	virtual bool IsEligibleForHeuristics() override
	{
		return false;
	}
    
    virtual bool IsStackReservedForArgumentRegisters() override
	{
		return false;
	}
};

class BlackfinLinuxCallingConvention: public CallingConvention
{
public:
	BlackfinLinuxCallingConvention(Architecture* arch): CallingConvention(arch, "blackfin-linux")
	{
	}

	virtual vector<uint32_t> GetIntegerArgumentRegisters() override
	{
		return vector<uint32_t>{
			REG_R0, REG_R1, REG_R2
            // the rest on the stack
		};
	}

	virtual vector<uint32_t> GetCallerSavedRegisters() override
	{
		return vector<uint32_t>{
            REG_R0
		};
	}


	virtual vector<uint32_t> GetCalleeSavedRegisters() override
	{
		return vector<uint32_t>{ 
            REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
            REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, 
            REG_FP, REG_RETS
        };
	}

	virtual uint32_t GetIntegerReturnValueRegister() override
	{
		return REG_R0;
	}

	virtual bool IsEligibleForHeuristics() override
	{
		return false;
	}
    
    virtual bool IsStackReservedForArgumentRegisters() override
	{
		return true;
	}
};

class BlackfinBFLTRelocationHandler : public RelocationHandler {
public:
	virtual bool GetRelocationInfo(Ref<BinaryView> view, Ref<Architecture> arch, vector<BNRelocationInfo>& result) override
	{
		(void)view; (void)arch;
		for (auto& reloc : result)
		{
            reloc.addend = 0;
            reloc.baseRelative = false;
            reloc.dataRelocation = false;
            reloc.external = false;
            reloc.hasSign = false;
            reloc.implicitAddend = false;
            reloc.pcRelative = false;
		}
		return false;
	}

    virtual bool ApplyRelocation(Ref<BinaryView> view, Ref<Architecture> arch, Ref<Relocation> reloc, uint8_t* dest, size_t len) override
	{
		// Note: info.base contains preferred base address and the base where the image is actually loaded
		(void)view;
		(void)arch;
		(void)len;
		uint64_t* data64 = (uint64_t*)dest;
		uint32_t* data32 = (uint32_t*)dest;
		uint16_t* data16 = (uint16_t*)dest;
		auto info = reloc->GetInfo();
        /*
        LogInfo("info.address: %#08x", info.address);
        LogInfo("info.nativeType: %#08x", info.nativeType);
        LogInfo("dest: %p", dest);
        LogInfo("len: %p", len);
        LogInfo("size: %08x", info.size);
        LogInfo("target: %08x", reloc->GetTarget());
        LogInfo("address: %08x\n\n", reloc->GetAddress());
        */
		if (info.size == 4)
		{
			data32[0] = reloc->GetTarget();
		}
		else if (info.size == 2)
		{
			data16[0] = reloc->GetTarget();
		}
		return true;
	}

};

// TODO: This is really rudimentary, need to implement more reloc types
class BlackfineElfRelocationHandler : public RelocationHandler {
public:
    virtual bool ApplyRelocation(Ref<BinaryView> view, Ref<Architecture> arch, Ref<Relocation> reloc, uint8_t* dest, size_t len) override 
    {
        (void) view;
        auto info = reloc->GetInfo();
        if (len < info.size)
            return false;
        uint32_t target = (uint32_t)reloc->GetTarget();
        uint32_t* dest32 = (uint32_t*)dest;
        uint16_t* dest16 = (uint16_t*)dest;

        switch (info.nativeType) {
        case R_BFIN_HUIMM16:
            dest16[0] = ((target + info.addend) & 0xffff0000) >> 16;
            break;
        case R_BFIN_LUIMM16:
            dest16[0] = ((target + info.addend) & 0xffff);
            break;
        case R_BFIN_BYTE4_DATA:
            dest32[0] = target + info.addend;
            break;
        default:
            LogInfo("Unsupported bfin relocation type: %s (%lx)", reloc_strings[info.nativeType], info.nativeType);
            break;
        }

        return true;
    }
    virtual bool GetRelocationInfo(Ref<BinaryView> view, Ref<Architecture> arch, vector<BNRelocationInfo>& result) override 
    {
        (void)view; (void)arch;
        set<uint64_t> relocTypes;
        // TODO: does anything need to go in here? All we really 
        // need is what the ELF view already gives us
    
        for (auto& reloc : result) {
            reloc.type = StandardRelocationType;
            /*
            LogInfo("reloc.size: %lx\n", reloc.size);
            LogInfo("reloc.address: %lx\n", reloc.address);
            LogInfo("reloc.addend: %lx\n", reloc.addend);
            LogInfo("reloc.nativeType: %lx\n", reloc.nativeType);
            LogInfo("reloc.base: %lx\n", reloc.base);
            LogInfo("reloc.baseRelative: %lx\n", reloc.baseRelative);
            LogInfo("reloc.external: %lx\n", reloc.external);
            LogInfo("reloc.implicitAddend: %lx\n", reloc.implicitAddend);
            LogInfo("reloc.sectionIndex: %lx\n", reloc.sectionIndex);
            LogInfo("reloc.target: %lx\n", reloc.target);
            LogInfo("reloc.dataRelocation: %lx\n", reloc.dataRelocation);
            LogInfo("reloc.symbolIndex: %lx\n", reloc.symbolIndex);
            LogInfo(".");
            */
        }
        return true;
    }
};

extern "C" {
    BN_DECLARE_CORE_ABI_VERSION
    BINARYNINJAPLUGIN bool CorePluginInit()
    {
        BinaryNinja::Architecture *bfin = new BlackfinArchitecture("blackfin", BNEndianness::LittleEndian);
        BinaryNinja::Architecture::Register(bfin);

        Ref<CallingConvention> conv;
        conv = new BlackfinLinuxCallingConvention(bfin);
	    bfin->RegisterCallingConvention(conv);
	    bfin->SetDefaultCallingConvention(conv);
	    bfin->SetCdeclCallingConvention(conv);
	    bfin->SetFastcallCallingConvention(conv);
	    bfin->SetStdcallCallingConvention(conv);

        Ref<CallingConvention> syscall_conv;
        syscall_conv = new BlackfinLinuxSystemCallingConvention(bfin);
	    bfin->RegisterCallingConvention(syscall_conv);

        bfin->RegisterRelocationHandler("bFLT File", new BlackfinBFLTRelocationHandler());
        BinaryViewType::RegisterArchitecture("ELF", 106, LittleEndian, bfin);
        bfin->RegisterRelocationHandler("ELF", new BlackfineElfRelocationHandler());
        return true;
    }
}
