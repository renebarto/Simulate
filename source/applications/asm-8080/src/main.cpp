#include <fstream>
#include <iostream>
#include <CommandLineOptionsParser.h>
#include <LineParser8080.h>
#include <Parser.h>

using namespace std;
using namespace ASM8080;

int main(int argc, char * argv[])
{
    cout << "8080 assembler (C) Barsoft 2016" << endl;
    CommandLineOptionsParser commandLineParser;

    commandLineParser.Parse(argc, argv);

    ifstream inputFile(commandLineParser.inputFilePath);
    ofstream outputObjectFile(commandLineParser.outputObjectFilePath, ios::binary);
    ofstream outputListingFile(commandLineParser.outputListingFilePath);
    ofstream outputCrossReferenceFile(commandLineParser.outputCrossReferenceFilePath);

    Reader reader(inputFile);
    LineParser8080 lineParser;
    Parser parser(reader, lineParser);
    parser.Parse();
}