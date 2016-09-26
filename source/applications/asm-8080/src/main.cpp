#include <fstream>
#include <iostream>
#include <CommandLineOptionsParser.h>
#include <ASM-8080.h>

using namespace std;
using namespace ASM;

int main(int argc, char * argv[])
{
    cout << "8080 assembler (C) Barsoft 2016" << endl << endl;
    CommandLineOptionsParser commandLineParser;

    commandLineParser.Parse(argc, argv);
    commandLineParser.ResolveDefaults();

    ASM_8080 assembler(commandLineParser);
    if (!assembler.Run())
    {
        cout << endl << "Errors assembling." << endl;;
    }
    cout << endl << "Done." << endl;;
}