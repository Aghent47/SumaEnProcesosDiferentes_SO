#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void main(){
  pid_t pid_hijo1,pid_hijo2;
  pid_hijo1=fork();
  if(pid_hijo1==0){
    printf("Soy el hijo 1, Mi padre es= %d, Mi PID= %d\n",getppid(),getpid());
    sleep(2);
    exit(0);
  }
  pid_hijo2=fork();
  if(pid_hijo2<=0){
    printf("Soy el hijo 2, Mi padre es= %d, Mi PID= %d\n",getppid(),getpid());
    sleep(2);
    exit(0);
  }
  printf("Proceso PADRE = %d\n",getppid());
  exit(0);
}