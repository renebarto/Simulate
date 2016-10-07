#include "emulator/processor.h"

using namespace Simulate;

Processor::Processor(double clockFreq,
                     uint8_t addressBusWidth,
                     uint8_t dataBusWidth)
    : clock(picoseconds(int64_t(std::tera::num / clockFreq + 0.5)))
    , addressBusWidth(addressBusWidth)
    , dataBusWidth(dataBusWidth)
{

}

Processor::~Processor()
{

}

