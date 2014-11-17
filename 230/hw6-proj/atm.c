#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "atm.h"
#include "command.h"
#include "trace.h"
#include "errors.h"

// The following functions can be used to check for errors. You should
// use them in the implementation of the `atm` function below.

// Checks the result of a `write` call to determine if there was an
// error. If there was an error it returns ERR_PIPE_WRITE_ERR.
static int check_pipe_write(int result) {
  if (result < 0) {
    error_msg(ERR_PIPE_WRITE_ERR, "could not write to atm output file descriptor");
    return ERR_PIPE_WRITE_ERR;
  }
  else if (result < MESSAGE_SIZE) {
    error_msg(ERR_PIPE_WRITE_ERR, "partial message written to atm output file descriptor");
    return ERR_PIPE_WRITE_ERR;
  }
  else {
    return SUCCESS;
  }
}

// Checks the result of a `read` call to determine if there was an
// error. If there was an error it returns ERR_PIPE_READ_ERR.
static int check_pipe_read(int result) {
  if (result < 0) {
    error_msg(ERR_PIPE_READ_ERR, "could not read from bank input file descriptor");
    return ERR_PIPE_READ_ERR;
  }
  else if (result < MESSAGE_SIZE) {
    error_msg(ERR_PIPE_READ_ERR, "partial message read from bank input file descriptor");
    return ERR_PIPE_READ_ERR;
  }
  else {
    return SUCCESS;
  }
}

// The `atm` function processes commands received from a trace
// file. It communicates to the bank transactions with a matching
// ID. It then receives a response from the bank process and handles
// the response appropriately.
int atm(int bank_out_fd, int atm_in_fd, int atm_id, byte cmd[]) {
  byte c;
  int i,f,t,a;
  byte atmcmd[MESSAGE_SIZE];

  cmd_unpack(cmd, &c, &i, &f, &t, &a);

  int status = SUCCESS;

  // TODO:
  // START YOUR IMPLEMENTATION

  // END YOUR IMPLEMENTATION

  return status;
}

int atm_run(char *trace, int bank_out_fd, int atm_in_fd, int atm_id) {
  int status = trace_open(trace);
  if (status == -1) {
    error_msg(ERR_BAD_TRACE_FILE, "could not open trace file");
    return ERR_BAD_TRACE_FILE;
  }

  byte cmd[MESSAGE_SIZE];
  while (trace_read_cmd(cmd) != 0) {
    status = atm(bank_out_fd, atm_in_fd, atm_id, cmd);

    // We continue if the ATM as unknown. This is ok because the trace
    // file contains commands for all the ATMs.
    if (status == ERR_UNKNOWN_ATM) {
      continue;
    }

    // We display an error message to the ATM user if the account
    // is not valid.
    if (status == ERR_UNKNOWN_ACCOUNT) {
      printf("ATM error: unknown account! ATM Out of service\n");
      continue;
    }

    // We display an error message to the ATM user if the account
    // does not have sufficient funds.
    if (status == ERR_NOFUNDS) {
      printf("not enough funds, retry transaction\n");
      continue;
    }

    // If we receive a status that is not successful we return with
    // the status.
    if (status != SUCCESS) {
      printf("status is %d\n", status);
      return status;
    }
  }

  trace_close();

  return SUCCESS;
}
