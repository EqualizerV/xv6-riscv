#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

char *
state_name(int state)
{
  static char *names[] = {
    "UNUSED",
    "USED",
    "SLEEPING",
    "RUNNABLE",
    "RUNNING",
    "ZOMBIE"
  };

  if (state < 0 || state >= sizeof(names) / sizeof(names[0]))
    return "UNKNOWN";

  return names[state];
}

int
main(int argc, char *argv[])
{
    struct pinfo entries[NPROC];
    int count;

    count = getprocs(entries, NPROC);
    if (count < 0) {
        fprintf(2, "ps: getprocs failed\n");
        exit(1);
    }

    printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");

    for (int index = 0; index < count; index++) {
        printf("%d\t%d\t%s\t%lu\t%s\n",
            entries[index].pid,
            entries[index].ppid,
            state_name(entries[index].state),
            entries[index].sz,
            entries[index].name);
    }

    exit(0);
}
