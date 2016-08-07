#include "simple-processor/simpleprocessor.h"

#include <map>
#include <thread>
#include "core/String.h"
#include "core/Util.h"

using namespace std;
using namespace Simulate;

const uint8_t SimpleProcessor::InitialPC = 0;

static map<SimpleProcessor::Opcode, string> mnemonics =
{
    {SimpleProcessor::Opcode::ACI, "ACI"},
    {SimpleProcessor::Opcode::ACX, "ACX"},
    {SimpleProcessor::Opcode::ADC, "ADC"},
    {SimpleProcessor::Opcode::ADD, "ADD"},
    {SimpleProcessor::Opcode::ADI, "ADI"},
    {SimpleProcessor::Opcode::ADX, "ADX"},
    {SimpleProcessor::Opcode::ANA, "ANA"},
    {SimpleProcessor::Opcode::ANI, "ANI"},
    {SimpleProcessor::Opcode::ANX, "ANX"},
    {SimpleProcessor::Opcode::BCC, "BCC"},
    {SimpleProcessor::Opcode::BCS, "BCS"},
    {SimpleProcessor::Opcode::BNG, "BNG"},
    {SimpleProcessor::Opcode::BNZ, "BNZ"},
    {SimpleProcessor::Opcode::BPZ, "BPZ"},
    {SimpleProcessor::Opcode::BRN, "BRN"},
    {SimpleProcessor::Opcode::BZE, "BZE"},
    {SimpleProcessor::Opcode::CLA, "CLA"},
    {SimpleProcessor::Opcode::CLC, "CLC"},
    {SimpleProcessor::Opcode::CLX, "CLX"},
    {SimpleProcessor::Opcode::CMC, "CMC"},
    {SimpleProcessor::Opcode::CMP, "CMP"},
    {SimpleProcessor::Opcode::CPI, "CPI"},
    {SimpleProcessor::Opcode::CPX, "CPX"},
    {SimpleProcessor::Opcode::DEC, "DEC"},
    {SimpleProcessor::Opcode::DEX, "DEX"},
    {SimpleProcessor::Opcode::HLT, "HLT"},
    {SimpleProcessor::Opcode::INA, "INA"},
    {SimpleProcessor::Opcode::INB, "INB"},
    {SimpleProcessor::Opcode::INC, "INC"},
    {SimpleProcessor::Opcode::INH, "INH"},
    {SimpleProcessor::Opcode::INI, "INI"},
    {SimpleProcessor::Opcode::INX, "INX"},
    {SimpleProcessor::Opcode::JSR, "JSR"},
    {SimpleProcessor::Opcode::LDA, "LDA"},
    {SimpleProcessor::Opcode::LDI, "LDI"},
    {SimpleProcessor::Opcode::LDX, "LDX"},
    {SimpleProcessor::Opcode::LSI, "LSI"},
    {SimpleProcessor::Opcode::LSP, "LSP"},
    {SimpleProcessor::Opcode::NOP, "NOP"},
    {SimpleProcessor::Opcode::ORA, "ORA"},
    {SimpleProcessor::Opcode::ORI, "ORI"},
    {SimpleProcessor::Opcode::ORX, "ORX"},
    {SimpleProcessor::Opcode::OTA, "OTA"},
    {SimpleProcessor::Opcode::OTB, "OTB"},
    {SimpleProcessor::Opcode::OTC, "OTC"},
    {SimpleProcessor::Opcode::OTH, "OTH"},
    {SimpleProcessor::Opcode::OTI, "OTI"},
    {SimpleProcessor::Opcode::POP, "POP"},
    {SimpleProcessor::Opcode::PSH, "PSH"},
    {SimpleProcessor::Opcode::RET, "RET"},
    {SimpleProcessor::Opcode::SBC, "SBC"},
    {SimpleProcessor::Opcode::SBI, "SBI"},
    {SimpleProcessor::Opcode::SBX, "SBX"},
    {SimpleProcessor::Opcode::SCI, "SCI"},
    {SimpleProcessor::Opcode::SCX, "SCX"},
    {SimpleProcessor::Opcode::SHL, "SHL"},
    {SimpleProcessor::Opcode::SHR, "SHR"},
    {SimpleProcessor::Opcode::STA, "STA"},
    {SimpleProcessor::Opcode::STX, "STX"},
    {SimpleProcessor::Opcode::SUB, "SUB"},
    {SimpleProcessor::Opcode::TAX, "TAX"},
};

