#include "ASM-8080.h"
#include "assembler/Parser.h"
#include "object-file/ObjectFile.h"
#include "emulator/Emulator.h"

using namespace std;
using namespace ASM;
using namespace Assembler;
using namespace Emulator;

ASM_8080::ASM_8080(CommandLineOptionsParser const & options)
    : options(options)
{
}


ASM_8080::~ASM_8080()
{
}

bool ASM_8080::Run()
{
    Assembler::AssemblerMessages messages;
    std::ifstream inputStream(options.inputFilePath);
    std::ofstream outputObjectStream(options.outputObjectFilePath, std::ios::binary);
    ObjectFile::ObjectFile outputObjectFile;
    std::wofstream reportStream(options.outputReportingFilePath);
    Assembler::Scanner scanner(&inputStream, true);
    Assembler::Parser parser("code", scanner, messages, reportStream);

    cout << "Assembling " << options.inputFilePath << endl;
    if (parser.Parse())
    {
        cout << "Writing objects code to " << options.outputObjectFilePath << endl;
        auto objectCode = parser.GetObjectCode();

        outputObjectFile.WriteObjectCode(outputObjectStream, objectCode);
        if (options.listSymbols)
        {
            cout << "Listing symbols" << endl;
            parser.PrintSymbols();
        }
        if (options.listSymbolCrossReferences)
        {
            cout << "Listing symbol cross reference" << endl;
            parser.PrintSymbolCrossReference();
        }
        if (options.emulate)
        {
            PrettyPrinter<wchar_t> printer(reportStream);
            std::unique_ptr<ICPUEmulator> emulator = CreateEmulator(parser.GetCPUType(), objectCode, printer);
            emulator->Run(Options::ShowInstructionResults);
        }
        return true;
    }
    cout << "Errors found, please check " << options.outputReportingFilePath << endl;

    return false;
}
