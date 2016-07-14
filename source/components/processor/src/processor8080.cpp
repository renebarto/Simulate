#include <processor8080.h>

#include <iomanip>
#include <iostream>
#include <sstream>

using namespace Simulate;

static const Reg16 InitialPC = 0;

static const InstructionData8080 instructionData8080[256] =
{
    /*00*/ { 4,  0, 0, 1, "NOP" },
    /*01*/ { 10, 0, 3, 3, "LXI B," },
    /*02*/ { 7,  0, 2, 1, "STAX B" },
    /*03*/ { 5,  0, 1, 1, "INX B" },
    /*04*/ { 5,  0, 1, 1, "INR B" },
    /*05*/ { 5,  0, 1, 1, "DCR B" },
    /*06*/ { 0,  0, 0, 0, "" },
    /*07*/ { 4,  0, 1, 1, "RLC" },
    /*08*/ { 0,  0, 0, 0, "" },
    /*09*/ { 10, 0, 3, 1, "DAD B" },
    /*0A*/ { 7,  0, 2, 1, "LDAX B" },
    /*0B*/ { 5,  0, 1, 1, "DCX B" },
    /*0C*/ { 5,  0, 1, 1, "INR C" },
    /*0D*/ { 5,  0, 1, 1, "DCR C" },
    /*0E*/ { 0,  0, 0, 0, "" },
    /*0F*/ { 4,  0, 1, 1, "RRC" },
    /*10*/ { 0,  0, 0, 0, "" },
    /*11*/ { 10, 0, 3, 3, "LXI D," },
    /*12*/ { 7,  0, 2, 1, "STAX D" },
    /*13*/ { 5,  0, 1, 1, "INX D" },
    /*14*/ { 5,  0, 1, 1, "INR D" },
    /*15*/ { 5,  0, 1, 1, "DCR D" },
    /*16*/ { 0,  0, 0, 0, "" },
    /*17*/ { 4,  0, 1, 1, "RAL" },
    /*18*/ { 0,  0, 0, 0, "" },
    /*19*/ { 10, 0, 3, 1, "DAD D" },
    /*1A*/ { 7,  0, 2, 1, "LDAX D" },
    /*1B*/ { 5,  0, 1, 1, "DCX D" },
    /*1C*/ { 5,  0, 1, 1, "INR E" },
    /*1D*/ { 5,  0, 1, 1, "DCR E" },
    /*1E*/ { 0,  0, 0, 0, "" },
    /*1F*/ { 4,  0, 1, 1, "RAR" },
    /*20*/ { 0,  0, 0, 0, "" },
    /*21*/ { 10, 0, 3, 3, "LXI H," },
    /*22*/ { 16, 0, 5, 3, "SHLD " },
    /*23*/ { 5,  0, 1, 1, "INX H" },
    /*24*/ { 5,  0, 1, 1, "INR H" },
    /*25*/ { 5,  0, 1, 1, "DCR H" },
    /*26*/ { 0,  0, 0, 0, "" },
    /*27*/ { 4,  0, 1, 1, "DAA" },
    /*28*/ { 0,  0, 0, 0, "" },
    /*29*/ { 10, 0, 3, 1, "DAD H" },
    /*2A*/ { 16, 0, 5, 3, "LHLD " },
    /*2B*/ { 5,  0, 1, 1, "DCX H" },
    /*2C*/ { 5,  0, 1, 1, "INR L" },
    /*2D*/ { 5,  0, 1, 1, "DCR L" },
    /*2E*/ { 0,  0, 0, 0, "" },
    /*2F*/ { 4,  0, 1, 1, "CMA" },
    /*30*/ { 0,  0, 0, 0, "" },
    /*31*/ { 10, 0, 3, 3, "LXI SP," },
    /*32*/ { 13, 0, 4, 3, "STA " },
    /*33*/ { 5,  0, 1, 1, "INX SP" },
    /*34*/ { 10, 0, 3, 1, "INR M" },
    /*35*/ { 10, 0, 3, 1, "DCR M" },
    /*36*/ { 10, 0, 3, 2, "MVI M," },
    /*37*/ { 4,  0, 1, 1, "STC" },
    /*38*/ { 0,  0, 0, 0, "" },
    /*39*/ { 10, 0, 3, 1, "DAD SP" },
    /*3A*/ { 13, 0, 4, 3, "LDA " },
    /*3B*/ { 5,  0, 1, 1, "DCX SP" },
    /*3C*/ { 5,  0, 1, 1, "INR A" },
    /*3D*/ { 5,  0, 1, 1, "DCR A" },
    /*3E*/ { 0,  0, 0, 0, "" },
    /*3F*/ { 4,  0, 1, 1, "CMC" },
    /*40*/ { 5,  0, 1, 1, "MOV B,B" },
    /*41*/ { 5,  0, 1, 1, "MOV B,C" },
    /*42*/ { 5,  0, 1, 1, "MOV B,D" },
    /*43*/ { 5,  0, 1, 1, "MOV B,E" },
    /*44*/ { 5,  0, 1, 1, "MOV B,H" },
    /*45*/ { 5,  0, 1, 1, "MOV B,L" },
    /*46*/ { 7,  0, 2, 1, "MOV B,M" },
    /*47*/ { 5,  0, 1, 1, "MOV B,A" },
    /*48*/ { 5,  0, 1, 1, "MOV C,B" },
    /*49*/ { 5,  0, 1, 1, "MOV C,C" },
    /*4A*/ { 5,  0, 1, 1, "MOV C,D" },
    /*4B*/ { 5,  0, 1, 1, "MOV C,E" },
    /*4C*/ { 5,  0, 1, 1, "MOV C,H" },
    /*4D*/ { 5,  0, 1, 1, "MOV C,L" },
    /*4E*/ { 7,  0, 2, 1, "MOV C,M" },
    /*4F*/ { 5,  0, 1, 1, "MOV C,A" },
    /*50*/ { 5,  0, 1, 1, "MOV D,B" },
    /*51*/ { 5,  0, 1, 1, "MOV D,C" },
    /*52*/ { 5,  0, 1, 1, "MOV D,D" },
    /*53*/ { 5,  0, 1, 1, "MOV D,E" },
    /*54*/ { 5,  0, 1, 1, "MOV D,H" },
    /*55*/ { 5,  0, 1, 1, "MOV D,L" },
    /*56*/ { 7,  0, 2, 1, "MOV D,M" },
    /*57*/ { 5,  0, 1, 1, "MOV D,A" },
    /*58*/ { 5,  0, 1, 1, "MOV E,B" },
    /*59*/ { 5,  0, 1, 1, "MOV E,C" },
    /*5A*/ { 5,  0, 1, 1, "MOV E,D" },
    /*5B*/ { 5,  0, 1, 1, "MOV E,E" },
    /*5C*/ { 5,  0, 1, 1, "MOV E,H" },
    /*5D*/ { 5,  0, 1, 1, "MOV E,L" },
    /*5E*/ { 7,  0, 2, 1, "MOV E,M" },
    /*5F*/ { 5,  0, 1, 1, "MOV E,A" },
    /*60*/ { 5,  0, 1, 1, "MOV H,B" },
    /*61*/ { 5,  0, 1, 1, "MOV H,C" },
    /*62*/ { 5,  0, 1, 1, "MOV H,D" },
    /*63*/ { 5,  0, 1, 1, "MOV H,E" },
    /*64*/ { 5,  0, 1, 1, "MOV H,H" },
    /*65*/ { 5,  0, 1, 1, "MOV H,L" },
    /*66*/ { 7,  0, 2, 1, "MOV H,M" },
    /*67*/ { 5,  0, 1, 1, "MOV H,A" },
    /*68*/ { 5,  0, 1, 1, "MOV L,B" },
    /*69*/ { 5,  0, 1, 1, "MOV L,C" },
    /*6A*/ { 5,  0, 1, 1, "MOV L,D" },
    /*6B*/ { 5,  0, 1, 1, "MOV L,E" },
    /*6C*/ { 5,  0, 1, 1, "MOV L,H" },
    /*6D*/ { 5,  0, 1, 1, "MOV L,L" },
    /*6E*/ { 7,  0, 2, 1, "MOV L,M" },
    /*6F*/ { 5,  0, 1, 1, "MOV L,A" },
    /*70*/ { 7,  0, 2, 1, "MOV M,B" },
    /*71*/ { 7,  0, 2, 1, "MOV M,C" },
    /*72*/ { 7,  0, 2, 1, "MOV M,D" },
    /*73*/ { 7,  0, 2, 1, "MOV M,E" },
    /*74*/ { 7,  0, 2, 1, "MOV M,H" },
    /*75*/ { 7,  0, 2, 1, "MOV M,L" },
    /*76*/ { 7,  0, 0, 1, "HLT" },
    /*77*/ { 7,  0, 2, 1, "MOV M,A" },
    /*78*/ { 5,  0, 1, 1, "MOV A,B" },
    /*79*/ { 5,  0, 1, 1, "MOV A,C" },
    /*7A*/ { 5,  0, 1, 1, "MOV A,D" },
    /*7B*/ { 5,  0, 1, 1, "MOV A,E" },
    /*7C*/ { 5,  0, 1, 1, "MOV A,H" },
    /*7D*/ { 5,  0, 1, 1, "MOV A,L" },
    /*7E*/ { 7,  0, 2, 1, "MOV A,M" },
    /*7F*/ { 5,  0, 1, 1, "MOV A,A" },
    /*80*/ { 4,  0, 1, 1, "ADD B" },
    /*81*/ { 4,  0, 1, 1, "ADD C" },
    /*82*/ { 4,  0, 1, 1, "ADD D" },
    /*83*/ { 4,  0, 1, 1, "ADD E" },
    /*84*/ { 4,  0, 1, 1, "ADD H" },
    /*85*/ { 4,  0, 1, 1, "ADD L" },
    /*86*/ { 7,  0, 2, 1, "ADD M" },
    /*87*/ { 4,  0, 1, 1, "ADD A" },
    /*88*/ { 4,  0, 1, 1, "ADC B" },
    /*89*/ { 4,  0, 1, 1, "ADC C" },
    /*8A*/ { 4,  0, 1, 1, "ADC D" },
    /*8B*/ { 4,  0, 1, 1, "ADC E" },
    /*8C*/ { 4,  0, 1, 1, "ADC H" },
    /*8D*/ { 4,  0, 1, 1, "ADC L" },
    /*8E*/ { 7,  0, 2, 1, "ADC M" },
    /*8F*/ { 4,  0, 1, 1, "ADC A" },
    /*90*/ { 4,  0, 1, 1, "SUB B" },
    /*91*/ { 4,  0, 1, 1, "SUB C" },
    /*92*/ { 4,  0, 1, 1, "SUB D" },
    /*93*/ { 4,  0, 1, 1, "SUB E" },
    /*94*/ { 4,  0, 1, 1, "SUB H" },
    /*95*/ { 4,  0, 1, 1, "SUB L" },
    /*96*/ { 7,  0, 2, 1, "SUB M" },
    /*97*/ { 4,  0, 1, 1, "SUB A" },
    /*98*/ { 4,  0, 1, 1, "SBB B" },
    /*99*/ { 4,  0, 1, 1, "SBB C" },
    /*9A*/ { 4,  0, 1, 1, "SBB D" },
    /*9B*/ { 4,  0, 1, 1, "SBB E" },
    /*9C*/ { 4,  0, 1, 1, "SBB H" },
    /*9D*/ { 4,  0, 1, 1, "SBB L" },
    /*9E*/ { 7,  0, 2, 1, "SBB M" },
    /*9F*/ { 4,  0, 1, 1, "SBB A" },
    /*A0*/ { 4,  0, 1, 1, "ANA B" },
    /*A1*/ { 4,  0, 1, 1, "ANA C" },
    /*A2*/ { 4,  0, 1, 1, "ANA D" },
    /*A3*/ { 4,  0, 1, 1, "ANA E" },
    /*A4*/ { 4,  0, 1, 1, "ANA H" },
    /*A5*/ { 4,  0, 1, 1, "ANA L" },
    /*A6*/ { 7,  0, 2, 1, "ANA M" },
    /*A7*/ { 4,  0, 1, 1, "ANA A" },
    /*A8*/ { 4,  0, 1, 1, "XRA B" },
    /*A9*/ { 4,  0, 1, 1, "XRA C" },
    /*AA*/ { 4,  0, 1, 1, "XRA D" },
    /*AB*/ { 4,  0, 1, 1, "XRA E" },
    /*AC*/ { 4,  0, 1, 1, "XRA H" },
    /*AD*/ { 4,  0, 1, 1, "XRA L" },
    /*AE*/ { 7,  0, 2, 1, "XRA M" },
    /*AF*/ { 4,  0, 1, 1, "XRA A" },
    /*B0*/ { 4,  0, 1, 1, "ORA B" },
    /*B1*/ { 4,  0, 1, 1, "ORA C" },
    /*B2*/ { 4,  0, 1, 1, "ORA D" },
    /*B3*/ { 4,  0, 1, 1, "ORA E" },
    /*B4*/ { 4,  0, 1, 1, "ORA H" },
    /*B5*/ { 4,  0, 1, 1, "ORA L" },
    /*B6*/ { 7,  0, 2, 1, "ORA M" },
    /*B7*/ { 4,  0, 1, 1, "ORA A" },
    /*B8*/ { 4,  0, 1, 1, "CMP B" },
    /*B9*/ { 4,  0, 1, 1, "CMP C" },
    /*BA*/ { 4,  0, 1, 1, "CMP D" },
    /*BB*/ { 4,  0, 1, 1, "CMP E" },
    /*BC*/ { 4,  0, 1, 1, "CMP H" },
    /*BD*/ { 4,  0, 1, 1, "CMP L" },
    /*BE*/ { 7,  0, 2, 1, "CMP M" },
    /*BF*/ { 4,  0, 1, 1, "CMP A" },
    /*C0*/ { 0,  0, 0, 0, "" },
    /*C1*/ { 0,  0, 0, 0, "" },
    /*C2*/ { 0,  0, 0, 0, "" },
    /*C3*/ { 0,  0, 0, 0, "" },
    /*C4*/ { 0,  0, 0, 0, "" },
    /*C5*/ { 0,  0, 0, 0, "" },
    /*C6*/ { 7,  0, 2, 2, "ADI " },
    /*C7*/ { 0,  0, 0, 0, "" },
    /*C8*/ { 0,  0, 0, 0, "" },
    /*C9*/ { 0,  0, 0, 0, "" },
    /*CA*/ { 0,  0, 0, 0, "" },
    /*CB*/ { 0,  0, 0, 0, "" },
    /*CC*/ { 0,  0, 0, 0, "" },
    /*CD*/ { 0,  0, 0, 0, "" },
    /*CE*/ { 7,  0, 2, 2, "ACI " },
    /*CF*/ { 0,  0, 0, 0, "" },
    /*D0*/ { 0,  0, 0, 0, "" },
    /*D1*/ { 0,  0, 0, 0, "" },
    /*D2*/ { 0,  0, 0, 0, "" },
    /*D3*/ { 0,  0, 0, 0, "" },
    /*D4*/ { 0,  0, 0, 0, "" },
    /*D5*/ { 0,  0, 0, 0, "" },
    /*D6*/ { 7,  0, 2, 2, "SUI " },
    /*D7*/ { 0,  0, 0, 0, "" },
    /*D8*/ { 0,  0, 0, 0, "" },
    /*D9*/ { 0,  0, 0, 0, "" },
    /*DA*/ { 0,  0, 0, 0, "" },
    /*DB*/ { 0,  0, 0, 0, "" },
    /*DC*/ { 0,  0, 0, 0, "" },
    /*DD*/ { 0,  0, 0, 0, "" },
    /*DE*/ { 7,  0, 2, 2, "SBI " },
    /*DF*/ { 0,  0, 0, 0, "" },
    /*E0*/ { 0,  0, 0, 0, "" },
    /*E1*/ { 0,  0, 0, 0, "" },
    /*E2*/ { 0,  0, 0, 0, "" },
    /*E3*/ { 0,  0, 0, 0, "" },
    /*E4*/ { 0,  0, 0, 0, "" },
    /*E5*/ { 0,  0, 0, 0, "" },
    /*E6*/ { 7,  0, 2, 2, "ANI " },
    /*E7*/ { 0,  0, 0, 0, "" },
    /*E8*/ { 0,  0, 0, 0, "" },
    /*E9*/ { 0,  0, 0, 0, "" },
    /*EA*/ { 0,  0, 0, 0, "" },
    /*EB*/ { 4,  0, 1, 1, "XCHG" },
    /*EC*/ { 0,  0, 0, 0, "" },
    /*ED*/ { 0,  0, 0, 0, "" },
    /*EE*/ { 7,  0, 2, 2, "XRI " },
    /*EF*/ { 0,  0, 0, 0, "" },
    /*F0*/ { 0,  0, 0, 0, "" },
    /*F1*/ { 0,  0, 0, 0, "" },
    /*F2*/ { 0,  0, 0, 0, "" },
    /*F3*/ { 4,  0, 0, 0, "DI" },
    /*F4*/ { 0,  0, 0, 0, "" },
    /*F5*/ { 0,  0, 0, 0, "" },
    /*F6*/ { 7,  0, 2, 2, "ORI " },
    /*F7*/ { 0,  0, 0, 0, "" },
    /*F8*/ { 0,  0, 0, 0, "" },
    /*F9*/ { 0,  0, 0, 0, "" },
    /*FA*/ { 0,  0, 0, 0, "" },
    /*FB*/ { 0,  0, 0, 0, "EI" },
    /*FC*/ { 0,  0, 0, 0, "" },
    /*FD*/ { 0,  0, 0, 0, "" },
    /*FE*/ { 7,  0, 2, 2, "CPI " },
    /*FF*/ { 0,  0, 0, 0, "" },
};