static const SimpleProcessor::InstructionInfo instructions[256] =
{
        { 0x00, 1, 0, 1, 0, 1, {}, "NOP" },
        { 0x01, 1, 0, 1, 0, 1, {}, "CLA" },
        { 0x02, 1, 0, 1, 0, 1, {}, "CLC" },
        { 0x03, 1, 0, 1, 0, 1, {}, "CLX" },
        { 0x04, 1, 0, 1, 0, 1, {}, "CMC" },
        { 0x05, 1, 0, 1, 0, 1, {}, "INC" },
        { 0x06, 1, 0, 1, 0, 1, {}, "DEC" },
        { 0x07, 1, 0, 1, 0, 1, {}, "INX" },
        { 0x08, 1, 0, 1, 0, 1, {}, "DEX" },
        { 0x09, 1, 0, 1, 0, 1, {}, "TAX" },
        { 0x0A, 1, 0, 1, 0, 1, {}, "INI" },
        { 0x0B, 1, 0, 1, 0, 1, {}, "INH" },
        { 0x0C, 1, 0, 1, 0, 1, {}, "INB" },
        { 0x0D, 1, 0, 1, 0, 1, {}, "INA" },
        { 0x0E, 1, 0, 1, 0, 1, {}, "OTI" },
        { 0x0F, 1, 0, 1, 0, 1, {}, "OTC" },
        { 0x10, 1, 0, 1, 0, 1, {}, "OTH" },
        { 0x11, 1, 0, 1, 0, 1, {}, "OTB" },
        { 0x12, 1, 0, 1, 0, 1, {}, "OTA" },
        { 0x13, 1, 0, 1, 0, 1, {}, "PSH" },
        { 0x14, 1, 0, 1, 0, 1, {}, "POP" },
        { 0x15, 1, 0, 1, 0, 1, {}, "SHL" },
        { 0x16, 1, 0, 1, 0, 1, {}, "SHR" },
        { 0x17, 1, 0, 1, 0, 1, {}, "RET" },
        { 0x18, 1, 0, 1, 0, 1, {}, "HLT" },
        { 0x19, 2, 0, 2, 0, 2, {}, "LDA" },
        { 0x1A, 2, 0, 2, 0, 2, {}, "LDX" },
        { 0x1B, 2, 0, 2, 0, 2, {}, "LDI" },
        { 0x1C, 2, 0, 2, 0, 2, {}, "LSP" },
        { 0x1D, 2, 0, 2, 0, 2, {}, "LSI" },
        { 0x1E, 2, 0, 2, 0, 2, {}, "STA" },
        { 0x1F, 2, 0, 2, 0, 2, {}, "STX" },
        { 0x20, 2, 0, 2, 0, 2, {}, "ADD" },
        { 0x21, 2, 0, 2, 0, 2, {}, "ADX" },
        { 0x22, 2, 0, 2, 0, 2, {}, "ADI" },
        { 0x23, 2, 0, 2, 0, 2, {}, "ADC" },
        { 0x24, 2, 0, 2, 0, 2, {}, "ACX" },
        { 0x25, 2, 0, 2, 0, 2, {}, "ACI" },
        { 0x26, 2, 0, 2, 0, 2, {}, "SUB" },
        { 0x27, 2, 0, 2, 0, 2, {}, "SBX" },
        { 0x28, 2, 0, 2, 0, 2, {}, "SBI" },
        { 0x29, 2, 0, 2, 0, 2, {}, "SBC" },
        { 0x2A, 2, 0, 2, 0, 2, {}, "SCX" },
        { 0x2B, 2, 0, 2, 0, 2, {}, "SCI" },
        { 0x2C, 2, 0, 2, 0, 2, {}, "CMP" },
        { 0x2D, 2, 0, 2, 0, 2, {}, "CPX" },
        { 0x2E, 2, 0, 2, 0, 2, {}, "CPI" },
        { 0x2F, 2, 0, 2, 0, 2, {}, "ANA" },
        { 0x30, 2, 0, 2, 0, 2, {}, "ANX" },
        { 0x31, 2, 0, 2, 0, 2, {}, "ANI" },
        { 0x32, 2, 0, 2, 0, 2, {}, "ORA" },
        { 0x33, 2, 0, 2, 0, 2, {}, "ORX" },
        { 0x34, 2, 0, 2, 0, 2, {}, "ORI" },
        { 0x35, 2, 0, 2, 0, 2, {}, "BRN" },
        { 0x36, 2, 0, 2, 0, 2, {}, "BZE" },
        { 0x37, 2, 0, 2, 0, 2, {}, "BNZ" },
        { 0x38, 2, 0, 2, 0, 2, {}, "BPZ" },
        { 0x39, 2, 0, 2, 0, 2, {}, "BNG" },
        { 0x3A, 2, 0, 2, 0, 2, {}, "BCC" },
        { 0x3B, 2, 0, 2, 0, 2, {}, "BCS" },
        { 0x3C, 2, 0, 2, 0, 2, {}, "JSR" },
        { 0x3D, 0, 0, 0, 0, 0, {}, "" },
        { 0x3E, 0, 0, 0, 0, 0, {}, "" },
        { 0x3F, 0, 0, 0, 0, 0, {}, "" },
        { 0x40, 0, 0, 0, 0, 0, {}, "" },
        { 0x41, 0, 0, 0, 0, 0, {}, "" },
        { 0x42, 0, 0, 0, 0, 0, {}, "" },
        { 0x43, 0, 0, 0, 0, 0, {}, "" },
        { 0x44, 0, 0, 0, 0, 0, {}, "" },
        { 0x45, 0, 0, 0, 0, 0, {}, "" },
        { 0x46, 0, 0, 0, 0, 0, {}, "" },
        { 0x47, 0, 0, 0, 0, 0, {}, "" },
        { 0x48, 0, 0, 0, 0, 0, {}, "" },
        { 0x49, 0, 0, 0, 0, 0, {}, "" },
        { 0x4A, 0, 0, 0, 0, 0, {}, "" },
        { 0x4B, 0, 0, 0, 0, 0, {}, "" },
        { 0x4C, 0, 0, 0, 0, 0, {}, "" },
        { 0x4D, 0, 0, 0, 0, 0, {}, "" },
        { 0x4E, 0, 0, 0, 0, 0, {}, "" },
        { 0x4F, 0, 0, 0, 0, 0, {}, "" },
        { 0x50, 0, 0, 0, 0, 0, {}, "" },
        { 0x51, 0, 0, 0, 0, 0, {}, "" },
        { 0x52, 0, 0, 0, 0, 0, {}, "" },
        { 0x53, 0, 0, 0, 0, 0, {}, "" },
        { 0x54, 0, 0, 0, 0, 0, {}, "" },
        { 0x55, 0, 0, 0, 0, 0, {}, "" },
        { 0x56, 0, 0, 0, 0, 0, {}, "" },
        { 0x57, 0, 0, 0, 0, 0, {}, "" },
        { 0x58, 0, 0, 0, 0, 0, {}, "" },
        { 0x59, 0, 0, 0, 0, 0, {}, "" },
        { 0x5A, 0, 0, 0, 0, 0, {}, "" },
        { 0x5B, 0, 0, 0, 0, 0, {}, "" },
        { 0x5C, 0, 0, 0, 0, 0, {}, "" },
        { 0x5D, 0, 0, 0, 0, 0, {}, "" },
        { 0x5E, 0, 0, 0, 0, 0, {}, "" },
        { 0x5F, 0, 0, 0, 0, 0, {}, "" },
        { 0x60, 0, 0, 0, 0, 0, {}, "" },
        { 0x61, 0, 0, 0, 0, 0, {}, "" },
        { 0x62, 0, 0, 0, 0, 0, {}, "" },
        { 0x63, 0, 0, 0, 0, 0, {}, "" },
        { 0x64, 0, 0, 0, 0, 0, {}, "" },
        { 0x65, 0, 0, 0, 0, 0, {}, "" },
        { 0x66, 0, 0, 0, 0, 0, {}, "" },
        { 0x67, 0, 0, 0, 0, 0, {}, "" },
        { 0x68, 0, 0, 0, 0, 0, {}, "" },
        { 0x69, 0, 0, 0, 0, 0, {}, "" },
        { 0x6A, 0, 0, 0, 0, 0, {}, "" },
        { 0x6B, 0, 0, 0, 0, 0, {}, "" },
        { 0x6C, 0, 0, 0, 0, 0, {}, "" },
        { 0x6D, 0, 0, 0, 0, 0, {}, "" },
        { 0x6E, 0, 0, 0, 0, 0, {}, "" },
        { 0x6F, 0, 0, 0, 0, 0, {}, "" },
        { 0x70, 0, 0, 0, 0, 0, {}, "" },
        { 0x71, 0, 0, 0, 0, 0, {}, "" },
        { 0x72, 0, 0, 0, 0, 0, {}, "" },
        { 0x73, 0, 0, 0, 0, 0, {}, "" },
        { 0x74, 0, 0, 0, 0, 0, {}, "" },
        { 0x75, 0, 0, 0, 0, 0, {}, "" },
        { 0x76, 0, 0, 0, 0, 0, {}, "" },
        { 0x77, 0, 0, 0, 0, 0, {}, "" },
        { 0x78, 0, 0, 0, 0, 0, {}, "" },
        { 0x79, 0, 0, 0, 0, 0, {}, "" },
        { 0x7A, 0, 0, 0, 0, 0, {}, "" },
        { 0x7B, 0, 0, 0, 0, 0, {}, "" },
        { 0x7C, 0, 0, 0, 0, 0, {}, "" },
        { 0x7D, 0, 0, 0, 0, 0, {}, "" },
        { 0x7E, 0, 0, 0, 0, 0, {}, "" },
        { 0x7F, 0, 0, 0, 0, 0, {}, "" },
        { 0x80, 0, 0, 0, 0, 0, {}, "" },
        { 0x81, 0, 0, 0, 0, 0, {}, "" },
        { 0x82, 0, 0, 0, 0, 0, {}, "" },
        { 0x83, 0, 0, 0, 0, 0, {}, "" },
        { 0x84, 0, 0, 0, 0, 0, {}, "" },
        { 0x85, 0, 0, 0, 0, 0, {}, "" },
        { 0x86, 0, 0, 0, 0, 0, {}, "" },
        { 0x87, 0, 0, 0, 0, 0, {}, "" },
        { 0x88, 0, 0, 0, 0, 0, {}, "" },
        { 0x89, 0, 0, 0, 0, 0, {}, "" },
        { 0x8A, 0, 0, 0, 0, 0, {}, "" },
        { 0x8B, 0, 0, 0, 0, 0, {}, "" },
        { 0x8C, 0, 0, 0, 0, 0, {}, "" },
        { 0x8D, 0, 0, 0, 0, 0, {}, "" },
        { 0x8E, 0, 0, 0, 0, 0, {}, "" },
        { 0x8F, 0, 0, 0, 0, 0, {}, "" },
        { 0x90, 0, 0, 0, 0, 0, {}, "" },
        { 0x91, 0, 0, 0, 0, 0, {}, "" },
        { 0x92, 0, 0, 0, 0, 0, {}, "" },
        { 0x93, 0, 0, 0, 0, 0, {}, "" },
        { 0x94, 0, 0, 0, 0, 0, {}, "" },
        { 0x95, 0, 0, 0, 0, 0, {}, "" },
        { 0x96, 0, 0, 0, 0, 0, {}, "" },
        { 0x97, 0, 0, 0, 0, 0, {}, "" },
        { 0x98, 0, 0, 0, 0, 0, {}, "" },
        { 0x99, 0, 0, 0, 0, 0, {}, "" },
        { 0x9A, 0, 0, 0, 0, 0, {}, "" },
        { 0x9B, 0, 0, 0, 0, 0, {}, "" },
        { 0x9C, 0, 0, 0, 0, 0, {}, "" },
        { 0x9D, 0, 0, 0, 0, 0, {}, "" },
        { 0x9E, 0, 0, 0, 0, 0, {}, "" },
        { 0x9F, 0, 0, 0, 0, 0, {}, "" },
        { 0xA0, 0, 0, 0, 0, 0, {}, "" },
        { 0xA1, 0, 0, 0, 0, 0, {}, "" },
        { 0xA2, 0, 0, 0, 0, 0, {}, "" },
        { 0xA3, 0, 0, 0, 0, 0, {}, "" },
        { 0xA4, 0, 0, 0, 0, 0, {}, "" },
        { 0xA5, 0, 0, 0, 0, 0, {}, "" },
        { 0xA6, 0, 0, 0, 0, 0, {}, "" },
        { 0xA7, 0, 0, 0, 0, 0, {}, "" },
        { 0xA8, 0, 0, 0, 0, 0, {}, "" },
        { 0xA9, 0, 0, 0, 0, 0, {}, "" },
        { 0xAA, 0, 0, 0, 0, 0, {}, "" },
        { 0xAB, 0, 0, 0, 0, 0, {}, "" },
        { 0xAC, 0, 0, 0, 0, 0, {}, "" },
        { 0xAD, 0, 0, 0, 0, 0, {}, "" },
        { 0xAE, 0, 0, 0, 0, 0, {}, "" },
        { 0xAF, 0, 0, 0, 0, 0, {}, "" },
        { 0xB0, 0, 0, 0, 0, 0, {}, "" },
        { 0xB1, 0, 0, 0, 0, 0, {}, "" },
        { 0xB2, 0, 0, 0, 0, 0, {}, "" },
        { 0xB3, 0, 0, 0, 0, 0, {}, "" },
        { 0xB4, 0, 0, 0, 0, 0, {}, "" },
        { 0xB5, 0, 0, 0, 0, 0, {}, "" },
        { 0xB6, 0, 0, 0, 0, 0, {}, "" },
        { 0xB7, 0, 0, 0, 0, 0, {}, "" },
        { 0xB8, 0, 0, 0, 0, 0, {}, "" },
        { 0xB9, 0, 0, 0, 0, 0, {}, "" },
        { 0xBA, 0, 0, 0, 0, 0, {}, "" },
        { 0xBB, 0, 0, 0, 0, 0, {}, "" },
        { 0xBC, 0, 0, 0, 0, 0, {}, "" },
        { 0xBD, 0, 0, 0, 0, 0, {}, "" },
        { 0xBE, 0, 0, 0, 0, 0, {}, "" },
        { 0xBF, 0, 0, 0, 0, 0, {}, "" },
        { 0xC0, 0, 0, 0, 0, 0, {}, "" },
        { 0xC1, 0, 0, 0, 0, 0, {}, "" },
        { 0xC2, 0, 0, 0, 0, 0, {}, "" },
        { 0xC3, 0, 0, 0, 0, 0, {}, "" },
        { 0xC4, 0, 0, 0, 0, 0, {}, "" },
        { 0xC5, 0, 0, 0, 0, 0, {}, "" },
        { 0xC6, 0, 0, 0, 0, 0, {}, "" },
        { 0xC7, 0, 0, 0, 0, 0, {}, "" },
        { 0xC8, 0, 0, 0, 0, 0, {}, "" },
        { 0xC9, 0, 0, 0, 0, 0, {}, "" },
        { 0xCA, 0, 0, 0, 0, 0, {}, "" },
        { 0xCB, 0, 0, 0, 0, 0, {}, "" },
        { 0xCC, 0, 0, 0, 0, 0, {}, "" },
        { 0xCD, 0, 0, 0, 0, 0, {}, "" },
        { 0xCE, 0, 0, 0, 0, 0, {}, "" },
        { 0xCF, 0, 0, 0, 0, 0, {}, "" },
        { 0xD0, 0, 0, 0, 0, 0, {}, "" },
        { 0xD1, 0, 0, 0, 0, 0, {}, "" },
        { 0xD2, 0, 0, 0, 0, 0, {}, "" },
        { 0xD3, 0, 0, 0, 0, 0, {}, "" },
        { 0xD4, 0, 0, 0, 0, 0, {}, "" },
        { 0xD5, 0, 0, 0, 0, 0, {}, "" },
        { 0xD6, 0, 0, 0, 0, 0, {}, "" },
        { 0xD7, 0, 0, 0, 0, 0, {}, "" },
        { 0xD8, 0, 0, 0, 0, 0, {}, "" },
        { 0xD9, 0, 0, 0, 0, 0, {}, "" },
        { 0xDA, 0, 0, 0, 0, 0, {}, "" },
        { 0xDB, 0, 0, 0, 0, 0, {}, "" },
        { 0xDC, 0, 0, 0, 0, 0, {}, "" },
        { 0xDD, 0, 0, 0, 0, 0, {}, "" },
        { 0xDE, 0, 0, 0, 0, 0, {}, "" },
        { 0xDF, 0, 0, 0, 0, 0, {}, "" },
        { 0xE0, 0, 0, 0, 0, 0, {}, "" },
        { 0xE1, 0, 0, 0, 0, 0, {}, "" },
        { 0xE2, 0, 0, 0, 0, 0, {}, "" },
        { 0xE3, 0, 0, 0, 0, 0, {}, "" },
        { 0xE4, 0, 0, 0, 0, 0, {}, "" },
        { 0xE5, 0, 0, 0, 0, 0, {}, "" },
        { 0xE6, 0, 0, 0, 0, 0, {}, "" },
        { 0xE7, 0, 0, 0, 0, 0, {}, "" },
        { 0xE8, 0, 0, 0, 0, 0, {}, "" },
        { 0xE9, 0, 0, 0, 0, 0, {}, "" },
        { 0xEA, 0, 0, 0, 0, 0, {}, "" },
        { 0xEB, 0, 0, 0, 0, 0, {}, "" },
        { 0xEC, 0, 0, 0, 0, 0, {}, "" },
        { 0xED, 0, 0, 0, 0, 0, {}, "" },
        { 0xEE, 0, 0, 0, 0, 0, {}, "" },
        { 0xEF, 0, 0, 0, 0, 0, {}, "" },
        { 0xF0, 0, 0, 0, 0, 0, {}, "" },
        { 0xF1, 0, 0, 0, 0, 0, {}, "" },
        { 0xF2, 0, 0, 0, 0, 0, {}, "" },
        { 0xF3, 0, 0, 0, 0, 0, {}, "" },
        { 0xF4, 0, 0, 0, 0, 0, {}, "" },
        { 0xF5, 0, 0, 0, 0, 0, {}, "" },
        { 0xF6, 0, 0, 0, 0, 0, {}, "" },
        { 0xF7, 0, 0, 0, 0, 0, {}, "" },
        { 0xF8, 0, 0, 0, 0, 0, {}, "" },
        { 0xF9, 0, 0, 0, 0, 0, {}, "" },
        { 0xFA, 0, 0, 0, 0, 0, {}, "" },
        { 0xFB, 0, 0, 0, 0, 0, {}, "" },
        { 0xFC, 0, 0, 0, 0, 0, {}, "" },
        { 0xFD, 0, 0, 0, 0, 0, {}, "" },
        { 0xFE, 0, 0, 0, 0, 0, {}, "" },
        { 0xFF, 0, 0, 0, 0, 0, {}, "" },
};

