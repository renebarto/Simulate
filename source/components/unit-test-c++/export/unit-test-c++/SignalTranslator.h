#pragma once

#include <setjmp.h>
#include <signal.h>

namespace UnitTestCpp
{

class SignalTranslator
{
public:
    SignalTranslator();
    ~SignalTranslator();

#if defined(__GNUC__)
	static sigjmp_buf * s_jumpTarget;
#endif

private:
#if defined(__GNUC__)
	sigjmp_buf m_currentJumpTarget;
    sigjmp_buf * m_oldJumpTarget;

    struct sigaction m_old_SIGFPE_action;
    struct sigaction m_old_SIGTRAP_action;
    struct sigaction m_old_SIGSEGV_action;
    struct sigaction m_old_SIGBUS_action;
    struct sigaction m_old_SIGABRT_action;
    struct sigaction m_old_SIGALRM_action;
#endif
};

#if !defined (__GNUC__)
    #define UNITTEST_EXTENSION
#else
    #define UNITTEST_EXTENSION __extension__
#endif

#if !defined (__GNUC__)
#define UNITTEST_THROW_SIGNALS
#else
#define UNITTEST_THROW_SIGNALS \
    ::UnitTestCpp::SignalTranslator sig; \
    if (UNITTEST_EXTENSION sigsetjmp(*::UnitTestCpp::SignalTranslator::s_jumpTarget, 1) != 0) \
        throw("Unhandled system exception");
#endif

} // namespace UnitTestCpp

