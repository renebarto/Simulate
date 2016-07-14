#include <sstream>
#include <iostream>
#include "core/DefaultLogger.h"

using namespace std;
using namespace Core;

static const string DefaultDomain = "";

extern DefaultLogger & Core::TheLogger()
{
    static DefaultLogger logger(LogLevel::Error | LogLevel::Warning | LogLevel::Message);
    return logger;
}

