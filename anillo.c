#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#define MAX_PROC 1023
#define MAX_SPACE_TUB 1048575 

void error(char*);  //Funcion de error
void esperar_padre(int);    //funcion para esperar padre
void generar_archivo(char *, int);  //Funcion para generar archivo de texto - nombre / cantidad caracteres
int enviar_info();
void leer_info(int);
int reenviar(int);

// 0=lectura
// 1=escritura
//!hijo es el hijo no el padre
FILE *archivo = NULL;
int **fd = NULL;
int main(int argc, char const *argv[]){

    srand(time(NULL));
    pid_t padre = getpid();
    pid_t hijo = (pid_t)(0);
    char instruccion[30] = {'\0'};  //para guardar instruccion
    int  n_proc = 0, p = 0, n = 0;
    char *filename = "info.txt";   //nombre archivo de texto
    char buffer[20] = {'\0'};
    int bytes = 0;
    int sigo=1;
    int num_tub=0;
    
   printf("Ingrese cantidad de procesos: ");
    scanf("%d",&n_proc);//Pido la cantidad de veces a realizar
    fflush(stdin);

    if(n_proc>0 && n_proc<=MAX_PROC){
       
     //   generar_archivo(filename, 200); //Genero el archivo
        num_tub = n_proc+1;
        fd = (int**)malloc(num_tub*sizeof(int*));//pongo la variable fd tipo matriz
        
        for(int i=0; i<num_tub; i++){
            fd[i]= (int*)malloc(2*sizeof(int));//creo las tuberias en tipo matriz
            
            if(pipe(fd[i])<0){
                error("Error al crear tuberias");
            }
        }

        for(p=0;p<n_proc;p++){//creo los procesos
            hijo = fork();
            if(hijo==-1){
                error("No se pudieron crear los procesos");
            }else if(!hijo){//Entra el hijo y cierra las tuberias innecesarias
				for (int t = 0; t < num_tub; ++t)
				{
					if(p == t)
					{
						close(fd[t][1]);
					}
					else if(p+1 == t)
					{
						close(fd[t][0]);
					}
					else
					{
						close(fd[t][0]);
						close(fd[t][1]);
					}
				}
                break;
            }
        }

        if(padre==getpid()){//entra padre

		char instruccion[50] = {'\0'};
		sprintf(instruccion, "pstree -lp %d", getpid());//Muestro el arbol de procesos
    
		for (int t = 0; t < num_tub; ++t)//Cierro las tuberias inncesarias del padre
		{
			if(t == 0)
			{
				close(fd[t][0]);
			}
			else if(t == num_tub-1)
			{
				close(fd[t][1]);
			}
			else
			{
				close(fd[t][0]);
				close(fd[t][1]);
			}
		}

            while(sigo){//sigo=1 haga n veces

                system(instruccion);
                sigo = enviar_info();//envia la informacion, retorna 1 si enviara informacion y 0 si ya no hay info
                leer_info(num_tub-1);    //leo informacion
            }
        close(fd[0][1]);//cierro la tuberia 1 modo lectura
		close(fd[num_tub-1][0]);//cierro la tuberia modo escritura
        esperar_padre(n_proc);

        }else{
            while(sigo)
			{
				sigo = reenviar(p);//hace el proceso de recibir y enviar info
				system(instruccion);
				printf("PID[%d] hijo -> sigo[%d]\n", getpid(), sigo);
			}

			free(fd);
			exit(EXIT_SUCCESS);

        }
        esperar_padre(n_proc);
		free(fd);



    }else{
        error("Cantidad de procesos invalido");
    }

    return 0;
}




void generar_archivo(char *filename, int cant_palabras){
    if(cant_palabras > 0){
        char letra = ' ';
        if(archivo = fopen(filename, "w")){  //Creo y abro el archivo de texto
            for (int i = 1; i <= cant_palabras; i++){    //Genero las letras en el fichero
                letra = 32 + rand() % 92;
                if(i % 20 == 0){ //Para estetica del fichero
                    fprintf(archivo, "%c\n", letra);
                }
                else{
                    fprintf(archivo, "%c", letra);
                }
            }

            if(fclose(archivo)) //Cierro archivo
                error("No se ha podido cerrar el archivo de texto");

            printf("El archivo de texto ha sido generado exitosamente\n");    
        }
        else{
            error("No se pudo crear el archivo de texto\n");}
        }
    else{
        error("Cantidad de palabras insuficientes para crear el archivo\n");
    }
}

void esperar_padre(int n_proc){
    if(n_proc > 0 ){
        int estado = 0, proceso = 0;    //Conocer valores de terminacion del proceso
        int valor_terminado = 0;

        for (int i = 0; i < n_proc; i++){
            proceso = wait(&estado);    //Conozco el proceso que termino y su estado
            valor_terminado = WIFEXITED(estado);            
            if(valor_terminado){
                printf("El proceso [%d] ha terminado de manera correcta con valor [%d]!\n", proceso, valor_terminado);}
            else{
                printf("El proceso [%d] ha terminado de manera incorrecta con valor [%d]!\n", proceso, valor_terminado);}   
        }
    }
    else{
        error("Numero de procesos invalido");}
}

void error(char *msg){   //funcion de error
    perror(msg);
    exit(EXIT_FAILURE);
}


int enviar_info(){
	char buffer[50] = {'\0'};
	int bytes = 0;
	do
	{
		printf("PID: [%d] PADRE -> Ingrese el mensaje a enviar: \n", getpid());
		fflush(stdin);
		scanf("%[^\n]", buffer);
		bytes = strlen(buffer);
		getchar();

	}while(bytes == 0);

	if( write(fd[0][1], &bytes, sizeof(bytes)) < 0)
		error("No se pudo enviar por la tuberia");

	if( write(fd[0][1], buffer, bytes) < 0)
		error("No se pudo enviar por la tuberia");

	printf("PID: [%d] SOY PADRE -> Mensaje enviado: [%s] - Tub [0]\n", getpid(), buffer);
	if(strcmp("salir", buffer) == 0)
		return 0;

	return 1;
}

void leer_info(int n_tub){
	char buffer[50] = {'\0'};
	int bytes = 0, n = 0;
	if( ( n = read(fd[n_tub][0], &bytes, sizeof(bytes)) ) < 0)
		error("No se pudo leer de tuberia");
	if(n != 0)
	{
		if( ( n = read(fd[n_tub][0], buffer, bytes)) < 0)
			error("No se pudo leer de tuberia");
		printf("PID: [%d] -> Info recibida: [%s] - Tub: [%d]\n", getpid(), buffer, n_tub);
	}
}


int reenviar(int n_tub){
	char buffer[50] = {'\0'};
	int bytes = 0, n = 0;

	if( ( n = read(fd[n_tub][0], &bytes, sizeof(bytes)) ) < 0)
		error("No se pudo leer de tuberia");

	if( ( n = read(fd[n_tub][0], buffer, bytes)) < 0)
		error("No se pudo leer de tuberia");

	if( n != 0)
	{
		if( write(fd[n_tub+1][1], &bytes, sizeof(bytes)) < 0)
		error("No se pudo enviar por la tuberia");

		if( write(fd[n_tub+1][1], buffer, bytes) < 0)
			error("No se pudo enviar por la tuberia");

		printf("PID: [%d] -> Info recibida: [%s] - Tub: [%d]\nPID: [%d] -> Info enviada: [%s] - Tub: [%d]\n", 
		getpid(), buffer, n_tub, getpid(), buffer, n_tub+1);

		return 1;
	}
	else
	{
		close(fd[n_tub][0]);
		close(fd[n_tub+1][1]);
	}

	return 0;
}