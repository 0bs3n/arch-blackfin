#pragma once

#include "binaryninjaapi.h"
#include "arch_blackfin.h"

#define IL_FLAG_AC0 0
#define IL_FLAG_AC1 1
#define IL_FLAG_AN 2
#define IL_FLAG_AQ 3
#define IL_FLAG_AV0 4
#define IL_FLAG_AVS0 5
#define IL_FLAG_AV1 6
#define IL_FLAG_AVS1 7
#define IL_FLAG_AZ 8
#define IL_FLAG_CC 9
#define IL_FLAG_V 10
#define IL_FLAG_VS 11


#define IL_FLAGWRITE_NONE 0
#define IL_FLAGWRITE_ALL 1
#define IL_FLAGWRITE_A0 2
#define IL_FLAGWRITE_A1 3
#define IL_FLAGWRITE_DR 4
#define IL_FLAGWRITE_CC 5

enum BlackfinIntrinsic {
    BFIN_INTRINSIC_LSETUP,
    BFIN_INTRINSIC_RAISE,
    BFIN_INTRINSIC_SYSCALL0,
    BFIN_INTRINSIC_SYSCALL1,
    BFIN_INTRINSIC_SYSCALL2,
    BFIN_INTRINSIC_SYSCALL3,
    BFIN_INTRINSIC_SYSCALL4,
    BFIN_INTRINSIC_SYSCALL5,
    BFIN_INTRINSIC_SYSCALL6
};

class BlackfinArchitecture : public BinaryNinja::Architecture {
public:
    BlackfinArchitecture(const char *name, BNEndianness _endian);
	virtual size_t GetAddressSize() const override;
	virtual BNEndianness GetEndianness() const override;
	virtual std::string GetFlagName(uint32_t flag) override;
	virtual std::string GetFlagWriteTypeName(uint32_t flags) override;
	virtual BNFlagRole GetFlagRole(uint32_t flag, uint32_t semClass = 0) override;
	virtual std::vector<uint32_t> GetFlagsWrittenByFlagWriteType(uint32_t flags) override;
	virtual std::vector<uint32_t> GetFlagsRequiredForFlagCondition(BNLowLevelILFlagCondition cond, uint32_t semClass) override;
	virtual size_t GetFlagWriteLowLevelIL(BNLowLevelILOperation op, size_t size, uint32_t flagWriteType, uint32_t flag,
			BNRegisterOrConstant* operands, size_t operandCount, BinaryNinja::LowLevelILFunction& il) override;
	virtual std::string GetRegisterName(uint32_t reg) override;
	virtual std::vector<uint32_t> GetFullWidthRegisters() override;
	virtual std::vector<uint32_t> GetAllRegisters() override;
	virtual std::vector<uint32_t> GetAllFlags() override;
	virtual std::vector<uint32_t> GetAllFlagWriteTypes() override;
	virtual BNRegisterInfo GetRegisterInfo(uint32_t reg) override;
	virtual uint32_t GetStackPointerRegister() override;
	virtual uint32_t GetLinkRegister() override;
    virtual bool CanAssemble() override;
    virtual int Disassemble(uint8_t *data, uint64_t addr, size_t maxLen, blackfin::Instruction &result);
    virtual bool GetInstructionText(const uint8_t *data, uint64_t addr, size_t &len, std::vector<BinaryNinja::InstructionTextToken>& result) override;
    virtual bool GetInstructionInfo(const uint8_t *data, uint64_t addr, size_t maxLen, BinaryNinja::InstructionInfo &result) override;
    virtual bool GetInstructionLowLevelIL(const uint8_t *data, uint64_t addr, size_t &len, BinaryNinja::LowLevelILFunction &il) override;
    virtual size_t GetDefaultIntegerSize() const override;
    virtual size_t GetInstructionAlignment() const override;
    virtual size_t GetMaxInstructionLength() const override;
	virtual std::string GetIntrinsicName(uint32_t intrinsic) override;
    virtual std::vector<uint32_t> GetAllIntrinsics() override;
    virtual std::vector<BinaryNinja::NameAndType> GetIntrinsicInputs(uint32_t intrinsic) override;
    virtual std::vector<BinaryNinja::Confidence<BinaryNinja::Ref<BinaryNinja::Type>>> GetIntrinsicOutputs(uint32_t intrinsic) override;
private:
    BNEndianness endian;
    uint64_t next_loopend;
    uint64_t next_loopstart;
    enum Register lc;
    bool has_loop;
    uint64_t il_next_loopend;
    uint64_t il_next_loopstart;
    enum Register il_lc;
    bool il_has_loop;
};
