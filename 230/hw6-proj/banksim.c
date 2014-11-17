#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include "hw6.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s trace_file\n", argv[0]);
    exit(1);
  }

  int result        = 0;
  int atm_count     = 0;
  int account_count = 0;

  // Get the number of ATMs:
  result = trace_open(argv[1]);
  if (result == -1) {
    printf("%s: could not open file %s\n", argv[0], argv[1]);
    exit (1);
  }
  atm_count     = trace_atm_count();
  account_count = trace_account_count();
  trace_close();

  // Setup bank pipes:
  int bankfd[2];
  pipe(bankfd);

  // This is a table of atm_out file descriptors. It will be used by
  // the bank process to communicate to each of the ATM processes.
  int atm_out_fd[atm_count];

  // TODO: ATM PROCESS FORKING
  // START YOUR IMPLEMENTATION

  // END YOUR IMPLEMENTATION


  // TODO: BANK PROCESS FORKING
  // START YOUR IMPLEMENTATION

  // END YOUR IMPLEMENTATION

  // Wait for each of the child processes to complete. We include +1
  // to include the bank process.
  for (int i = 0; i <= atm_count; i++) {
    wait(NULL);
  }

  return 0;
}

