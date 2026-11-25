#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(void)
{
  struct pinfo entries[NPROC];
  int children[2];
  int count;
  int found = 0;

  for (int index = 0; index < 2; index++) {
    int pid = fork();

    if (pid < 0) {
      fprintf(2, "procstest: fork failed\n");
      for (int previous = 0; previous < index; previous++)
        wait(0);
      exit(1);
    }

    if (pid == 0) {
      pause(100);
      exit(0);
    }

    children[index] = pid;
    printf("parent=%d child=%d\n", getpid(), pid);
  }

  pause(10);

  count = getprocs(entries, NPROC);
  if (count < 0) {
    fprintf(2, "procstest: getprocs failed\n");
    for (int index = 0; index < 2; index++)
      wait(0);
    exit(1);
  }

  printf("\nProcesses returned: %d\n", count);

  for (int index = 0; index < count; index++) {
    printf("pid=%d ppid=%d state=%d name=%s\n",
           entries[index].pid,
           entries[index].ppid,
           entries[index].state,
           entries[index].name);

    for (int child = 0; child < 2; child++) {
      if (entries[index].pid == children[child] &&
          entries[index].ppid == getpid())
        found++;
    }
  }

  if (found == 2)
    printf("Child PID and PPID test: PASS\n");
  else
    printf("Child PID and PPID test: FAIL\n");

  count = getprocs(entries, 1);
  printf("max=1: returned %d, expected 1\n", count);

  count = getprocs(entries, NPROC);
  printf("max=NPROC: returned %d, expected more than 1\n", count);

  count = getprocs(entries, 0);
  printf("max=0: returned %d, expected 0\n", count);

  count = getprocs(entries, -1);
  printf("max=-1: returned %d, expected -1\n", count);

  for (int index = 0; index < 2; index++)
    wait(0);

  exit(0);
}
