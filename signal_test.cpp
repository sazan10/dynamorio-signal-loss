#include <pthread.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>

using std::cout;
using std::endl;

void* sayGoodMorningAfterFiveSeconds(void*)
{
    usleep(5000000);
    cout << "good morning from thread" << endl;
    return NULL;
}

static const int signalNumber = 37;

void registerSignalHandler(void (*handler)(int, siginfo_t*, void*))
{
    struct sigaction action;
    action.sa_handler = NULL;
    action.sa_sigaction = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    sigaction(signalNumber, &action, NULL);
}

unsigned int signalCount = 0;

void handleSignal(int signalNumber, siginfo_t* signalInfo, void* context)
{
    cout << "signal " << signalNumber << " received" << endl;
    usleep(1000000);
    cout << "leaving signal handler after one second" << endl;
    ++signalCount;
}


int main()
{
    registerSignalHandler(handleSignal);

    pthread_t thread;

    cout << "before thread creation" << endl;

    pthread_create(&thread, NULL, sayGoodMorningAfterFiveSeconds, NULL);

    //send signal twice to thread, both should be processed
    pthread_kill(thread, signalNumber);
    pthread_kill(thread, signalNumber);

    pthread_join(thread, NULL);

    return signalCount != 2;
}
