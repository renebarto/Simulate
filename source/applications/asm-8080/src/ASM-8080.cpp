#include "ASM-8080.h"
#include "Parser.h"

using namespace std;
using namespace ASM;

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
    std::ofstream outputObjectFile(options.outputObjectFilePath, std::ios::binary);
    std::wofstream reportStream(options.outputReportingFilePath);
    Assembler::Scanner scanner(&inputStream, true);
    Assembler::Parser parser(scanner, messages, reportStream);

    cout << "Assembling " << options.inputFilePath << endl;
    if (parser.Parse())
    {
        cout << "Writing objects code to " << options.outputObjectFilePath << endl;
        auto machineCode = parser.GetMachineCode();
        outputObjectFile.write((const char *)machineCode.data(), machineCode.size());
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
        return true;
    }
    cout << "Errors found, please check " << options.outputReportingFilePath << endl;

    return false;
}
