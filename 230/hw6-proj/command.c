#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "command.h"

// An array of strings that corresponds to each of the command types.
char *cmd_strings[] = {
  "OK",
  "CONNECT",
  "EXIT",
  "DEPOSIT",
  "WITHDRAW",
  "TRANSFER",
  "BALANCE",
  "NOFUNDS",
  "ATMUNKN",
  "ACCUNKN"
};

void cmd_pack(byte cmd[], cmd_t c, int id, int from, int to, int amt) {
  memset(cmd, 0, MESSAGE_SIZE);
  cmd[MESSAGE_CMD] = c;

  cmd[MESSAGE_ID]   = (id >> 24);
  cmd[MESSAGE_ID+1] = (id >> 16) & 0xFF;
  cmd[MESSAGE_ID+2] = (id >>  8) & 0xFF;
  cmd[MESSAGE_ID+3] = id & 0xFF;

  cmd[MESSAGE_FROM]   = (from >> 24);
  cmd[MESSAGE_FROM+1] = (from >> 16) & 0xFF;
  cmd[MESSAGE_FROM+2] = (from >>  8) & 0xFF;
  cmd[MESSAGE_FROM+3] = from & 0xFF;

  cmd[MESSAGE_TO]   = (to >> 24);
  cmd[MESSAGE_TO+1] = (to >> 16) & 0xFF;
  cmd[MESSAGE_TO+2] = (to >>  8) & 0xFF;
  cmd[MESSAGE_TO+3] = to & 0xFF;

  cmd[MESSAGE_AMT]   = (amt >> 24);
  cmd[MESSAGE_AMT+1] = (amt >> 16) & 0xFF;
  cmd[MESSAGE_AMT+2] = (amt >>  8) & 0xFF;
  cmd[MESSAGE_AMT+3] = amt & 0xFF;
}

void cmd_unpack(byte cmd[], cmd_t *c, int *id, int *from, int *to, int *amt) {
  *c    = 0;
  *id   = 0;
  *from = 0;
  *to   = 0;
  *amt  = 0;

  *c = cmd[MESSAGE_CMD];
  *id |= cmd[MESSAGE_ID]   << 24;
  *id |= cmd[MESSAGE_ID+1] << 16;
  *id |= cmd[MESSAGE_ID+2] << 8;
  *id |= cmd[MESSAGE_ID+3];

  *from |= cmd[MESSAGE_FROM]   << 24;
  *from |= cmd[MESSAGE_FROM+1] << 16;
  *from |= cmd[MESSAGE_FROM+2] << 8;
  *from |= cmd[MESSAGE_FROM+3];

  *to |= cmd[MESSAGE_TO]   << 24;
  *to |= cmd[MESSAGE_TO+1] << 16;
  *to |= cmd[MESSAGE_TO+2] << 8;
  *to |= cmd[MESSAGE_TO+3];

  *amt |= cmd[MESSAGE_AMT]   << 24;
  *amt |= cmd[MESSAGE_AMT+1] << 16;
  *amt |= cmd[MESSAGE_AMT+2] << 8;
  *amt |= cmd[MESSAGE_AMT+3];
}

void cmd_dump(char *msg, int id, byte cmd[]) {
  // We use an environment variable to toggle debug printing. If the
  // environment variable `BANKSIM_DEBUG` is set then commands will be
  // dumped to standard output.
  char *eval = getenv("BANKSIM_DEBUG");
  if (eval != NULL) {
    cmd_t c;
    int i,f,t,a;
    cmd_unpack(cmd, &c, &i, &f, &t, &a);
    printf("%s[%d] %s %d %d %d %d\n", msg, id, cmd_strings[c], i, f, t, a);
  }
}
