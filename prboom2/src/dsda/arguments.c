#include "arguments.h"
#include "m_argv.h"
#include "m_cheat.h"
#include <string.h>

#define CHT_ARG "-cheat"

void dsda_CheckCheatArgs(void) {
  int i;
  if ((i = M_CheckParm(CHT_ARG))) {
    while (++i != myargc && *myargv[i] != '-') {
      if (strcmp(myargv[i], "iddqd") == 0) {
        M_CheatGod();
      }
    }
  }
}