SimpleProcessor::SimpleProcessor(double clockFreq,
                                 CharReader & reader, 
                                 CharWriter & writer)
    : Processor<AddressType, Opcode, Registers, InstructionInfo>(clockFreq, reader, writer)
{
}


SimpleProcessor::~SimpleProcessor()
{
}

void SimpleProcessor::Registers::Reset()
{
    a = {};
    sp = {};
    x = {};
    ir = {};
    pc = SimpleProcessor::InitialPC;
    flags = {};
    totalClockCount = 0;
    state = State::Uninitialized;
}

void SimpleProcessor::Reset()
{
    registers.Reset();
    clock.Reset();
    if ((debugMode & DebugMode::Trace) != 0)
    {
        for (auto observer : observers)
        {
            observer->Reset();
        }
    }
}

SimpleProcessor::Opcode SimpleProcessor::LookupOpcode(string const & str)
{
    for (auto entry : mnemonics)
    {
        if (Core::String::IsEqualIgnoreCase(str, entry.second))
        {
            return entry.first;
        }
    }
    return SimpleProcessor::Opcode::BAD_OPCODE;
}

SimpleProcessor::InstructionInfo SimpleProcessor::LookupOpcode(uint8_t opcode)
{
    SimpleProcessor::InstructionInfo instructionInfo = instructions[opcode];
    if (instructionInfo.instructionSize == 0)
    {
        throw IllegalInstructionException(opcode);
    }
    return instructionInfo;
}

