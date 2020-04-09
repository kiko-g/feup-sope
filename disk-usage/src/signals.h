#include <stdbool.h>

void install_sighandlers();

void install_sigactions(); 

void sigint_handler(int signo);

void sigstop_handler(int signo);

void sigcont_handler(int signo);

void sigterm_handler(int signo);

bool askTerminateProgram();