static const uint8_t flagsZSTable[256] =
{
    Flags8080::Zero, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign
};

static const uint8_t flagsPZSTable[256] =
{
    Flags8080::Zero | Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity,
    Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity
};

static const uint16_t correctDAATable[1024] =
{
    0x4400, 0x0001, 0x0002, 0x0403, 0x0004, 0x0405, 0x0406, 0x0007,
    0x0008, 0x0409, 0x1010, 0x1411, 0x1412, 0x1013, 0x1414, 0x1015,
    0x0010, 0x0411, 0x0412, 0x0013, 0x0414, 0x0015, 0x0016, 0x0417,
    0x0418, 0x0019, 0x1020, 0x1421, 0x1422, 0x1023, 0x1424, 0x1025,
    0x0020, 0x0421, 0x0422, 0x0023, 0x0424, 0x0025, 0x0026, 0x0427,
    0x0428, 0x0029, 0x1430, 0x1031, 0x1032, 0x1433, 0x1034, 0x1435,
    0x0430, 0x0031, 0x0032, 0x0433, 0x0034, 0x0435, 0x0436, 0x0037,
    0x0038, 0x0439, 0x1040, 0x1441, 0x1442, 0x1043, 0x1444, 0x1045,
    0x0040, 0x0441, 0x0442, 0x0043, 0x0444, 0x0045, 0x0046, 0x0447,
    0x0448, 0x0049, 0x1450, 0x1051, 0x1052, 0x1453, 0x1054, 0x1455,
    0x0450, 0x0051, 0x0052, 0x0453, 0x0054, 0x0455, 0x0456, 0x0057,
    0x0058, 0x0459, 0x1460, 0x1061, 0x1062, 0x1463, 0x1064, 0x1465,
    0x0460, 0x0061, 0x0062, 0x0463, 0x0064, 0x0465, 0x0466, 0x0067,
    0x0068, 0x0469, 0x1070, 0x1471, 0x1472, 0x1073, 0x1474, 0x1075,
    0x0070, 0x0471, 0x0472, 0x0073, 0x0474, 0x0075, 0x0076, 0x0477,
    0x0478, 0x0079, 0x9080, 0x9481, 0x9482, 0x9083, 0x9484, 0x9085,
    0x8080, 0x8481, 0x8482, 0x8083, 0x8484, 0x8085, 0x8086, 0x8487,
    0x8488, 0x8089, 0x9490, 0x9091, 0x9092, 0x9493, 0x9094, 0x9495,
    0x8490, 0x8091, 0x8092, 0x8493, 0x8094, 0x8495, 0x8496, 0x8097,
    0x8098, 0x8499, 0x5500, 0x1101, 0x1102, 0x1503, 0x1104, 0x1505,
    0x4500, 0x0101, 0x0102, 0x0503, 0x0104, 0x0505, 0x0506, 0x0107,
    0x0108, 0x0509, 0x1110, 0x1511, 0x1512, 0x1113, 0x1514, 0x1115,
    0x0110, 0x0511, 0x0512, 0x0113, 0x0514, 0x0115, 0x0116, 0x0517,
    0x0518, 0x0119, 0x1120, 0x1521, 0x1522, 0x1123, 0x1524, 0x1125,
    0x0120, 0x0521, 0x0522, 0x0123, 0x0524, 0x0125, 0x0126, 0x0527,
    0x0528, 0x0129, 0x1530, 0x1131, 0x1132, 0x1533, 0x1134, 0x1535,
    0x0530, 0x0131, 0x0132, 0x0533, 0x0134, 0x0535, 0x0536, 0x0137,
    0x0138, 0x0539, 0x1140, 0x1541, 0x1542, 0x1143, 0x1544, 0x1145,
    0x0140, 0x0541, 0x0542, 0x0143, 0x0544, 0x0145, 0x0146, 0x0547,
    0x0548, 0x0149, 0x1550, 0x1151, 0x1152, 0x1553, 0x1154, 0x1555,
    0x0550, 0x0151, 0x0152, 0x0553, 0x0154, 0x0555, 0x0556, 0x0157,
    0x0158, 0x0559, 0x5400, 0x1001, 0x1002, 0x1403, 0x1004, 0x1405,
    0x0560, 0x0161, 0x0162, 0x0563, 0x0164, 0x0565, 0x0566, 0x0167,
    0x0168, 0x0569, 0x1170, 0x1571, 0x1572, 0x1173, 0x1574, 0x1175,
    0x0170, 0x0571, 0x0572, 0x0173, 0x0574, 0x0175, 0x0176, 0x0577,
    0x0578, 0x0179, 0x9180, 0x9581, 0x9582, 0x9183, 0x9584, 0x9185,
    0x8180, 0x8581, 0x8582, 0x8183, 0x8584, 0x8185, 0x8186, 0x8587,
    0x8588, 0x8189, 0x9590, 0x9191, 0x9192, 0x9593, 0x9194, 0x9595,
    0x8590, 0x8191, 0x8192, 0x8593, 0x8194, 0x8595, 0x8596, 0x8197,
    0x8198, 0x8599, 0x95a0, 0x91a1, 0x91a2, 0x95a3, 0x91a4, 0x95a5,
    0x85a0, 0x81a1, 0x81a2, 0x85a3, 0x81a4, 0x85a5, 0x85a6, 0x81a7,
    0x81a8, 0x85a9, 0x91b0, 0x95b1, 0x95b2, 0x91b3, 0x95b4, 0x91b5,
    0x81b0, 0x85b1, 0x85b2, 0x81b3, 0x85b4, 0x81b5, 0x81b6, 0x85b7,
    0x85b8, 0x81b9, 0x95c0, 0x91c1, 0x91c2, 0x95c3, 0x91c4, 0x95c5,
    0x85c0, 0x81c1, 0x81c2, 0x85c3, 0x81c4, 0x85c5, 0x85c6, 0x81c7,
    0x81c8, 0x85c9, 0x91d0, 0x95d1, 0x95d2, 0x91d3, 0x95d4, 0x91d5,
    0x81d0, 0x85d1, 0x85d2, 0x81d3, 0x85d4, 0x81d5, 0x81d6, 0x85d7,
    0x85d8, 0x81d9, 0x91e0, 0x95e1, 0x95e2, 0x91e3, 0x95e4, 0x91e5,
    0x81e0, 0x85e1, 0x85e2, 0x81e3, 0x85e4, 0x81e5, 0x81e6, 0x85e7,
    0x85e8, 0x81e9, 0x95f0, 0x91f1, 0x91f2, 0x95f3, 0x91f4, 0x95f5,
    0x85f0, 0x81f1, 0x81f2, 0x85f3, 0x81f4, 0x85f5, 0x85f6, 0x81f7,
    0x81f8, 0x85f9, 0x5500, 0x1101, 0x1102, 0x1503, 0x1104, 0x1505,
    0x4500, 0x0101, 0x0102, 0x0503, 0x0104, 0x0505, 0x0506, 0x0107,
    0x0108, 0x0509, 0x1110, 0x1511, 0x1512, 0x1113, 0x1514, 0x1115,
    0x0110, 0x0511, 0x0512, 0x0113, 0x0514, 0x0115, 0x0116, 0x0517,
    0x0518, 0x0119, 0x1120, 0x1521, 0x1522, 0x1123, 0x1524, 0x1125,
    0x0120, 0x0521, 0x0522, 0x0123, 0x0524, 0x0125, 0x0126, 0x0527,
    0x0528, 0x0129, 0x1530, 0x1131, 0x1132, 0x1533, 0x1134, 0x1535,
    0x0530, 0x0131, 0x0132, 0x0533, 0x0134, 0x0535, 0x0536, 0x0137,
    0x0138, 0x0539, 0x1140, 0x1541, 0x1542, 0x1143, 0x1544, 0x1145,
    0x0140, 0x0541, 0x0542, 0x0143, 0x0544, 0x0145, 0x0146, 0x0547,
    0x0548, 0x0149, 0x1550, 0x1151, 0x1152, 0x1553, 0x1154, 0x1555,
    0x0550, 0x0151, 0x0152, 0x0553, 0x0154, 0x0555, 0x0556, 0x0157,
    0x0158, 0x0559, 0x1560, 0x1161, 0x1162, 0x1563, 0x1164, 0x1565,
    0x1406, 0x1007, 0x1008, 0x1409, 0x140a, 0x100b, 0x140c, 0x100d,
    0x100e, 0x140f, 0x1010, 0x1411, 0x1412, 0x1013, 0x1414, 0x1015,
    0x1016, 0x1417, 0x1418, 0x1019, 0x101a, 0x141b, 0x101c, 0x141d,
    0x141e, 0x101f, 0x1020, 0x1421, 0x1422, 0x1023, 0x1424, 0x1025,
    0x1026, 0x1427, 0x1428, 0x1029, 0x102a, 0x142b, 0x102c, 0x142d,
    0x142e, 0x102f, 0x1430, 0x1031, 0x1032, 0x1433, 0x1034, 0x1435,
    0x1436, 0x1037, 0x1038, 0x1439, 0x143a, 0x103b, 0x143c, 0x103d,
    0x103e, 0x143f, 0x1040, 0x1441, 0x1442, 0x1043, 0x1444, 0x1045,
    0x1046, 0x1447, 0x1448, 0x1049, 0x104a, 0x144b, 0x104c, 0x144d,
    0x144e, 0x104f, 0x1450, 0x1051, 0x1052, 0x1453, 0x1054, 0x1455,
    0x1456, 0x1057, 0x1058, 0x1459, 0x145a, 0x105b, 0x145c, 0x105d,
    0x105e, 0x145f, 0x1460, 0x1061, 0x1062, 0x1463, 0x1064, 0x1465,
    0x1466, 0x1067, 0x1068, 0x1469, 0x146a, 0x106b, 0x146c, 0x106d,
    0x106e, 0x146f, 0x1070, 0x1471, 0x1472, 0x1073, 0x1474, 0x1075,
    0x1076, 0x1477, 0x1478, 0x1079, 0x107a, 0x147b, 0x107c, 0x147d,
    0x147e, 0x107f, 0x9080, 0x9481, 0x9482, 0x9083, 0x9484, 0x9085,
    0x9086, 0x9487, 0x9488, 0x9089, 0x908a, 0x948b, 0x908c, 0x948d,
    0x948e, 0x908f, 0x9490, 0x9091, 0x9092, 0x9493, 0x9094, 0x9495,
    0x9496, 0x9097, 0x9098, 0x9499, 0x949a, 0x909b, 0x949c, 0x909d,
    0x909e, 0x949f, 0x5500, 0x1101, 0x1102, 0x1503, 0x1104, 0x1505,
    0x1506, 0x1107, 0x1108, 0x1509, 0x150a, 0x110b, 0x150c, 0x110d,
    0x110e, 0x150f, 0x1110, 0x1511, 0x1512, 0x1113, 0x1514, 0x1115,
    0x1116, 0x1517, 0x1518, 0x1119, 0x111a, 0x151b, 0x111c, 0x151d,
    0x151e, 0x111f, 0x1120, 0x1521, 0x1522, 0x1123, 0x1524, 0x1125,
    0x1126, 0x1527, 0x1528, 0x1129, 0x112a, 0x152b, 0x112c, 0x152d,
    0x152e, 0x112f, 0x1530, 0x1131, 0x1132, 0x1533, 0x1134, 0x1535,
    0x1536, 0x1137, 0x1138, 0x1539, 0x153a, 0x113b, 0x153c, 0x113d,
    0x113e, 0x153f, 0x1140, 0x1541, 0x1542, 0x1143, 0x1544, 0x1145,
    0x1146, 0x1547, 0x1548, 0x1149, 0x114a, 0x154b, 0x114c, 0x154d,
    0x154e, 0x114f, 0x1550, 0x1151, 0x1152, 0x1553, 0x1154, 0x1555,
    0x1556, 0x1157, 0x1158, 0x1559, 0x155a, 0x115b, 0x155c, 0x115d,
    0x115e, 0x155f, 0x5400, 0x1001, 0x1002, 0x1403, 0x1004, 0x1405,
    0x1566, 0x1167, 0x1168, 0x1569, 0x156a, 0x116b, 0x156c, 0x116d,
    0x116e, 0x156f, 0x1170, 0x1571, 0x1572, 0x1173, 0x1574, 0x1175,
    0x1176, 0x1577, 0x1578, 0x1179, 0x117a, 0x157b, 0x117c, 0x157d,
    0x157e, 0x117f, 0x9180, 0x9581, 0x9582, 0x9183, 0x9584, 0x9185,
    0x9186, 0x9587, 0x9588, 0x9189, 0x918a, 0x958b, 0x918c, 0x958d,
    0x958e, 0x918f, 0x9590, 0x9191, 0x9192, 0x9593, 0x9194, 0x9595,
    0x9596, 0x9197, 0x9198, 0x9599, 0x959a, 0x919b, 0x959c, 0x919d,
    0x919e, 0x959f, 0x95a0, 0x91a1, 0x91a2, 0x95a3, 0x91a4, 0x95a5,
    0x95a6, 0x91a7, 0x91a8, 0x95a9, 0x95aa, 0x91ab, 0x95ac, 0x91ad,
    0x91ae, 0x95af, 0x91b0, 0x95b1, 0x95b2, 0x91b3, 0x95b4, 0x91b5,
    0x91b6, 0x95b7, 0x95b8, 0x91b9, 0x91ba, 0x95bb, 0x91bc, 0x95bd,
    0x95be, 0x91bf, 0x95c0, 0x91c1, 0x91c2, 0x95c3, 0x91c4, 0x95c5,
    0x95c6, 0x91c7, 0x91c8, 0x95c9, 0x95ca, 0x91cb, 0x95cc, 0x91cd,
    0x91ce, 0x95cf, 0x91d0, 0x95d1, 0x95d2, 0x91d3, 0x95d4, 0x91d5,
    0x91d6, 0x95d7, 0x95d8, 0x91d9, 0x91da, 0x95db, 0x91dc, 0x95dd,
    0x95de, 0x91df, 0x91e0, 0x95e1, 0x95e2, 0x91e3, 0x95e4, 0x91e5,
    0x91e6, 0x95e7, 0x95e8, 0x91e9, 0x91ea, 0x95eb, 0x91ec, 0x95ed,
    0x95ee, 0x91ef, 0x95f0, 0x91f1, 0x91f2, 0x95f3, 0x91f4, 0x95f5,
    0x95f6, 0x91f7, 0x91f8, 0x95f9, 0x95fa, 0x91fb, 0x95fc, 0x91fd,
    0x91fe, 0x95ff, 0x5500, 0x1101, 0x1102, 0x1503, 0x1104, 0x1505,
    0x1506, 0x1107, 0x1108, 0x1509, 0x150a, 0x110b, 0x150c, 0x110d,
    0x110e, 0x150f, 0x1110, 0x1511, 0x1512, 0x1113, 0x1514, 0x1115,
    0x1116, 0x1517, 0x1518, 0x1119, 0x111a, 0x151b, 0x111c, 0x151d,
    0x151e, 0x111f, 0x1120, 0x1521, 0x1522, 0x1123, 0x1524, 0x1125,
    0x1126, 0x1527, 0x1528, 0x1129, 0x112a, 0x152b, 0x112c, 0x152d,
    0x152e, 0x112f, 0x1530, 0x1131, 0x1132, 0x1533, 0x1134, 0x1535,
    0x1536, 0x1137, 0x1138, 0x1539, 0x153a, 0x113b, 0x153c, 0x113d,
    0x113e, 0x153f, 0x1140, 0x1541, 0x1542, 0x1143, 0x1544, 0x1145,
    0x1146, 0x1547, 0x1548, 0x1149, 0x114a, 0x154b, 0x114c, 0x154d,
    0x154e, 0x114f, 0x1550, 0x1151, 0x1152, 0x1553, 0x1154, 0x1555,
    0x1556, 0x1157, 0x1158, 0x1559, 0x155a, 0x115b, 0x155c, 0x115d,
    0x115e, 0x155f, 0x1560, 0x1161, 0x1162, 0x1563, 0x1164, 0x1565,
};

