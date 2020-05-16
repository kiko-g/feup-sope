#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include "utils.h"

time_t begin;

void timer_begin() {
  begin = time(NULL);
}

int timer_duration(){
  time_t end = time(NULL);
  int elapsed_time = ((int) (end - begin));

  return elapsed_time;
}

void log_operation(int i, int pid, long tid, int dur, int pl, char* op) {
  char log_string[MAX_STR_LEN];
  sprintf(log_string, "%d ; %d ; %d ; %ld ; %d ; %d ; %s\n", timer_duration(), i, pid, tid, dur, pl, op);
  write(STDOUT_FILENO, log_string, strlen(log_string));
}

void send_message(int fd, int i, int pid, long tid, int dur, int pl) {
  char msg_string[MAX_STR_LEN];
  sprintf(msg_string, "[ %d, %d, %ld, %d, %d ]", i, pid, tid, dur, pl);
  write(fd, &msg_string, strlen(msg_string));
}

void install_sigpipe_handler() {

    struct sigaction sigpipeIgnore;
    sigpipeIgnore.sa_handler = SIG_IGN;
    sigemptyset(&sigpipeIgnore.sa_mask);
    sigpipeIgnore.sa_flags = 0;

    if (sigaction(SIGPIPE,&sigpipeIgnore,NULL) < 0)  {        
        fprintf(stderr,"Failed to install SIGPIPE handler\n");        
        exit(1);  
    }  
}