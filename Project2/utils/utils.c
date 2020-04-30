#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "utils.h"

clock_t begin;

void timer_begin() {
  begin = clock();
}

int timer_duration(){
  clock_t end = clock();
  int elapsed_time = ((int) (end - begin)) / CLOCKS_PER_SEC;

  return elapsed_time;
}

void log_operation(int i, int pid, long tid, int dur, int pl, int op) {
  char oper[6];
  
  switch(op) {
    case IWANT:
      strcpy(oper, "IWANT");
      break;
    case RECVD:
      strcpy(oper, "RECVD");
      break;
    case ENTER:
      strcpy(oper, "ENTER");
      break;
    case IAMIN:
      strcpy(oper, "IAMIN");
      break;
    case TIMUP:
      strcpy(oper, "TIMUP");
      break;
    case TLATE:
      strcpy(oper, "2LATE");
      break;
    case CLOSD:
      strcpy(oper, "CLOSD");
      break;
    case FAILD:
      strcpy(oper, "FAILD");
      break;
    case GAVUP:
      strcpy(oper, "GAVUP");
      break;
  }

  char log_string[MAX_STR_LEN];
  sprintf(log_string, "%d ; %d ; %d ; %ld ; %d ; %d ; %s\n", timer_duration(), i, pid, tid, dur, pl, oper);
  write(STDOUT_FILENO, log_string, strlen(log_string));
}

void send_message(int fd, int i, int pid, long tid, int dur, int pl) {
  char msg_string[MAX_STR_LEN];
  sprintf(msg_string, "[ %d, %d, %ld, %d, %d ]", i, pid, tid, dur, pl);
  write(fd, &msg_string, strlen(msg_string));
}