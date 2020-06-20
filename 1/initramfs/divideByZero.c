#include<stdio.h>
#include<signal.h>

void bypass_sigint(int sig_no)
{
  printf("divide by zero\n");
}

int main()
{
  int a,b,c;
  a = 2;
  b = 0;
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));   // inicializa a area da memoria de sa com 0
  sa.sa_handler = &bypass_sigint;
  sigaction(SIGFPE, &sa,NULL);    // aponta o handler para &sa, contendo a funcao bypass_sigint
  while (1) {
    // sleep(2);
    // printf("do nothing \n ");
    printf("Division %d by %d\n", a,b);
    c = a/b;
  }
  return 0;
}
