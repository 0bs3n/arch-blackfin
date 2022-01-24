#include "BlackfinArchitecture.h"
#include "binaryninjaapi.h"
#include "binaryninjacore.h"
#include "lowlevelilinstruction.h"
#include "arch_blackfin.h"
#include "Disassembler.h"
#include <string.h>

#define MYLOG BinaryNinja::LogInfo
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
    // MYLOG("%s()\n", __func__);
    return 4;
}

size_t BlackfinArchitecture::GetDefaultIntegerSize() const
{
    // MYLOG("%s()\n", __func__);
    return 4;
}

size_t BlackfinArchitecture::GetInstructionAlignment() const
{
    // MYLOG("%s()\n", __func__);
    return 2;
}

size_t BlackfinArchitecture::GetMaxInstructionLength() const
{
    // MYLOG("%s()\n", __func__);
    return 4;
}

std::string BlackfinArchitecture::GetFlagName(uint32_t flag) { 
    char result[32];
	switch (flag)
	{
	case IL_FLAG_N:
		return "n";
	case IL_FLAG_Z:
		return "z";
	case IL_FLAG_C:
		return "c";
	case IL_FLAG_V:
		return "v";
	case IL_FLAG_Q:
		return "q";
	default:
		sprintf(result, "flag%" PRIu32, flag);
		return result;
	}
}

std::string BlackfinArchitecture::GetFlagWriteTypeName(uint32_t flags) {
    	switch (flags)
	{
		case IL_FLAGWRITE_ALL: return "*";
		case IL_FLAGWRITE_NZ: return "nz";
		default:
			return "";
	}
}

BNFlagRole BlackfinArchitecture::GetFlagRole(uint32_t flag, uint32_t semClass) {
    switch (flag)
	{
	case IL_FLAG_N:
		return NegativeSignFlagRole;
	case IL_FLAG_Z:
		return ZeroFlagRole;
	case IL_FLAG_C:
		return CarryFlagRole;
	case IL_FLAG_V:
		return OverflowFlagRole;
	default:
		return SpecialFlagRole;
	}
}

std::vector<uint32_t> BlackfinArchitecture::GetFlagsWrittenByFlagWriteType(uint32_t flags) {
    switch (flags)
	{
	case IL_FLAGWRITE_ALL:
		return vector<uint32_t> { IL_FLAG_N, IL_FLAG_Z, IL_FLAG_C, IL_FLAG_V };
	case IL_FLAGWRITE_NZ:
		return vector<uint32_t> { IL_FLAG_N, IL_FLAG_Z };
	default:
		return vector<uint32_t> {};
	}
}

std::vector<uint32_t> BlackfinArchitecture::GetFlagsRequiredForFlagCondition(BNLowLevelILFlagCondition cond, uint32_t semClass) {
    switch (cond)
	{
	case LLFC_E:
	case LLFC_NE:
		return vector<uint32_t>{ IL_FLAG_Z };
	case LLFC_SLT:
	case LLFC_SGE:
		return vector<uint32_t>{ IL_FLAG_N, IL_FLAG_V };
	case LLFC_ULT:
	case LLFC_UGE:
		return vector<uint32_t>{ IL_FLAG_C };
	case LLFC_SLE:
	case LLFC_SGT:
		return vector<uint32_t>{ IL_FLAG_Z, IL_FLAG_N, IL_FLAG_V };
	case LLFC_ULE:
	case LLFC_UGT:
		return vector<uint32_t>{ IL_FLAG_C, IL_FLAG_Z };
	case LLFC_NEG:
	case LLFC_POS:
		return vector<uint32_t>{ IL_FLAG_N };
	case LLFC_O:
	case LLFC_NO:
		return vector<uint32_t>{ IL_FLAG_V };
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
        // is there a dedicated binja logging function?
        // LogError("Unknown Register: %x -- investigate!\n", reg);
        return "unknown";
    }
}

std::vector<uint32_t> BlackfinArchitecture::GetFullWidthRegisters() {
    std::vector<uint32_t> vec; 
    vec.push_back(1);
    return vec;
}

std::vector<uint32_t> BlackfinArchitecture::GetAllRegisters() {
    std::vector<uint32_t> vec; 
    vec.push_back(1);
    return vec;
}

std::vector<uint32_t> BlackfinArchitecture::GetAllFlags() {
    return vector<uint32_t>{
		IL_FLAG_N, IL_FLAG_Z, IL_FLAG_C, IL_FLAG_V, IL_FLAG_Q
	};
}

