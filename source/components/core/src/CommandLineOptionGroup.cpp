#include "core/CommandLineOptionGroup.h"
#include "core/Exception.h"

using namespace std;
using namespace Core;

CommandLineOptionGroup::CommandLineOptionGroup(const string & name,
                                               const string description /*= ""*/)
    : name(name)
    , description(description)
    , options()
{
}

CommandLineOptionGroup::~CommandLineOptionGroup()
{
    options.clear();
}

void CommandLineOptionGroup::Add(const CommandLineOptionPtr option)
{
    options.push_back(option);
}
