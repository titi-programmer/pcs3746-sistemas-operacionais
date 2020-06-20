#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define len(_arr) ((int)((&_arr)[1] - _arr))

static const char * const programs[] = { "/divideByZero" };

void panic(const char *msg) {
  fprintf(stderr, "%s: %s (errno = %d)\n", msg, strerror(errno), errno);
  exit(-1);
}

int main() {
  printf("\nInit starting!\n");

  // forking process to create a child, prevent kernel panic by trying to kill init
  pid_t pid;
  for (int i = 0; i < len(programs); i++) {
		const char *path = programs[i];
    pid = fork();
    if (pid == -1) {
      panic("fork");
    } else if (pid) {
      printf("I'm father process! Pid: %d, fork() returns: %d (child pid)\n", getpid(), pid);
    } else {
      execl(path, path, (char *)NULL);
      panic("execl");
    }
  }
  printf("Created fork!\n");

  // for (;;) {
  //   printf("Signaling process: %d, with signal SIGFPE: %d\n", pid, SIGFPE);
  //   sleep(6);
  //   kill(pid, SIGFPE);
  // }

  printf("Awaiting child process to exit/terminate\n");
  int program_count = len(programs);
  while (program_count) {
    int wstatus;
    pid_t pidt = wait(&wstatus);
    if (WIFEXITED(wstatus))
      printf("pid %d exited with %d\n", pidt, WEXITSTATUS(wstatus));
    else if (WIFSIGNALED(wstatus))
      printf("pid %d killed by signal %d\n", pidt, WTERMSIG(wstatus));
    else 
      panic("wait");
  }

  return 0;
}