void SimpleProcessor::FetchAndExecute()
{
    registers.state = State::Running;
    registers.pcLast = registers.pc;
    FetchInstruction();
    Execute(registers.ir);
}

void SimpleProcessor::FetchInstruction()
{
    if (memory == nullptr)
        throw UnassignedMemoryException();
    registers.ir = memory->Fetch(registers.pc++);
}

void Increment(uint8_t & value)
{
    value = uint8_t(value + 1);
}

void Decrement(uint8_t & value)
{
    value = uint8_t(value + 255);
}

void SimpleProcessor::SetFlags(uint8_t value)
{
    registers.flags = (registers.flags & Flags::C) | 
                      ((value == 0) ? Flags::Z : Flags::None) |
                      (((value == 0) || (value & 0x80)) ? Flags::None : Flags::P);
}

void SimpleProcessor::Load(uint8_t value)
{
    registers.a = value;
    SetFlags(registers.a);
}

void SimpleProcessor::Store(uint8_t value)
{
    if (memory == nullptr)
        throw UnassignedMemoryException();
    memory->Store(value, registers.a);
}

void SimpleProcessor::Add(uint8_t value)
{
    registers.flags = (registers.flags ^ ~Flags::C) | (((registers.a + value) > 255) ? Flags::C : Flags::None);
    registers.a = (registers.a + value) & 0xff;
    SetFlags(registers.a);
}