std::ostream & Simulate::operator << (std::ostream & stream, Opcodes8080 opcode)
{
    stream << instructionData8080[uint8_t(opcode)].instructionMnemonic;
    return stream;
}

Processor8080::Processor8080(double clockFreq)
    : Processor(clockFreq, 16, 8)
{

}

Processor8080::~Processor8080()
{

}

void Processor8080::Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager)
{
    memory = memoryManager;
    io = ioManager;
}

void Processor8080::Reset()
{
    registers.pc = InitialPC;
    inte.reset();
    hlda.reset();
}

void Processor8080::FetchInstruction()
{
    //#ifdef DEBUG
    // Turn tracing on when reached trap address
    if (registers.pc == registers.trap)
        registers.trace = 1;
    /* Call single-step debugger, exit if requested */
    if (registers.trace)
        if (debugCallback && !debugCallback(0, registers))
            return;
    //#endif

    uint8_t data = FetchInstructionByte();
    instruction = Opcodes8080(data);
    registers.instructionCycles = instructionData8080[data].cycleCount;
    registers.cycleCount -= registers.instructionCycles;
}

uint8_t Add(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg16LH result;
    result.W = a + r;
    flags = Flags8080(flagsZSTable[result.B.l] | 
                      (result.B.h & Flags8080::Carry) |
                      ((a ^ r ^ result.B.l) & Flags8080::AuxCarry));
    return result.B.l;
}

