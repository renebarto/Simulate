#pragma once

#include <iostream>
#include <iomanip>
#include <setjmp.h>
#include <signal.h>

namespace Core
{

enum class SignalType
{
    Unknown = -1,
    Hangup = SIGHUP,                // Hangup detected on controlling terminal or death of controlling process
    Interrupt = SIGINT,             // Interrupt from keyboard
    Quit = SIGQUIT,                 // Quit from keyboard
    IllegalInstruction = SIGILL,    // Illegal Instruction
    Abort = SIGABRT,                // Abort signal from abort(3)
    FPException = SIGFPE,           // Floating point exception
    SegmentViolation = SIGSEGV,     // Invalid memory reference
    BrokenPipe = SIGPIPE,           // Broken pipe: write to pipe with no readers
    Alarm = SIGALRM,                // Timer signal from alarm(2)
    Terminate = SIGTERM,            // Termination signal
    UserDefined1 = SIGUSR1,         // User-defined signal 1
    UserDefined2 = SIGUSR2,         // User-defined signal 2
    ChildTerminate = SIGCHLD,       // Child stopped or terminated
    Continue = SIGCONT,             // Continue if stopped
    TTYStop = SIGTSTP,              // Stop typed at TTY
    TTYInput = SIGTTIN,             // TTY input for background process
    TTYOutput = SIGTTOU,            // TTY output for background process
};

template <int sig>
class SignalTranslator
{
public:
    SignalTranslator();
    SignalTranslator(const SignalTranslator &) = delete;
    SignalTranslator(__sighandler_t handler);
    virtual ~SignalTranslator();

    SignalTranslator & operator = (const SignalTranslator &) = delete;

    static sigjmp_buf * mainJumpTarget;
    static SignalType currentSignalType;
    static void OnSignalCaught();

protected:
    virtual void OnSignalCaught(SignalType UNUSED(signalType))
    {
        std::cout << std::endl << "Caught signal, ignoring..." << std::endl << std::flush;
    }

    sigjmp_buf currentJumpTarget;
    sigjmp_buf * savedJumpTarget;
    SignalTranslator * savedHandler;

    static SignalTranslator * currentHandler;

    struct sigaction savedAction;
};

template<int sig>
sigjmp_buf * SignalTranslator<sig>::mainJumpTarget = 0;

template<int sig>
SignalTranslator<sig> * SignalTranslator<sig>::currentHandler = 0;

template<int sig>
SignalType SignalTranslator<sig>::currentSignalType = SignalType::Unknown;

namespace
{

template<int sig>
void SignalHandler(int signal)
{
    siglongjmp(*SignalTranslator<sig>::mainJumpTarget, signal);
}

}

template<int sig>
SignalTranslator<sig>::SignalTranslator()
{
    savedJumpTarget = mainJumpTarget;
    mainJumpTarget = &currentJumpTarget;
    savedHandler = currentHandler;
    currentHandler = this;

    struct sigaction action;
    action.sa_flags = 0;
    action.sa_handler = SignalHandler<sig>;
    sigemptyset(&action.sa_mask);

    sigaction(sig, &action, &savedAction);
    currentSignalType = (SignalType)sig;
}

template<int sig>
SignalTranslator<sig>::SignalTranslator(__sighandler_t handler)
{
    struct sigaction action;
    action.sa_flags = 0;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);

    int result = sigaction(sig, &action, &savedAction);
    if (result == -1)
        Core::Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

template<int sig>
SignalTranslator<sig>::~SignalTranslator()
{
    sigaction(sig, &savedAction, 0);

    if (currentHandler != 0)
        mainJumpTarget = savedJumpTarget;
}

template<int sig>
void SignalTranslator<sig>::OnSignalCaught()
{
    currentHandler->OnSignalCaught(currentSignalType);
}

#define CORE_SETUP_SIGNAL_HANDLER(sig) \
    if (__extension__ sigsetjmp(*Core::SignalTranslator<sig>::mainJumpTarget, 1) != 0) \
    { \
        Core::SignalTranslator<sig>::OnSignalCaught(); \
    }

} // namespace Core

