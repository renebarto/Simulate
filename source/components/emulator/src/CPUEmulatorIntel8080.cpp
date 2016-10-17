#include "emulator/CPUEmulatorIntel8080.h"
#include <iomanip>
#include "core/Util.h"
#include "emulator/RAM.h"

using namespace std;
using namespace Emulator;

std::wostream & Emulator::operator << (std::wostream & stream, FlagsIntel8080 value)
{
    stream << ((value & FlagsIntel8080::Carry) ? L"C" : L".");
    stream << ((value & FlagsIntel8080::Overflow) ? L"V" : L".");
    stream << ((value & FlagsIntel8080::Parity) ? L"P" : L".");
    stream << L".";
    stream << ((value & FlagsIntel8080::AuxCarry) ? L"H" : L".");
    stream << ((value & FlagsIntel8080::KFlag) ? L"K" : L".");
    stream << ((value & FlagsIntel8080::Zero) ? L"Z" : L".");
    stream << ((value & FlagsIntel8080::Sign) ? L"S" : L".");
    return stream;
}

CPUEmulatorIntel8080::CPUEmulatorIntel8080(ObjectFile::ObjectCode const & objectCode, Assembler::PrettyPrinter<wchar_t> & printer)
    : objectCode(objectCode)
    , printer(printer)
    , processor()
{
}

CPUEmulatorIntel8080::~CPUEmulatorIntel8080()
{
}

static const size_t ColDec = 7;
static const size_t ColHex = 13;
static const size_t ColOct = 20;
static const size_t ColBin = 29;

void CPUEmulatorIntel8080::PrintRegister8(std::wstring const & name, Reg8 const & reg)
{
    printer << name << column(ColDec) << Core::Util::ToWString(reg) 
                    << column(ColHex) << Core::Util::ToWString(reg, 16) << L"h"
                    << column(ColOct) << Core::Util::ToWString(reg, 8) << L"o" 
                    << column(ColBin) << Core::Util::ToWString(reg, 2) << L"b" << endl;
}

void CPUEmulatorIntel8080::PrintRegister16(std::wstring const & name, Reg16 const & reg)
{
    printer << name << column(ColDec) << Core::Util::ToWString(reg) 
                    << column(ColHex) << Core::Util::ToWString(reg, 16) << L"h"
                    << column(ColOct) << Core::Util::ToWString(reg, 8) << L"o"
                    << column(ColBin) << Core::Util::ToWString(reg, 2) << L"b" << endl;
}

void CPUEmulatorIntel8080::PrintRegister16(std::wstring const & name, Reg16LH const & reg)
{
    printer << name << column(ColDec) << Core::Util::ToWString(reg.W) 
                    << column(ColHex) << Core::Util::ToWString(reg.W, 16) << L"h"
                    << column(ColOct) << Core::Util::ToWString(reg.W, 8) << L"o"
                    << column(ColBin) << Core::Util::ToWString(reg.W, 2) << L"b" << endl;
}

void CPUEmulatorIntel8080::PrintRegisterValues(RegistersIntel8080 const & registers)
{
    printer << L"Cycle timer: " << registers.cycleCount << endl;
    printer << L"Register values:" << endl;
    printer << L"Flags" << column(ColBin) << registers.flags << endl;
    PrintRegister8(L"A", registers.a);
    PrintRegister16(L"BC", registers.bc);
    PrintRegister16(L"DE", registers.de);
    PrintRegister16(L"HL", registers.hl);
    PrintRegister16(L"SP", registers.sp);
    PrintRegister16(L"PC", registers.pc);
}

bool CPUEmulatorIntel8080::OnCallback(RegistersIntel8080 const & registers)
{
    PrintRegisterValues(registers);
    return true;
}

bool CPUEmulatorIntel8080::Run(Options options)
{
    RegistersIntel8080 & registers = processor.GetRegisters();
    processor.Reset();
    MemoryManagerPtr memoryManager = std::make_shared<MemoryManager>();
    ROMPtr rom = std::make_shared<ROM>(objectCode.GetSegment(ObjectFile::SegmentID::ASEG).Offset(), 
                                       objectCode.GetSegment(ObjectFile::SegmentID::ASEG).Size());
    size_t offset = objectCode.GetSegment(ObjectFile::SegmentID::ASEG).Offset() + objectCode.GetSegment(ObjectFile::SegmentID::ASEG).Size();
    RAMPtr ram = std::make_shared<RAM>(offset, 65536 - offset);
    memoryManager->AddMemory(rom);
    memoryManager->AddMemory(ram);
    IOManagerPtr ioManager = std::make_shared<IOManager>();
    IOPortPtr ioPort = std::make_shared<IOPort>(0, 256);
    ioManager->AddIO(ioPort);
    processor.Setup(memoryManager, ioManager);
    processor.LoadCode(objectCode.GetSegment(ObjectFile::SegmentID::ASEG).Data(), 
                       objectCode.GetSegment(ObjectFile::SegmentID::ASEG).Offset(),
                       rom);
    if ((options & Options::ShowInstructionResults) != Options::None)
    {
        processor.SetupDebug(std::bind(&CPUEmulatorIntel8080::OnCallback, this, placeholders::_1));
        registers.trace = true;
    }
    bool done = false;
    while (!done)
    {
        done = !processor.RunInstruction();
    }
    if (((options & Options::ShowFinalResults) != Options::None) || ((options & Options::ShowInstructionResults) != Options::None))
        PrintRegisterValues(registers);
    return true;
}