std::vector<uint32_t> BlackfinArchitecture::GetAllFlagWriteTypes() {
    return vector<uint32_t>{
		IL_FLAGWRITE_ALL,
		IL_FLAGWRITE_NZ
	};
}
/*
enum Register
{
  REG_AZ, REG_AN, REG_AC0, REG_AC1, REG_AV0, REG_AV1, REG_AV0S, REG_AV1S, REG_AQ, REG_V, REG_VS, REG_CC, 
  REG_sftreset, REG_omode, REG_excause, REG_emucause, REG_idle_req, REG_hwerrcause, 
  REG_EMUDAT, REG_BR0, REG_BR1, REG_BR2, REG_BR3, REG_BR4, REG_BR5, REG_BR6, REG_BR7, 
  REG_AC0_COPY, REG_V_COPY, REG_RND_MOD,
  REG_LASTREG,
};
*/

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
        return RegisterInfo(reg, 0, 1);

    // low 32 bits of accumulator
    case REG_A0w:
    case REG_A1w:
        return RegisterInfo(reg, 1, 4);

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
        return RegisterInfo(reg, 2, 2);
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
        return RegisterInfo(reg, 0, 2);
    default:
        return RegisterInfo(0, 0, 0); // FIXME: Better fail case?
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
    MYLOG("len: %lu", len);
    blackfin::Instruction instr;
    memset(&instr, 0, sizeof(instr));
    int instr_size = Disassemble((uint8_t*)data, addr, 0, instr);
    MYLOG("reported len: %u", instr_size);
    MYLOG("instr operation: %d", instr.operation);
    MYLOG("instr operation op count: %d", instr.operand_count);
    MYLOG("instr opcode: %x", instr.opcode);

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
        MYLOG("operand type: %d", operand.cls);
        char imm_string[60] = {0};
        if (operand.flags.cc_inverted) {
            result.emplace_back(TextToken, "!");
        }
        switch (operand.cls) {
        case blackfin::NONE:
            return true;

        case blackfin::MNEMOMIC:
            MYLOG("got mnemonic operand");
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
            MYLOG("got register operand");
            const char *inc_dec_symbol;
            result.emplace_back(RegisterToken, GetRegisterName(operand.reg));
            if (instr.operands[i + 1].cls == blackfin::REG) result.emplace_back(TextToken, " ");
            break; 

        case blackfin::OPERATOR:
            MYLOG("got operator operand");
            result.emplace_back(TextToken, blackfin::get_operator_string(operand.operat));
            break;

        case blackfin::IMM:
            MYLOG("got imm operand");
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
            MYLOG("got uimm operand");
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
            if (operand.mem_access.width == 1) result.emplace_back(TextToken, "B");
            if (operand.mem_access.width == 2) result.emplace_back(TextToken, "W");
            result.emplace_back(BeginMemoryOperandToken, "[");
            switch (operand.mem_access.mode) {
            case blackfin::MEM_REGMOD:
                if (operand.mem_access.pre_dec) result.emplace_back(TextToken, "--");
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.ptr_reg));
                if (operand.mem_access.post_inc) result.emplace_back(TextToken, "++");
                break;
            case blackfin::MEM_REG:
                printf("regname: %s", GetRegisterName(operand.mem_access.ptr_reg).c_str());
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.ptr_reg));
                break;
            case blackfin::MEM_IMM:
                sprintf(imm_string, "%#x", operand.mem_access.ptr_imm);
                result.emplace_back(IntegerToken, imm_string, operand.mem_access.ptr_imm);
                break;
            case blackfin::MEM_REGIMM:
                printf("regname: %s", GetRegisterName(operand.mem_access.ptr_reg).c_str());
                sprintf(imm_string, "%#x", operand.mem_access.idx_imm);
                result.emplace_back(RegisterToken, GetRegisterName(operand.mem_access.ptr_reg));
                result.emplace_back(TextToken, get_operator_string(operand.mem_access.oper));
                if (operand.mem_access.idx_imm != 1) result.emplace_back(IntegerToken, imm_string, operand.mem_access.idx_imm);
                break;
            case blackfin::MEM_REGREG:
                printf("regname: %s", GetRegisterName(operand.mem_access.ptr_reg).c_str());
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
        if (operand.flags.sign_extended) result.emplace_back(TextToken, " (X)");
        if (operand.flags.zero_extended) result.emplace_back(TextToken, " (Z)");
    }

    /*
    if (instr.operation == blackfin::OP_MOVIMM) {
        result.emplace_back(RegisterToken, GetRegisterName(instr.operands[0].reg));
        result.emplace_back(TextToken, " ");
        result.emplace_back(OperandSeparatorToken, instr.operands[1].operat == blackfin::OPER_EQ ? "=" : "+=");
        result.emplace_back(TextToken, " ");
        char operand_string[60];
        sprintf(operand_string, "%x", instr.operands[2].imm);
        result.emplace_back(IntegerToken, operand_string, instr.operands[2].imm);
    }
    */
    return true;
}

bool BlackfinArchitecture::GetInstructionInfo(const uint8_t *data, uint64_t addr, size_t maxLen, BinaryNinja::InstructionInfo &result) {

    blackfin::Instruction instr;
    int instr_size = Disassemble((uint8_t*)data, addr, 0, instr);
    if (instr.operation == blackfin::OP_ILLEGAL) return true;
    result.length = instr_size;
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
        }
    }
    return true;
}

/*
static void RegisterBlackfinArchitecture() {
    BlackfinArchitecture *bfin = new BlackfinArchitecture("blackfin");
    BinaryNinja::Architecture::Register(bfin);
}
*/

extern "C" {
    BN_DECLARE_CORE_ABI_VERSION
    BINARYNINJAPLUGIN bool CorePluginInit()
    {
        BinaryNinja::Architecture *bfin = new BlackfinArchitecture("blackfin", BNEndianness::LittleEndian);
        BinaryNinja::Architecture::Register(bfin);
        return true;
    }
}
