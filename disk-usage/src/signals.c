#include "signals.h"

bool received_sigint = false;

void sigintHandler() {
    received_sigint = true;
}

void enableSignalHandlers() {
    struct sigaction sa;
    sa.sa_handler = sigintHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}