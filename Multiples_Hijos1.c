#include <stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void SumaHijos(int num[],int );
void SumaHijos(int num[],int n){

          int suma = 0;
         for (int i=0; i < n-1; i++){
           printf("%d,",num[i]);
           suma = suma + num[i];
        }
        printf("HIJO: PID = %d, Valor Cal = %d \n",getpid(),suma);
  
}

void crear_hijos(int vector[],int t){
    
    pid_t pidHijo;
    int NUM_CHILD = 2, status;
    bool aux = true; 
    
    for (int i = 0; i < NUM_CHILD; i++){
        pidHijo = fork();

      if(pidHijo > 0 ){
          if(aux){
              printf("\nPADRE El indece es: %d \n", getpid());
              aux = false;
          }
        pidHijo =  wait(&status);
        
      }else
        if(pidHijo == 0){
            SumaHijos(vector,t);
        }
        //error
    }
}

int main (void){
    int *num,n=0;
    int dato,dato2 = 0, suma = 0;
    int indicesP1, indiceP2;

    FILE *archivo = fopen( "input.txt","r+" );
        if (archivo == NULL) {
            printf( "ERROR\n" );
            return 0;
        }
    while (!feof(archivo)) {
        fscanf( archivo, "%d", &dato );
            num[n] = dato;
            n++;
    }
    
    crear_hijos(num,n);
    // printf("\nLongitud del vector es: %d \n", n-1);
    indiceP2 = (n-1)/2;
  //  printf("\nEl indece es: %d y la suma es %d \n", indiceP2 , suma);
    fclose(archivo);

    
}