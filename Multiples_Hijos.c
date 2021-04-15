#include <stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int crear_hijos(int num[],int,int,int);
int SumaHijo1(int num[],int,int,int);
int SumaHijo2(int num[],int,int,int);

int SumaHijo1(int num[],int n,int ind1,int ind2){
        int suma1 = 0;
         for (int i=0; i < ind2; i++){
           printf("%d,",num[i]);
           suma1 = suma1 + num[i] ;
        }     
    return(suma1);
}

int SumaHijo2(int num[],int n,int ind1,int ind2){
        int suma2 = 0;
         for (ind2; ind2 < n-1; ind2++){
           printf("%d,",num[ind2]);
           suma2 = suma2 + num[ind2] ;
        }
     
    return(suma2);
}

int crear_hijos(int num[],int n,int ind1,int ind2){
    
    pid_t pidHijo;
    int suma1 = 0, suma2 = 0;
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
        if(pidHijo == 0 & i == 0){
            
            suma1 = SumaHijo1(num,n,ind1,ind2);
        printf("HIJO: PID = %d, Valor Cal = %d \n",getpid(),suma1);
         FILE *out = fopen("out.txt", "wt");
             fprintf (out,"%d",suma1);
             fclose(out);
            return(suma1);

        }else if ( i == 1){
           
            suma2 = SumaHijo2(num,n,ind1,ind2);
         printf("HIJO: PID = %d, Valor Cal = %d \n",getpid(),suma2);
             FILE *out = fopen("out.txt", "a+");
             fprintf (out,"\n%d",suma2);
             fclose(out);
          
            return (suma2);
        }
        
        //error
    }
    return (0);
}

int leerTotal(){
 FILE *myfile;
    int sumap1=0,sumap2=0,total=0; myfile = fopen("out.txt","r");
    if(!myfile) perror("Error padre archivo resultados");
        fscanf(myfile,"%d", &sumap1);
        fscanf(myfile,"%d", &sumap2);
    total = sumap1 + sumap2; 
    return total;
 }

int main (void){
    int *num,n=0;
    int dato,dato2 = 0, suma = 0;
    int indiceP1=0, indiceP2,TOTAL=0;

    FILE *archivo = fopen( "input.txt","r+" );
        if (archivo == NULL) {
            printf( "ERROR" );
            return 0;
        }
    while (!feof(archivo)) {
        fscanf( archivo, "%d", &dato );
            num[n] = dato;
            n++;
    }
    indiceP2 = (n-1)/2;
    crear_hijos(num,n,indiceP1,indiceP2);
    
    printf("TOTAL = %d \n",leerTotal());

    fclose(archivo);
    
    
}