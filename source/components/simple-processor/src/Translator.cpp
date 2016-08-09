#include "simple-processor/Translator.h"

using namespace Simulate;

Translator::Translator(InputSpecifier & inputSpecifier, OutputSpecifier & outputSpecifier)
    : inputSpecifier(inputSpecifier)
    , outputSpecifier(outputSpecifier)
{
}


Translator::~Translator()
{
}

