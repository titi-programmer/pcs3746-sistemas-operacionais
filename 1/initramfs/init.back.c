#include <dirent.h>
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

void panic(const char *msg) {
  fprintf(stderr, "%s: %s (errno = %d)\n", msg, strerror(errno), errno);
  exit(-1);
}

void printRep(const char *str) {
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    printf("Listing \"%s\" repository - %s:\n", cwd, str);
    while ((dir = readdir(d)) != NULL) {
      printf("%s  ", dir->d_name);
    }
    printf("\n\n");
    closedir(d);
  }
}

int main()
{
  printf("\nInit starting!\n");
  printRep("Before opening file");

  // forking process to create a child, prevent kernel panic by trying to kill init
  pid_t pid = fork();
  if (pid == -1) {
    panic("fork");
  } else if (pid) {
    printf("I'm father process! Pid: %d, fork() returns: %d (child pid)\n", getpid(), pid);
  } else {
    printf("I'm child process! Pid: %d, fork() returns: %d\n", getpid(), pid);
    int fd;
    // int bytes;
    printf("Opening file text.txt\n");
    fd = open("text.txt", O_WRONLY | O_CREAT, 0777);
    printRep("After opening file");
    // for(;;) will never exit loop, change code to test exit status of child process
    // example: for(int i; i < 100; i++)
    for (;;)
      write(fd, "muitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytes", sizeof("muitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytesmuitosbytes"));
    printf("Closing file text.txt\n");
    close(fd);
    return 0;
  }
  printf("Created fork!\n");

  printf("Awaiting child process to exit/terminate\n");
  int wstatus;
  pid_t pidt = wait(&wstatus);
  if (WIFEXITED(wstatus))
    printf("pid %d exited with %d\n", pidt, WEXITSTATUS(wstatus));
  else if (WIFSIGNALED(wstatus))
    printf("pid %d killed by signal %d\n", pidt, WTERMSIG(wstatus));
  else 
    panic("wait");

  for (;;) {
    printf("Init lives!\n");
    sleep(60);
  }
  return 0;
}
