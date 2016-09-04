#include "Assembler.h"

namespace Assembler
{

Assembler::Assembler(Simulate::SimpleMachine & machine, std::istream & source, std::wostream & reportStream)
    : machine(machine)
    , source(source)
    , scanner(&source, false, reportStream)
    , opcodeKeywords()
{
    opcodeKeywords.Set(L"NOP", OpcodeType::NOP);
    opcodeKeywords.Set(L"LXI", OpcodeType::LXI);
    opcodeKeywords.Set(L"STAX", OpcodeType::STAX);
    opcodeKeywords.Set(L"INX", OpcodeType::INX);
    opcodeKeywords.Set(L"INR", OpcodeType::INR);
    opcodeKeywords.Set(L"DCR", OpcodeType::DCR);
    opcodeKeywords.Set(L"RLC", OpcodeType::RLC);
    opcodeKeywords.Set(L"DAD", OpcodeType::DAD);
    opcodeKeywords.Set(L"LDAX", OpcodeType::LDAX);
    opcodeKeywords.Set(L"DCX", OpcodeType::DCX);
    opcodeKeywords.Set(L"RRC", OpcodeType::RRC);
    opcodeKeywords.Set(L"RAL", OpcodeType::RAL);
    opcodeKeywords.Set(L"RAR", OpcodeType::RAR);
    opcodeKeywords.Set(L"SHLD", OpcodeType::SHLD);
    opcodeKeywords.Set(L"DAA", OpcodeType::DAA);
    opcodeKeywords.Set(L"LHLD", OpcodeType::LHLD);
    opcodeKeywords.Set(L"CMA", OpcodeType::CMA);
    opcodeKeywords.Set(L"STA", OpcodeType::STA);
    opcodeKeywords.Set(L"MVI", OpcodeType::MVI);
    opcodeKeywords.Set(L"STC", OpcodeType::STC);
    opcodeKeywords.Set(L"LDA", OpcodeType::LDA);
    opcodeKeywords.Set(L"CMC", OpcodeType::CMC);
    opcodeKeywords.Set(L"MOV", OpcodeType::MOV);
    opcodeKeywords.Set(L"HLT", OpcodeType::HLT);
    opcodeKeywords.Set(L"ADD", OpcodeType::ADD);
    opcodeKeywords.Set(L"ADC", OpcodeType::ADC);
    opcodeKeywords.Set(L"SUB", OpcodeType::SUB);
    opcodeKeywords.Set(L"SBB", OpcodeType::SBB);
    opcodeKeywords.Set(L"ANA", OpcodeType::ANA);
    opcodeKeywords.Set(L"XRA", OpcodeType::XRA);
    opcodeKeywords.Set(L"ORA", OpcodeType::ORA);
    opcodeKeywords.Set(L"CMP", OpcodeType::CMP);
    opcodeKeywords.Set(L"RNZ", OpcodeType::RNZ);
    opcodeKeywords.Set(L"POP", OpcodeType::POP);
    opcodeKeywords.Set(L"JNZ", OpcodeType::JNZ);
    opcodeKeywords.Set(L"JMP", OpcodeType::JMP);
    opcodeKeywords.Set(L"CNZ", OpcodeType::CNZ);
    opcodeKeywords.Set(L"PUSH", OpcodeType::PUSH);
    opcodeKeywords.Set(L"ADI", OpcodeType::ADI);
    opcodeKeywords.Set(L"RST", OpcodeType::RST);
    opcodeKeywords.Set(L"RZ", OpcodeType::RZ);
    opcodeKeywords.Set(L"RET", OpcodeType::RET);
    opcodeKeywords.Set(L"JZ", OpcodeType::JZ);
    opcodeKeywords.Set(L"CZ", OpcodeType::CZ);
    opcodeKeywords.Set(L"CALL", OpcodeType::CALL);
    opcodeKeywords.Set(L"ACI", OpcodeType::ACI);
    opcodeKeywords.Set(L"RNC", OpcodeType::RNC);
    opcodeKeywords.Set(L"JNC", OpcodeType::JNC);
    opcodeKeywords.Set(L"OUTP", OpcodeType::OUTP);
    opcodeKeywords.Set(L"CNC", OpcodeType::CNC);
    opcodeKeywords.Set(L"SUI", OpcodeType::SUI);
    opcodeKeywords.Set(L"RC", OpcodeType::RC);
    opcodeKeywords.Set(L"JC", OpcodeType::JC);
    opcodeKeywords.Set(L"INP", OpcodeType::INP);
    opcodeKeywords.Set(L"CC", OpcodeType::CC);
    opcodeKeywords.Set(L"SBI", OpcodeType::SBI);
    opcodeKeywords.Set(L"RPO", OpcodeType::RPO);
    opcodeKeywords.Set(L"JPO", OpcodeType::JPO);
    opcodeKeywords.Set(L"XTHL", OpcodeType::XTHL);
    opcodeKeywords.Set(L"CPO", OpcodeType::CPO);
    opcodeKeywords.Set(L"ANI", OpcodeType::ANI);
    opcodeKeywords.Set(L"RPE", OpcodeType::RPE);
    opcodeKeywords.Set(L"PCHL", OpcodeType::PCHL);
    opcodeKeywords.Set(L"JPE", OpcodeType::JPE);
    opcodeKeywords.Set(L"XCHG", OpcodeType::XCHG);
    opcodeKeywords.Set(L"CPE", OpcodeType::CPE);
    opcodeKeywords.Set(L"XRI", OpcodeType::XRI);
    opcodeKeywords.Set(L"RP", OpcodeType::RP);
    opcodeKeywords.Set(L"JP", OpcodeType::JP);
    opcodeKeywords.Set(L"DI", OpcodeType::DI);
    opcodeKeywords.Set(L"CP", OpcodeType::CP);
    opcodeKeywords.Set(L"ORI", OpcodeType::ORI);
    opcodeKeywords.Set(L"RM", OpcodeType::RM);
    opcodeKeywords.Set(L"SPHL", OpcodeType::SPHL);
    opcodeKeywords.Set(L"JM", OpcodeType::JM);
    opcodeKeywords.Set(L"EI", OpcodeType::EI);
    opcodeKeywords.Set(L"CM =", OpcodeType::CM);
    opcodeKeywords.Set(L"CPI", OpcodeType::CPI);
// 8085 only
    opcodeKeywords.Set(L"DSUB", OpcodeType::DSUB);
    opcodeKeywords.Set(L"ARHL", OpcodeType::ARHL);
    opcodeKeywords.Set(L"RDEL", OpcodeType::RDEL);
    opcodeKeywords.Set(L"RIM", OpcodeType::RIM);
    opcodeKeywords.Set(L"LDHI", OpcodeType::LDHI);
    opcodeKeywords.Set(L"SIM", OpcodeType::SIM);
    opcodeKeywords.Set(L"LDSI", OpcodeType::LDSI);
    opcodeKeywords.Set(L"RSTV", OpcodeType::RSTV);
    opcodeKeywords.Set(L"SHLX", OpcodeType::SHLX);
    opcodeKeywords.Set(L"JNK", OpcodeType::JNK);
    opcodeKeywords.Set(L"LHLX", OpcodeType::LHLX);
    opcodeKeywords.Set(L"JK", OpcodeType::JK);
}

} // namespace Assembler
