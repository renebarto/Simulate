#include "osal/thread.h"

#if defined(__GNUC__)

#include  <pthread.h>

using namespace std;
using namespace OSAL;

void OSAL::SetThreadName(std::thread & thread, char const * threadName)
{
	auto handle = thread.native_handle();
	pthread_setname_np(handle, threadName);
}

bool OSAL::IsThreadSelf(std::thread & thread)
{
	return thread.native_handle() == pthread_self();
}

#endif // defined(__GNUC__)
