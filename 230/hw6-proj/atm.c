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

	/*
	 *	Task 1: First, you must check to make sure that the 
	 *	ID in the unpacked command is the same as this ATMs ID.
	 *	If it is not the same ATM ID then you must return 
	 *	the ERR_UNKNOWN_ATM error code. This will be used by 
	 *	the atm_run function to skip over that trace command 
	 *	as it is not to be processed by this ATM.
	 */
	 if(atm_id != i){ 
	 	return ERR_UNKNOWN_ATM;
	 }

	 /*
	  * Task 2: Next, you need to handle the logic for each of 
	  * the commands: CONNECT, EXIT, DEPOSIT, WITHDRAW, TRANSFER, 
	  * and BALANCE. Each of these commands perform the same functionality.
      */
      if (c == CONNECT || c == EXIT || c == DEPOSIT || c == WITHDRAW 
        || c == TRANSFER || c == BALANCE){
        status = write(bank_out_fd, cmd, MESSAGE_SIZE);
       if(check_pipe_write(status) != SUCCESS)  return check_pipe_write(status);
      

      /*
       * After you write the command to the bank and it is successful
       * you must then read the response from the bank using the read 
       * system call into the empty atmcmd command buffer.
       */
         status = read(atm_in_fd, atmcmd, MESSAGE_SIZE);
  
       if(check_pipe_read(status) != SUCCESS) return check_pipe_read(status);
      
        cmd_unpack( cmd, &c, &i,  &f,  &t,  &a);
      
     
       if(c == OK)
        status = SUCCESS;
       else if(c == NOFUNDS)
        status = ERR_NOFUNDS;
       else if(c == ACCUNKN)
        status = ERR_UNKNOWN_ACCOUNT;
       else {
        error_msg(ERR_UNKNOWN_CMD, "The command you tried to use does not exist, please try again!");
        status = ERR_UNKNOWN_CMD;
       }
       }       else {
        error_msg(ERR_UNKNOWN_CMD, "The command you tried to use does not exist, please try again!");
        status = ERR_UNKNOWN_CMD;
       }

   

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