uint8_t AddC(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg16LH result;
    result.W = a + r + (flags & Flags8080::Carry);
    flags = Flags8080(flagsZSTable[result.B.l] |
                      (result.B.h & Flags8080::Carry) |
                      ((a ^ r ^ result.B.l) & Flags8080::AuxCarry));
    return result.B.l;
}

uint16_t AddW(uint16_t hl, uint16_t rp, Flags8080 & flags)
{
    Reg16 result = hl + rp;
    flags = Flags8080(flags & ~(Flags8080::Carry) |
                      (((long)hl + (long)rp) & 0x10000 ? Flags8080::Carry : 0));
    return result;
}

uint8_t Sub(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg16LH result;
    result.W = a - r;
    flags = Flags8080(flagsZSTable[result.B.l] |
        (-result.B.h & Flags8080::Carry) |
        ((a ^ r ^ result.B.l) & Flags8080::AuxCarry));
    return result.B.l;
}

uint8_t SubC(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg16LH result;
    result.W = a - r - (flags & Flags8080::Carry);
    flags = Flags8080(flagsZSTable[result.B.l] |
        (-result.B.h & Flags8080::Carry) |
        ((a ^ r ^ result.B.l) & Flags8080::AuxCarry));
    return result.B.l;
}

uint8_t Inc(uint8_t r, Flags8080 & flags)
{
    Reg8 result = r + 1;
    flags = Flags8080((flags & Flags8080::Carry) | flagsZSTable[result] |
        (result & 0x0F ? 0 : Flags8080::AuxCarry));
    return result;
}

