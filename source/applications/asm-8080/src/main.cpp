#include <fstream>
#include <iostream>
#include <CommandLineOptionsParser.h>
#include <Parser8080.h>

using namespace std;

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
    Parser8080 parser(reader);
    parser.Parse();
}