void SimpleProcessor::AddC(uint8_t value)
{
    uint8_t carry = (((registers.flags & Flags::C) != Flags::None) ? 1 : 0);
    registers.flags = (registers.flags ^ ~Flags::C) | (((registers.a + value + carry) > 255) ? Flags::C : Flags::None);
    registers.a = (registers.a + value + carry) & 0xff;
    SetFlags(registers.a);
}

void SimpleProcessor::Sub(uint8_t value)
{
    registers.flags = (registers.flags ^ ~Flags::C) | ((registers.a < value) ? Flags::C : Flags::None);
    registers.a = (registers.a - value + 256) & 0xff;
    SetFlags(registers.a);
}

void SimpleProcessor::SubC(uint8_t value)
{
    uint8_t carry = (((registers.flags & Flags::C) != Flags::None) ? 1 : 0);
    registers.flags = (registers.flags ^ ~Flags::C) | ((registers.a < (value + carry)) ? Flags::C : Flags::None);
    registers.a = (registers.a - value - carry + 256) & 0xff;
    SetFlags(registers.a);
}

void SimpleProcessor::Cmp(uint8_t value)
{
    registers.flags = (registers.flags ^ ~Flags::C) | ((registers.a < value) ? Flags::C : Flags::None);
    SetFlags((registers.a - value + 256) & 0xff);
}