uint8_t Dec(uint8_t r, Flags8080 & flags)
{
    Reg8 result = r - 1;
    flags = Flags8080((flags & Flags8080::Carry) | flagsZSTable[result] |
        (r & 0x0F ? 0 : Flags8080::AuxCarry));
    return result;
}

uint8_t DAA(uint8_t a, Flags8080 & flags)
{
    Reg16LH result{};
    result.B.l = a;
    if (flags & Flags8080::Carry)
        result.B.h |= 0x01;
    if (flags & Flags8080::AuxCarry)
        result.B.h |= 0x02;
    result.W = correctDAATable[result.W];
    flags = Flags8080(result.B.h);
    return result.B.l;
}

uint8_t And(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg8 result = a & r;
    flags = Flags8080(Flags8080::AuxCarry | flagsPZSTable[result]);
    return result;
}

uint8_t Or(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg8 result = a | r;
    flags = Flags8080(Flags8080::AuxCarry | flagsPZSTable[result]);
    return result;
}

uint8_t Xor(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg8 result = a ^ r;
    flags = Flags8080(Flags8080::AuxCarry | flagsPZSTable[result]);
    return result;
}

void Processor8080::ExecuteInstruction()
{
    switch (instruction)
    {
    case Opcodes8080::NOP:                                                                                          break;
    case Opcodes8080::LXI_B:    registers.bc.B.l = FetchInstructionByte();              
                                registers.bc.B.h = FetchInstructionByte();                                          break;
    case Opcodes8080::STAX_B:   memory->Store8(registers.bc.W, registers.a);                                        break;
    case Opcodes8080::INX_B:    registers.bc.W++;                                                                   break;
    case Opcodes8080::INR_B:    registers.bc.B.h = Inc(registers.bc.B.h, registers.flags);                          break;
    case Opcodes8080::DCR_B:    registers.bc.B.h = Dec(registers.bc.B.h, registers.flags);                          break;
    case Opcodes8080::DAD_B:    registers.hl.W = AddW(registers.hl.W, registers.bc.W, registers.flags);             break;
    case Opcodes8080::LDAX_B:   registers.a = memory->Fetch8(registers.bc.W);                                       break;
    case Opcodes8080::DCX_B:    registers.bc.W--;                                                                   break;
    case Opcodes8080::INR_C:    registers.bc.B.l = Inc(registers.bc.B.l, registers.flags);                          break;
    case Opcodes8080::DCR_C:    registers.bc.B.l = Dec(registers.bc.B.l, registers.flags);                          break;
    case Opcodes8080::LXI_D:    registers.de.B.l = FetchInstructionByte();
                                registers.de.B.h = FetchInstructionByte();                                          break;
    case Opcodes8080::STAX_D:   memory->Store8(registers.de.W, registers.a);                                        break;
    case Opcodes8080::INX_D:    registers.de.W++;                                                                   break;
    case Opcodes8080::INR_D:    registers.de.B.h = Inc(registers.de.B.h, registers.flags);                          break;
    case Opcodes8080::DCR_D:    registers.de.B.h = Dec(registers.de.B.h, registers.flags);                          break;
    case Opcodes8080::DAD_D:    registers.hl.W = AddW(registers.hl.W, registers.de.W, registers.flags);             break;
    case Opcodes8080::LDAX_D:   registers.a = memory->Fetch8(registers.de.W);                                       break;
    case Opcodes8080::DCX_D:    registers.de.W--;                                                                   break;
    case Opcodes8080::INR_E:    registers.de.B.l = Inc(registers.de.B.l, registers.flags);                          break;
    case Opcodes8080::DCR_E:    registers.de.B.l = Dec(registers.de.B.l, registers.flags);                          break;
    case Opcodes8080::LXI_H:    registers.hl.B.l = FetchInstructionByte();
                                registers.hl.B.h = FetchInstructionByte();                                          break;
    case Opcodes8080::SHLD:     registers.wz.B.l = FetchInstructionByte();
                                registers.wz.B.h = FetchInstructionByte();
                                memory->Store16(registers.wz.W, registers.hl.W);                                    break;
    case Opcodes8080::INX_H:    registers.hl.W++;                                                                   break;
    case Opcodes8080::INR_H:    registers.hl.B.h = Inc(registers.hl.B.h, registers.flags);                          break;
    case Opcodes8080::DCR_H:    registers.hl.B.h = Dec(registers.hl.B.h, registers.flags);                          break;
    case Opcodes8080::DAA:      registers.a = DAA(registers.a, registers.flags);                                    break;
    case Opcodes8080::DAD_H:    registers.hl.W = AddW(registers.hl.W, registers.hl.W, registers.flags);             break;
    case Opcodes8080::LHLD:     registers.wz.B.l = FetchInstructionByte();
                                registers.wz.B.h = FetchInstructionByte();
                                registers.hl.W = memory->Fetch16(registers.wz.W);                                   break;
    case Opcodes8080::DCX_H:    registers.hl.W--;                                                                   break;
    case Opcodes8080::INR_L:    registers.hl.B.l = Inc(registers.hl.B.l, registers.flags);                          break;
    case Opcodes8080::DCR_L:    registers.hl.B.l = Dec(registers.hl.B.l, registers.flags);                          break;
    case Opcodes8080::LXI_SP:   registers.sp.B.l = FetchInstructionByte();
                                registers.sp.B.h = FetchInstructionByte();                                          break;
    case Opcodes8080::STA:      registers.wz.B.l = FetchInstructionByte();
                                registers.wz.B.h = FetchInstructionByte();
                                memory->Store8(registers.wz.W, registers.a);                                        break;
    case Opcodes8080::INX_SP:   registers.sp.W++;                                                                   break;
    case Opcodes8080::INR_M:    memory->Store8(registers.hl.W, Inc(memory->Fetch8(registers.hl.W), registers.flags));break;
    case Opcodes8080::DCR_M:    memory->Store8(registers.hl.W, Dec(memory->Fetch8(registers.hl.W), registers.flags));break;
    case Opcodes8080::DAD_SP:   registers.hl.W = AddW(registers.hl.W, registers.sp.W, registers.flags);             break;
    case Opcodes8080::LDA:      registers.wz.B.l = FetchInstructionByte();
                                registers.wz.B.h = FetchInstructionByte();
                                registers.a = memory->Fetch8(registers.wz.W);                                       break;
    case Opcodes8080::DCX_SP:   registers.sp.W--;                                                                   break;
    case Opcodes8080::INR_A:    registers.a = Inc(registers.a, registers.flags);                                    break;
    case Opcodes8080::DCR_A:    registers.a = Dec(registers.a, registers.flags);                                    break;
    case Opcodes8080::MVI_M:    memory->Store8(registers.hl.W, FetchInstructionByte());                             break;
    case Opcodes8080::MOV_B_B:  registers.bc.B.h = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_B_C:  registers.bc.B.h = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_B_D:  registers.bc.B.h = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_B_E:  registers.bc.B.h = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_B_H:  registers.bc.B.h = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_B_L:  registers.bc.B.h = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_B_M:  registers.bc.B.h = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_B_A:  registers.bc.B.h = registers.a;                                                     break;
    case Opcodes8080::MOV_C_B:  registers.bc.B.l = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_C_C:  registers.bc.B.l = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_C_D:  registers.bc.B.l = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_C_E:  registers.bc.B.l = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_C_H:  registers.bc.B.l = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_C_L:  registers.bc.B.l = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_C_M:  registers.bc.B.l = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_C_A:  registers.bc.B.l = registers.a;                                                     break;
    case Opcodes8080::MOV_D_B:  registers.de.B.h = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_D_C:  registers.de.B.h = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_D_D:  registers.de.B.h = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_D_E:  registers.de.B.h = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_D_H:  registers.de.B.h = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_D_L:  registers.de.B.h = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_D_M:  registers.de.B.h = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_D_A:  registers.de.B.h = registers.a;                                                     break;
    case Opcodes8080::MOV_E_B:  registers.de.B.l = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_E_C:  registers.de.B.l = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_E_D:  registers.de.B.l = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_E_E:  registers.de.B.l = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_E_H:  registers.de.B.l = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_E_L:  registers.de.B.l = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_E_M:  registers.de.B.l = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_E_A:  registers.de.B.l = registers.a;                                                     break;
    case Opcodes8080::MOV_H_B:  registers.hl.B.h = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_H_C:  registers.hl.B.h = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_H_D:  registers.hl.B.h = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_H_E:  registers.hl.B.h = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_H_H:  registers.hl.B.h = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_H_L:  registers.hl.B.h = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_H_M:  registers.hl.B.h = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_H_A:  registers.hl.B.h = registers.a;                                                     break;
    case Opcodes8080::MOV_L_B:  registers.hl.B.l = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_L_C:  registers.hl.B.l = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_L_D:  registers.hl.B.l = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_L_E:  registers.hl.B.l = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_L_H:  registers.hl.B.l = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_L_L:  registers.hl.B.l = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_L_M:  registers.hl.B.l = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_L_A:  registers.hl.B.l = registers.a;                                                     break;
    case Opcodes8080::MOV_M_B:  memory->Store8(registers.hl.W, registers.bc.B.h);                                   break;
    case Opcodes8080::MOV_M_C:  memory->Store8(registers.hl.W, registers.bc.B.l);                                   break;
    case Opcodes8080::MOV_M_D:  memory->Store8(registers.hl.W, registers.de.B.h);                                   break;
    case Opcodes8080::MOV_M_E:  memory->Store8(registers.hl.W, registers.de.B.l);                                   break;
    case Opcodes8080::MOV_M_H:  memory->Store8(registers.hl.W, registers.hl.B.h);                                   break;
    case Opcodes8080::MOV_M_L:  memory->Store8(registers.hl.W, registers.hl.B.l);                                   break;
    case Opcodes8080::HLT:  halt = true;                                                                            break;
    case Opcodes8080::MOV_M_A:  memory->Store8(registers.hl.W, registers.a);                                        break;
    case Opcodes8080::MOV_A_B:  registers.a = registers.bc.B.h;                                                     break;
    case Opcodes8080::MOV_A_C:  registers.a = registers.bc.B.l;                                                     break;
    case Opcodes8080::MOV_A_D:  registers.a = registers.de.B.h;                                                     break;
    case Opcodes8080::MOV_A_E:  registers.a = registers.de.B.l;                                                     break;
    case Opcodes8080::MOV_A_H:  registers.a = registers.hl.B.h;                                                     break;
    case Opcodes8080::MOV_A_L:  registers.a = registers.hl.B.l;                                                     break;
    case Opcodes8080::MOV_A_M:  registers.a = memory->Fetch8(registers.hl.W);                                       break;
    case Opcodes8080::MOV_A_A:  registers.a = registers.a;                                                          break;
    case Opcodes8080::ADD_B:    registers.a = Add(registers.a, registers.bc.B.h, registers.flags);                  break;
    case Opcodes8080::ADD_C:    registers.a = Add(registers.a, registers.bc.B.l, registers.flags);                  break;
    case Opcodes8080::ADD_D:    registers.a = Add(registers.a, registers.de.B.h, registers.flags);                  break;
    case Opcodes8080::ADD_E:    registers.a = Add(registers.a, registers.de.B.l, registers.flags);                  break;
    case Opcodes8080::ADD_H:    registers.a = Add(registers.a, registers.hl.B.h, registers.flags);                  break;
    case Opcodes8080::ADD_L:    registers.a = Add(registers.a, registers.hl.B.l, registers.flags);                  break;
    case Opcodes8080::ADD_M:    registers.a = Add(registers.a, memory->Fetch8(registers.hl.W), registers.flags);    break;
    case Opcodes8080::ADD_A:    registers.a = Add(registers.a, registers.a, registers.flags);                       break;
    case Opcodes8080::ADC_B:    registers.a = AddC(registers.a, registers.bc.B.h, registers.flags);                 break;
    case Opcodes8080::ADC_C:    registers.a = AddC(registers.a, registers.bc.B.l, registers.flags);                 break;
    case Opcodes8080::ADC_D:    registers.a = AddC(registers.a, registers.de.B.h, registers.flags);                 break;
    case Opcodes8080::ADC_E:    registers.a = AddC(registers.a, registers.de.B.l, registers.flags);                 break;
    case Opcodes8080::ADC_H:    registers.a = AddC(registers.a, registers.hl.B.h, registers.flags);                 break;
    case Opcodes8080::ADC_L:    registers.a = AddC(registers.a, registers.hl.B.l, registers.flags);                 break;
    case Opcodes8080::ADC_M:    registers.a = AddC(registers.a, memory->Fetch8(registers.hl.W), registers.flags);   break;
    case Opcodes8080::ADC_A:    registers.a = AddC(registers.a, registers.a, registers.flags);                      break;
    case Opcodes8080::SUB_B:    registers.a = Sub(registers.a, registers.bc.B.h, registers.flags);                  break;
    case Opcodes8080::SUB_C:    registers.a = Sub(registers.a, registers.bc.B.l, registers.flags);                  break;
    case Opcodes8080::SUB_D:    registers.a = Sub(registers.a, registers.de.B.h, registers.flags);                  break;
    case Opcodes8080::SUB_E:    registers.a = Sub(registers.a, registers.de.B.l, registers.flags);                  break;
    case Opcodes8080::SUB_H:    registers.a = Sub(registers.a, registers.hl.B.h, registers.flags);                  break;
    case Opcodes8080::SUB_L:    registers.a = Sub(registers.a, registers.hl.B.l, registers.flags);                  break;
    case Opcodes8080::SUB_M:    registers.a = Sub(registers.a, memory->Fetch8(registers.hl.W), registers.flags);    break;
    case Opcodes8080::SUB_A:    registers.a = Sub(registers.a, registers.a, registers.flags);                       break;
    case Opcodes8080::SBB_B:    registers.a = SubC(registers.a, registers.bc.B.h, registers.flags);                 break;
    case Opcodes8080::SBB_C:    registers.a = SubC(registers.a, registers.bc.B.l, registers.flags);                 break;
    case Opcodes8080::SBB_D:    registers.a = SubC(registers.a, registers.de.B.h, registers.flags);                 break;
    case Opcodes8080::SBB_E:    registers.a = SubC(registers.a, registers.de.B.l, registers.flags);                 break;
    case Opcodes8080::SBB_H:    registers.a = SubC(registers.a, registers.hl.B.h, registers.flags);                 break;
    case Opcodes8080::SBB_L:    registers.a = SubC(registers.a, registers.hl.B.l, registers.flags);                 break;
    case Opcodes8080::SBB_M:    registers.a = SubC(registers.a, memory->Fetch8(registers.hl.W), registers.flags);   break;
    case Opcodes8080::SBB_A:    registers.a = SubC(registers.a, registers.a, registers.flags);                      break;
    case Opcodes8080::ANA_B:    registers.a = And(registers.a, registers.bc.B.h, registers.flags);                  break;
    case Opcodes8080::ANA_C:    registers.a = And(registers.a, registers.bc.B.l, registers.flags);                  break;
    case Opcodes8080::ANA_D:    registers.a = And(registers.a, registers.de.B.h, registers.flags);                  break;
    case Opcodes8080::ANA_E:    registers.a = And(registers.a, registers.de.B.l, registers.flags);                  break;
    case Opcodes8080::ANA_H:    registers.a = And(registers.a, registers.hl.B.h, registers.flags);                  break;
    case Opcodes8080::ANA_L:    registers.a = And(registers.a, registers.hl.B.l, registers.flags);                  break;
    case Opcodes8080::ANA_M:    registers.a = And(registers.a, memory->Fetch8(registers.hl.W), registers.flags);    break;
    case Opcodes8080::ANA_A:    registers.a = And(registers.a, registers.a, registers.flags);                       break;
    case Opcodes8080::XRA_B:    registers.a = Xor(registers.a, registers.bc.B.h, registers.flags);                  break;
    case Opcodes8080::XRA_C:    registers.a = Xor(registers.a, registers.bc.B.l, registers.flags);                  break;
    case Opcodes8080::XRA_D:    registers.a = Xor(registers.a, registers.de.B.h, registers.flags);                  break;
    case Opcodes8080::XRA_E:    registers.a = Xor(registers.a, registers.de.B.l, registers.flags);                  break;
    case Opcodes8080::XRA_H:    registers.a = Xor(registers.a, registers.hl.B.h, registers.flags);                  break;
    case Opcodes8080::XRA_L:    registers.a = Xor(registers.a, registers.hl.B.l, registers.flags);                  break;
    case Opcodes8080::XRA_M:    registers.a = Xor(registers.a, memory->Fetch8(registers.hl.W), registers.flags);    break;
    case Opcodes8080::XRA_A:    registers.a = Xor(registers.a, registers.a, registers.flags);                       break;
    case Opcodes8080::ORA_B:    registers.a = Or(registers.a, registers.bc.B.h, registers.flags);                   break;
    case Opcodes8080::ORA_C:    registers.a = Or(registers.a, registers.bc.B.l, registers.flags);                   break;
    case Opcodes8080::ORA_D:    registers.a = Or(registers.a, registers.de.B.h, registers.flags);                   break;
    case Opcodes8080::ORA_E:    registers.a = Or(registers.a, registers.de.B.l, registers.flags);                   break;
    case Opcodes8080::ORA_H:    registers.a = Or(registers.a, registers.hl.B.h, registers.flags);                   break;
    case Opcodes8080::ORA_L:    registers.a = Or(registers.a, registers.hl.B.l, registers.flags);                   break;
    case Opcodes8080::ORA_M:    registers.a = Or(registers.a, memory->Fetch8(registers.hl.W), registers.flags);     break;
    case Opcodes8080::ORA_A:    registers.a = Or(registers.a, registers.a, registers.flags);                        break;
    case Opcodes8080::ADI:      registers.a = Add(registers.a, FetchInstructionByte(), registers.flags);            break;
    case Opcodes8080::ACI:      registers.a = AddC(registers.a, FetchInstructionByte(), registers.flags);           break;
    case Opcodes8080::SUI:      registers.a = Sub(registers.a, FetchInstructionByte(), registers.flags);            break;
    case Opcodes8080::SBI:      registers.a = SubC(registers.a, FetchInstructionByte(), registers.flags);           break;
    case Opcodes8080::ANI:      registers.a = And(registers.a, FetchInstructionByte(), registers.flags);            break;
    case Opcodes8080::XCHG:     std::swap(registers.de.W, registers.hl.W);                                          break;
    case Opcodes8080::XRI:      registers.a = Xor(registers.a, FetchInstructionByte(), registers.flags);            break;
    case Opcodes8080::EI:       inte = true;                                                                        break;
    case Opcodes8080::ORI:      registers.a = Or(registers.a, FetchInstructionByte(), registers.flags);             break;
    case Opcodes8080::DI:       inte = false;                                                                       break;

    default:
        std::cout << "Unrecognized instruction: " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << instruction << " at PC=" << std::setw(2) << registers.pc - 2 << std::endl;
    }

}

