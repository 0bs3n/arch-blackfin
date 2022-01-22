#pragma once

#include "binaryninjaapi.h"
#include "arch_blackfin.h"

#define IL_FLAG_N 0
#define IL_FLAG_Z 2
#define IL_FLAG_C 4
#define IL_FLAG_V 6
#define IL_FLAG_Q 8

#define IL_FLAGWRITE_NONE 0
#define IL_FLAGWRITE_ALL 1
#define IL_FLAGWRITE_NZ 2

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
    virtual size_t GetDefaultIntegerSize() const override;
    virtual size_t GetInstructionAlignment() const override;
    virtual size_t GetMaxInstructionLength() const override;
private:
    BNEndianness endian;
};