void SimpleProcessor::And(uint8_t value)
{
    registers.flags = (registers.flags & ~Flags::C);
    registers.a &= value;
    SetFlags(registers.a);
}

void SimpleProcessor::Or(uint8_t value)
{
    registers.flags = (registers.flags & ~Flags::C);
    registers.a |= value;
    SetFlags(registers.a);
}

uint8_t SimpleProcessor::Index(uint8_t index)
{
    return (registers.x + index) & 0xFF;
}

bool IsValidCharacterForBase(char ch, int radix)
{
    switch (radix)
    {
    case 2:
        if (strchr("01", toupper(ch)) == nullptr)
            return false;
        break;
    case 10:
        if (strchr("0123456789+-", toupper(ch)) == nullptr)
            return false;
        break;
    case 16:
        if (strchr("0123456789ABCDEF", toupper(ch)) == nullptr)
            return false;
        break;
    default:
        return false;
    }
    return true;
}

uint8_t SimpleProcessor::GetNumber(int radix)
{
    if (reader.NoMoreData())
    {
        registers.state = State::NoData;
        return 0;
    }
    char ch = '\0';
    string str;
    while (!reader.NoMoreData() && IsValidCharacterForBase(ch = reader.ReadChar(), radix))
    {
        str += ch;
    }
    uint8_t value;
    if (!Core::Util::TryParse(str, value, radix))
    {
        registers.state = State::BadData;
        return 0;
    }
    return value;
}

