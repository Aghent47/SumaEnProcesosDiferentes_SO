#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void leer_Archivo();

int main(){
    pid_t pidHijo , pidHijo2;
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
     for (int i=0; i < n-1; i++){
           printf("%d,",num[i]);
           suma = suma + num[i];
        }
   // printf("\nLongitud del vector es: %d \n", n-1);
    indiceP2 = (n-1)/2;
  //  printf("\nEl indece es: %d y la suma es %d \n", indiceP2 , suma);
    fclose(archivo);
    
    pidHijo=fork();
    
    switch (pidHijo)  
	{ 
        case -1:/* Error en la ejecución de fork(), se debe implementar la \\
                    función manejadora del error*/
                perror("Error en la creacion\n");
            	exit(EXIT_FAILURE);
        case 0: /* Bloque proceso hijo */ 
                printf("\nHIJO El indece es: %d y la suma es: %d \n", indiceP2 , suma); 
		        break;
        default: /* Bloque proceso padre */ 
            wait(NULL);
                 printf("\nPADRE El indece es: %d y la suma es: %d \n", indiceP2 , suma);
 	}

    
	
}
