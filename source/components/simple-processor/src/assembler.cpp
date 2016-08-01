#include "simple-processor/assembler.h"

#include <algorithm>
#include <iomanip>

using namespace Simulate;
using namespace std;

Assembler::Assembler(SimpleMachine & machine, std::istream & source)
    : machine(machine)
    , source(source)
{
}

Assembler::~Assembler()
{
}

void Assembler::Assemble(std::vector<uint8_t> machineCode, bool & errors)
{
    machineCode.clear();
    std::string mnemonic;   // mnemonic for matching
    uint8_t lc = 0;         // location counter
    SimpleProcessor::Opcode op; // assembled opcode
    int number;             // assembled number
    char ch;                // general character for input
    bool ok;                // error checking on reading numbers

    printf("Assembling code ... \n");
    lc = 0;                                     // initialize location counter
    errors = false;
    do
    {
        do
            ch = toupper(source.get());
        while (ch <= ' ' && !source.eof());     // skip spaces and blank lines
    
        if (!source.eof())                      // there should be a line to assemble
        {
            if (isupper(ch))                    // we should have a mnemonic
            {
                ReadMnemonic(ch, mnemonic);     // unpack it
                op = machine.LookupOpcode(mnemonic);   // look it up
                if (op == SimpleProcessor::Opcode::BAD_OPCODE) // the opcode was unrecognizable
                {
                    cout << mnemonic << " - Bad mnemonic at " << lc << std::endl; 
                    errors = true;
                }
                machineCode.push_back(uint8_t(op)); // store numerical equivalent
            }
            else // we should have a numeric constant
            {
                ReadInt(ch, number, ok);        // unpack it
                if (!ok)
                {
                    cout << "Bad number at " << lc << std::endl;
                    errors = true;
                }
                if (number >= 0)                // convert to proper byte value
                    machineCode.push_back(number % 256);
                else
                    machineCode.push_back((256 - abs(number) % 256) % 256);
            }
            lc = (lc + 1) % 256;                // bump up location counter
        }
    }
    while (!source.eof());
}

void Assembler::ReadMnemonic(char & ch, std::string & mnemonic)
{
    mnemonic = "";
    int i = 0;
    while (ch > ' ')
    {
        if (i <= 2)
        { 
            mnemonic += ch; 
            i++;
        }
        ch = toupper(source.get());
    }
}

void Assembler::ReadInt(char & ch, int & number, bool & ok)
{
    ok = true;
    number = 0;
    bool negative = (ch == '-');
    if (ch == '-' || ch == '+')
        ch = source.get();
    while (ch > ' ')
    {
        if (isdigit(ch))
            number = number * 10 + (ch - '0');
        else
            ok = false;
        ch = source.get();
    }
    if (negative)
        number = -number;
}