bool Processor8080::RunInstruction()
{
    register InterruptFlags8080 interruptFlag;

    FetchInstruction();
    ExecuteInstruction();
    // If cycle counter expired...
    if (registers.cycleCount <= 0)
    {
        if (InterruptPending())
        {
            ResetInterrupt();
            registers.cycleCount += registers.cycleCountBackup - 1;     // Restore the interrupt count

                                                                        // Call periodic handler or set pending IRQ
            if (registers.cycleCount > 0)
                interruptFlag = registers.interruptRequest;
            else
            {
                interruptFlag = Loop();                               // Call periodic handler
                registers.cycleCount += registers.cycleCountPeriod;     // Reset the cycle counter
                if (interruptFlag == InterruptFlags8080::None)
                    interruptFlag = registers.interruptRequest;       // Pending IRQ
            }
        }
        else
        {
            interruptFlag = Loop();                                   // Call periodic handler
            registers.cycleCount += registers.cycleCountPeriod;         // Reset the cycle counter
            if (interruptFlag == InterruptFlags8080::None)
                interruptFlag = registers.interruptRequest;           // Pending IRQ
        }

        if (interruptFlag == InterruptFlags8080::Quit)
            return false; /* Exit if INT_QUIT */
        if (interruptFlag != InterruptFlags8080::None)
            HandleInterrupt(interruptFlag);   /* Int-pt if needed */
    }
    return true;
}

