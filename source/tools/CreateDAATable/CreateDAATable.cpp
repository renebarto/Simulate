// CreateDAATable.cpp : Defines the entry point for the console application.
//

#include <cstdint>
#include <iostream>
#include <iomanip>

using namespace std;

bool parity(uint8_t a)
{
    uint8_t numOnes = 0;
    while (a != 0)
    {
        if (a & 1)
            numOnes++;
        a >>= 1;
    }
    return (numOnes & 0x01) == 0x00;
}

int main()
{
    cout << "static const uint16_t correctDAATable[1024] =" << endl << "{" << endl;

    for (int i = 0; i < 1024; i++)
    {
        if (i % 8 == 0)
            cout << "    ";
        uint8_t a = i & 0xFF;
        bool c = ((i & 0x100) != 0);
        bool ac = ((i & 0x200) != 0);
        if (((a & 0x0F) > 0x09) | ac)
        {
            a += 0x06;
            ac = true;
        }
        else
        {
            ac = false;
        }
        if ((((a >> 4) & 0x0F) > 0x09) | c)
        {
            a += 0x60;
            c = true;
        }
        else
        {
            c = false;
        }
        bool z = !a;
        bool s = ((a & 0x80) != 0);
        bool p = parity(a);
        uint8_t flags = (c ? 0x01 : 0x00);
        flags |= (ac ? 0x10 : 0x00);
        flags |= (p ? 0x04 : 0x00);
        flags |= (z ? 0x40 : 0x00);
        flags |= (s ? 0x80 : 0x00);
        cout << "0x" << hex << setw(2) << std::setfill('0') << int(flags) << setw(2) << std::setfill('0') << int(a) << ", ";
        if (i % 8 == 7)
            cout << endl;
    }
    cout << "};" << endl;
    return 0;
}

