#ifndef __ATM_H
#define __ATM_H

// The `atm_run` function runs an "ATM" on the trace file. This function
// has the following parameters:
//
// trace        - the name of the trace file to read
// bank_out_fd  - the output file descriptor the ATM will write commands
// atm_in_fd    - the input file descriptor the ATM will receive
//                commands
// atm_id       - the ID of the ATM to run
int atm_run(char *trace, int bank_out_fd, int atm_in_fd, int atm_id);

#endif