uint8_t SimpleProcessor::GetChar()
{
    if (reader.NoMoreData())
    {
        registers.state = State::NoData;
        return 0;
    }

    return uint8_t(reader.ReadChar());
}

void SimpleProcessor::OutputNumber(int value, int radix)
{
    string str = Core::Util::ToString(value, radix);
    for (auto ch : str)
    {
        writer.WriteChar(ch);
    }
}

void SimpleProcessor::OutputNumber(uint8_t value, int radix)
{
    string str = Core::Util::ToString(value, radix);
    for (auto ch : str)
    {
        writer.WriteChar(ch);
    }
}

void SimpleProcessor::OutputChar(char value)
{
    writer.WriteChar(value);
}

void SimpleProcessor::Execute(uint8_t opcodeByte)
{
    if (memory == nullptr)
        throw UnassignedMemoryException();
    InstructionInfo info = LookupOpcode(opcodeByte);
    uint8_t value{};
    if (info.instructionSize > 1)
    {
        registers.operand = memory->Fetch(registers.pc++);
    }
    SimpleProcessor::Opcode opcode = SimpleProcessor::Opcode(opcodeByte);
    bool condition = true;
    switch (opcode)
    {
    case SimpleProcessor::Opcode::NOP:
        break;
    case SimpleProcessor::Opcode::CLA:
        registers.a = 0;
        break;
    case SimpleProcessor::Opcode::CLC:
        registers.flags = registers.flags & ~Flags::C;
        break;
    case SimpleProcessor::Opcode::CLX:
        registers.x = 0;
        break;
    case SimpleProcessor::Opcode::CMC:
        registers.flags = registers.flags ^ Flags::C;
        break;
    case SimpleProcessor::Opcode::INC:
        Increment(registers.a);
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::DEC:
        Decrement(registers.a);
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::INX:
        Increment(registers.x);
        SetFlags(registers.x);
        break;
    case SimpleProcessor::Opcode::DEX:
        Decrement(registers.x);
        SetFlags(registers.x);
        break;
    case SimpleProcessor::Opcode::TAX:
        registers.x = registers.a;
        break;
    case SimpleProcessor::Opcode::INI:
        registers.a = GetNumber(10);
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::INH:
        registers.a = GetNumber(16);
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::INB:
        registers.a = GetNumber(2);
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::INA:
        registers.a = GetChar();
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::OTI:
        OutputNumber((registers.a < 128) ? registers.a : registers.a - 256, 10);
        break;
    case SimpleProcessor::Opcode::OTC:
        OutputNumber(registers.a, 10);
        break;
    case SimpleProcessor::Opcode::OTH:
        OutputNumber(registers.a, 16);
        break;
    case SimpleProcessor::Opcode::OTB:
        OutputNumber(registers.a, 2);
        break;
    case SimpleProcessor::Opcode::OTA:
        OutputChar(registers.a);
        break;
    case SimpleProcessor::Opcode::PSH:
        Decrement(registers.sp);
        memory->Store(registers.sp, registers.a);
        break;
    case SimpleProcessor::Opcode::POP:
        registers.a = memory->Fetch(registers.sp);
        Increment(registers.sp);
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::SHL:
        registers.flags = (registers.flags & ~Flags::C) | ((registers.a & 0x80) ? Flags::C : Flags::None);
        registers.a <<= 1;
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::SHR:
        registers.flags = (registers.flags & ~Flags::C) | ((registers.a & 0x01) ? Flags::C : Flags::None);
        registers.a >>= 1;
        SetFlags(registers.a);
        break;
    case SimpleProcessor::Opcode::RET:
        registers.pc = memory->Fetch(registers.sp);
        Increment(registers.sp);
        break;
    case SimpleProcessor::Opcode::HLT:
        registers.state = State::Halted;
        break;
    case SimpleProcessor::Opcode::LDA:
        Load(memory->Fetch(registers.operand));
        break;
    case SimpleProcessor::Opcode::LDX:
        Load(memory->Fetch(Index(registers.operand)));
        break;
    case SimpleProcessor::Opcode::LDI:
        Load(registers.operand);
        break;
    case SimpleProcessor::Opcode::LSP:
        registers.sp = memory->Fetch(registers.operand);
        break;
    case SimpleProcessor::Opcode::LSI:
        registers.sp = registers.operand;
        break;
    case SimpleProcessor::Opcode::STA:
        Store(registers.operand);
        break;
    case SimpleProcessor::Opcode::STX:
        Store(Index(registers.operand));
        break;
    case SimpleProcessor::Opcode::ADD:
        Add(memory->Fetch(registers.operand));
        break;
    case SimpleProcessor::Opcode::ADX:
        Add(memory->Fetch(Index(registers.operand)));
        break;
    case SimpleProcessor::Opcode::ADI:
        Add(registers.operand);
        break;
    case SimpleProcessor::Opcode::ADC:
        AddC(memory->Fetch(registers.operand));
        break;
    case SimpleProcessor::Opcode::ACX:
        AddC(memory->Fetch(Index(registers.operand)));
        break;
    case SimpleProcessor::Opcode::ACI:
        AddC(registers.operand);
        break;
    case SimpleProcessor::Opcode::SUB:
        Sub(memory->Fetch(registers.operand));
        break;
    case SimpleProcessor::Opcode::SBX:
        Sub(memory->Fetch(Index(registers.operand)));
        break;
    case SimpleProcessor::Opcode::SBI:
        Sub(registers.operand);
        break;
    case SimpleProcessor::Opcode::SBC:
        SubC(memory->Fetch(registers.operand));
        break;
    case SimpleProcessor::Opcode::SCX:
        SubC(memory->Fetch(Index(registers.operand)));
        break;
    case SimpleProcessor::Opcode::SCI:
        SubC(registers.operand);
        break;
    case SimpleProcessor::Opcode::CMP:
        Cmp(memory->Fetch(registers.operand));
        break;
    case SimpleProcessor::Opcode::CPX:
        Cmp(memory->Fetch(Index(registers.operand)));
        break;
    case SimpleProcessor::Opcode::CPI:
        Cmp(registers.operand);
        break;
    case SimpleProcessor::Opcode::ANA:
        And(memory->Fetch(registers.operand));
        break;
    case SimpleProcessor::Opcode::ANX:
        And(memory->Fetch(Index(registers.operand)));
        break;
    case SimpleProcessor::Opcode::ANI:
        And(registers.operand);
        break;
    case SimpleProcessor::Opcode::ORA:
        Or(memory->Fetch(registers.operand));
        break;
    case SimpleProcessor::Opcode::ORX:
        Or(memory->Fetch(Index(registers.operand)));
        break;
    case SimpleProcessor::Opcode::ORI:
        Or(registers.operand);
        break;
    case SimpleProcessor::Opcode::BRN:
        registers.pc = registers.operand;
        break;
    case SimpleProcessor::Opcode::BZE:
        if ((registers.flags & Flags::Z) != Flags::None)
            registers.pc = registers.operand;
        break;
    case SimpleProcessor::Opcode::BNZ:
        if ((registers.flags & Flags::Z) == Flags::None)
            registers.pc = registers.operand;
        break;
    case SimpleProcessor::Opcode::BPZ:
        if ((registers.flags & Flags::P) != Flags::None)
            registers.pc = registers.operand;
        break;
    case SimpleProcessor::Opcode::BNG:
        if ((registers.flags & Flags::P) == Flags::None)
            registers.pc = registers.operand;
        break;
    case SimpleProcessor::Opcode::BCC:
        if ((registers.flags & Flags::C) == Flags::None)
            registers.pc = registers.operand;
        break;
    case SimpleProcessor::Opcode::BCS:
        if ((registers.flags & Flags::C) != Flags::None)
            registers.pc = registers.operand;
        break;
    case SimpleProcessor::Opcode::JSR:
        Decrement(registers.sp);
        memory->Store(registers.sp, registers.pc);
        registers.pc = registers.operand;
        break;
    default:
        throw IllegalInstructionException(opcodeByte);
    }
    registers.totalClockCount += (condition ? info.cycleCount : info.cycleCountConditionFailed);
    if ((debugMode & DebugMode::Trace) != 0)
    {
        for (auto observer : observers)
        {
            observer->Trace(info, registers);
        }
    }
    if ((debugMode & DebugMode::NonRealTime) == 0)
    {
        clock.Wait(registers.totalClockCount);
    }
}

