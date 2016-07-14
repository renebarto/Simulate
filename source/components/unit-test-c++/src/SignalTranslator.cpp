#include "unit-test-c++/SignalTranslator.h"

namespace UnitTestCpp
{

#if defined(__GNUC__)
sigjmp_buf * SignalTranslator::s_jumpTarget = nullptr;

namespace
{

void SignalHandler(int sig)
{
    siglongjmp(*SignalTranslator::s_jumpTarget, sig);
}

}

#endif

SignalTranslator::SignalTranslator()
{
#if defined(__GNUC__)
	m_oldJumpTarget = s_jumpTarget;
    s_jumpTarget = &m_currentJumpTarget;

    struct sigaction action;
    action.sa_flags = 0;
    action.sa_handler = SignalHandler;
    sigemptyset(&action.sa_mask);

    sigaction(SIGSEGV, &action, &m_old_SIGSEGV_action);
    sigaction(SIGFPE , &action, &m_old_SIGFPE_action );
    sigaction(SIGTRAP, &action, &m_old_SIGTRAP_action);
    sigaction(SIGBUS , &action, &m_old_SIGBUS_action );
    sigaction(SIGILL , &action, &m_old_SIGBUS_action );
#endif
}

SignalTranslator::~SignalTranslator()
{
#if defined(__GNUC__)
	sigaction(SIGILL , &m_old_SIGBUS_action , nullptr);
    sigaction(SIGBUS , &m_old_SIGBUS_action , nullptr);
    sigaction(SIGTRAP, &m_old_SIGTRAP_action, nullptr);
    sigaction(SIGFPE , &m_old_SIGFPE_action , nullptr);
    sigaction(SIGSEGV, &m_old_SIGSEGV_action, nullptr);

    s_jumpTarget = m_oldJumpTarget;
#endif
}

} // namespace UnitTestCpp
