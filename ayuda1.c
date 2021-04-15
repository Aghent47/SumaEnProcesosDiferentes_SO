#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    pid_t pidHijo;
    pidHijo=fork();
    if(pidHijo < 0){ perror("Error en la creaci´on\n"); exit(-1); }
        printf("Finalizando padre %d\n", getpid());
    wait(NULL);
 return 0;
 }
