#pragma once

#include <thread>

namespace OSAL
{

void SetThreadName(std::thread & thread, char const * threadName);
bool IsThreadSelf(std::thread & thread);

}