void Processor8080::Run()
{
    while (RunInstruction())
    {
    }
}

InstructionData8080 Processor8080::GetInstructionData()
{
    return GetInstructionData(instruction);
}

InstructionData8080 Processor8080::GetInstructionData(Opcodes8080 opcode)
{
    return instructionData8080[int(opcode)];
}

size_t Processor8080::DisassembleInstruction(std::vector<uint8_t> const & machineCode, std::string & mnemonic)
{
    std::ostringstream stream;
    Opcodes8080 opcode = Opcodes8080(machineCode[0]);
    stream << instructionData8080[machineCode[0]].instructionMnemonic;
    size_t instructionSize = instructionData8080[machineCode[0]].instructionSize;
    switch (opcode)
    {
    case Opcodes8080::SHLD:
    case Opcodes8080::LHLD:
    case Opcodes8080::STA:
    case Opcodes8080::LDA:      
        stream << "(" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(machineCode[2]) 
               << std::setw(2) << std::setfill('0') << int(machineCode[1]) << ")";
        break;

    case Opcodes8080::LXI_B:
    case Opcodes8080::LXI_D:
    case Opcodes8080::LXI_H:
    case Opcodes8080::LXI_SP:
        stream << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(machineCode[2])
            << std::setw(2) << std::setfill('0') << int(machineCode[1]);
        break;

    case Opcodes8080::MVI_M:
    case Opcodes8080::ADI:
    case Opcodes8080::ACI:
    case Opcodes8080::SUI:
    case Opcodes8080::SBI:
    case Opcodes8080::ANI:
    case Opcodes8080::XRI:
    case Opcodes8080::ORI:
    case Opcodes8080::CPI:
        stream << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(machineCode[1]);
        break;

    case Opcodes8080::NOP:
    case Opcodes8080::STAX_B:
    case Opcodes8080::INX_B:
    case Opcodes8080::INR_B:
    case Opcodes8080::DCR_B:
    case Opcodes8080::RLC:
    case Opcodes8080::DAD_B:
    case Opcodes8080::LDAX_B:
    case Opcodes8080::DCX_B:
    case Opcodes8080::INR_C:
    case Opcodes8080::DCR_C:
    case Opcodes8080::RRC:
    case Opcodes8080::STAX_D:
    case Opcodes8080::INX_D:
    case Opcodes8080::INR_D:
    case Opcodes8080::DCR_D:
    case Opcodes8080::RAL:
    case Opcodes8080::DAD_D:
    case Opcodes8080::LDAX_D:
    case Opcodes8080::DCX_D:
    case Opcodes8080::INR_E:
    case Opcodes8080::DCR_E:
    case Opcodes8080::RAR:
    case Opcodes8080::INX_H:
    case Opcodes8080::INR_H:
    case Opcodes8080::DCR_H:
    case Opcodes8080::DAA:
    case Opcodes8080::DAD_H:
    case Opcodes8080::DCX_H:
    case Opcodes8080::INR_L:
    case Opcodes8080::DCR_L:
    case Opcodes8080::CMA:
    case Opcodes8080::INX_SP:
    case Opcodes8080::INR_M:
    case Opcodes8080::DCR_M:
    case Opcodes8080::STC:
    case Opcodes8080::DAD_SP:
    case Opcodes8080::DCX_SP:
    case Opcodes8080::INR_A:
    case Opcodes8080::DCR_A:
    case Opcodes8080::CMC:
    case Opcodes8080::MOV_B_B:
    case Opcodes8080::MOV_B_C:
    case Opcodes8080::MOV_B_D:
    case Opcodes8080::MOV_B_E:
    case Opcodes8080::MOV_B_H:
    case Opcodes8080::MOV_B_L:
    case Opcodes8080::MOV_B_M:
    case Opcodes8080::MOV_B_A:
    case Opcodes8080::MOV_C_B:
    case Opcodes8080::MOV_C_C:
    case Opcodes8080::MOV_C_D:
    case Opcodes8080::MOV_C_E:
    case Opcodes8080::MOV_C_H:
    case Opcodes8080::MOV_C_L:
    case Opcodes8080::MOV_C_M:
    case Opcodes8080::MOV_C_A:
    case Opcodes8080::MOV_D_B:
    case Opcodes8080::MOV_D_C:
    case Opcodes8080::MOV_D_D:
    case Opcodes8080::MOV_D_E:
    case Opcodes8080::MOV_D_H:
    case Opcodes8080::MOV_D_L:
    case Opcodes8080::MOV_D_M:
    case Opcodes8080::MOV_D_A:
    case Opcodes8080::MOV_E_B:
    case Opcodes8080::MOV_E_C:
    case Opcodes8080::MOV_E_D:
    case Opcodes8080::MOV_E_E:
    case Opcodes8080::MOV_E_H:
    case Opcodes8080::MOV_E_L:
    case Opcodes8080::MOV_E_M:
    case Opcodes8080::MOV_E_A:
    case Opcodes8080::MOV_H_B:
    case Opcodes8080::MOV_H_C:
    case Opcodes8080::MOV_H_D:
    case Opcodes8080::MOV_H_E:
    case Opcodes8080::MOV_H_H:
    case Opcodes8080::MOV_H_L:
    case Opcodes8080::MOV_H_M:
    case Opcodes8080::MOV_H_A:
    case Opcodes8080::MOV_L_B:
    case Opcodes8080::MOV_L_C:
    case Opcodes8080::MOV_L_D:
    case Opcodes8080::MOV_L_E:
    case Opcodes8080::MOV_L_H:
    case Opcodes8080::MOV_L_L:
    case Opcodes8080::MOV_L_M:
    case Opcodes8080::MOV_L_A:
    case Opcodes8080::MOV_M_B:
    case Opcodes8080::MOV_M_C:
    case Opcodes8080::MOV_M_D:
    case Opcodes8080::MOV_M_E:
    case Opcodes8080::MOV_M_H:
    case Opcodes8080::MOV_M_L:
    case Opcodes8080::HLT:
    case Opcodes8080::MOV_M_A:
    case Opcodes8080::MOV_A_B:
    case Opcodes8080::MOV_A_C:
    case Opcodes8080::MOV_A_D:
    case Opcodes8080::MOV_A_E:
    case Opcodes8080::MOV_A_H:
    case Opcodes8080::MOV_A_L:
    case Opcodes8080::MOV_A_M:
    case Opcodes8080::MOV_A_A:
    case Opcodes8080::ADD_B:
    case Opcodes8080::ADD_C:
    case Opcodes8080::ADD_D:
    case Opcodes8080::ADD_E:
    case Opcodes8080::ADD_H:
    case Opcodes8080::ADD_L:
    case Opcodes8080::ADD_M:
    case Opcodes8080::ADD_A:
    case Opcodes8080::ADC_B:
    case Opcodes8080::ADC_C:
    case Opcodes8080::ADC_D:
    case Opcodes8080::ADC_E:
    case Opcodes8080::ADC_H:
    case Opcodes8080::ADC_L:
    case Opcodes8080::ADC_M:
    case Opcodes8080::ADC_A:
    case Opcodes8080::SUB_B:
    case Opcodes8080::SUB_C:
    case Opcodes8080::SUB_D:
    case Opcodes8080::SUB_E:
    case Opcodes8080::SUB_H:
    case Opcodes8080::SUB_L:
    case Opcodes8080::SUB_M:
    case Opcodes8080::SUB_A:
    case Opcodes8080::SBB_B:
    case Opcodes8080::SBB_C:
    case Opcodes8080::SBB_D:
    case Opcodes8080::SBB_E:
    case Opcodes8080::SBB_H:
    case Opcodes8080::SBB_L:
    case Opcodes8080::SBB_M:
    case Opcodes8080::SBB_A:
    case Opcodes8080::ANA_B:
    case Opcodes8080::ANA_C:
    case Opcodes8080::ANA_D:
    case Opcodes8080::ANA_E:
    case Opcodes8080::ANA_H:
    case Opcodes8080::ANA_L:
    case Opcodes8080::ANA_M:
    case Opcodes8080::ANA_A:
    case Opcodes8080::XRA_B:
    case Opcodes8080::XRA_C:
    case Opcodes8080::XRA_D:
    case Opcodes8080::XRA_E:
    case Opcodes8080::XRA_H:
    case Opcodes8080::XRA_L:
    case Opcodes8080::XRA_M:
    case Opcodes8080::XRA_A:
    case Opcodes8080::ORA_B:
    case Opcodes8080::ORA_C:
    case Opcodes8080::ORA_D:
    case Opcodes8080::ORA_E:
    case Opcodes8080::ORA_H:
    case Opcodes8080::ORA_L:
    case Opcodes8080::ORA_M:
    case Opcodes8080::ORA_A:
    case Opcodes8080::CMP_B:
    case Opcodes8080::CMP_C:
    case Opcodes8080::CMP_D:
    case Opcodes8080::CMP_E:
    case Opcodes8080::CMP_H:
    case Opcodes8080::CMP_L:
    case Opcodes8080::CMP_M:
    case Opcodes8080::CMP_A:
    case Opcodes8080::XCHG:
    case Opcodes8080::EI:
    case Opcodes8080::DI:
        break;

    default:
        std::cout << "Unrecognized instruction: " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(machineCode[0]);
        break;
    }
    mnemonic = stream.str();
    return instructionSize;
}

size_t Processor8080::AssembleInstruction(std::string const & mnemonic, std::vector<uint8_t> & machineCode)
{
    return 0;
}

void Processor8080::Disassemble(std::vector<uint8_t> const & machineCode, std::ostream & disassembledCode)
{
    
}

void Processor8080::Assemble(std::string const & disassembledCode, std::vector<uint8_t> & machineCode)
{

}

void Processor8080::PrintRegisterValues(std::ostream & stream)
{

}

std::vector<std::string> Processor8080::GetRegisterNames()
{
    std::vector<std::string> names;
    names.push_back("B");
    names.push_back("C");
    names.push_back("D");
    names.push_back("E");
    names.push_back("H");
    names.push_back("L");
    names.push_back("A");
    names.push_back("F");
    names.push_back("SP");
    names.push_back("PC");
    return names;
}

bool Processor8080::InterruptPending()
{
    return false;
}

void Processor8080::ResetInterrupt()
{

}

void Processor8080::HandleInterrupt(InterruptFlags8080 flags)
{
}

InterruptFlags8080 Processor8080::Loop()
{
    return InterruptFlags8080::None;
}

uint8_t Processor8080::FetchInstructionByte()
{
    return memory->Fetch8(registers.pc++);
}

