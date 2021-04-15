#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <string.h>
#include <stdint.h>

int mitad=0;
int filas=0;
int columnas=0;
int matriz[40][40],vector[40];
int *vectorR=NULL;
char datos[]="datos.txt";


void strreverse(char* begin, char* end) {  
    char aux;  
    while(end>begin)   
        aux=*end, *end--=*begin, *begin++=aux; 
}

char* itoa(int value, char* str, int base) {
    static char num[] = "0123456789abcdefghijklmncalcularVecRqrstuvwxyz";
    char* wstr=str;
    int sign;  
    div_t res;
    // Validate base
   
    if (base<2 || base>35){ *wstr='\0'; return NULL; }  
    // Take care of sign
   
    if ((sign=value) < 0) value = -value;
    // Conversion. Number is reversed.
   
    do {   
        res = div(value,base); 
        *wstr++ = num[res.rem];
    }while(value=res.quot);
   
    if(sign<0) *wstr++='-';
    *wstr='\0';
    // Reverse string  
    strreverse(str,wstr-1);
        return str;
}



void cargarData(){
	FILE *f=fopen(datos,"r");
	int i1=0;
	if(f==NULL){
		printf("%s no exite","datos.txt");
		exit(-1);
	}
	char cadena[40];
	fgets(cadena,40,f);
	i1=0;
	while(cadena[i1]!='\n' || cadena[i1]==' '){
		i1++;
	}
	cadena[i1]='\0';
	filas=atoi(cadena);
	for(int i=0;i<filas;i++){
		fgets(cadena,40,f);
		i1=0;
		while(cadena[i1]!='\n' || cadena[i1]==' '){
			i1++;
		}
		cadena[i1]='\0';
		vector[i]=atoi(cadena);
	}
	fgets(cadena,40,f);

	i1=0;
	while(cadena[i1]!='\n' || cadena[i1]==' '){
		i1++;
	}
	cadena[i1]='\0';
	filas=atoi(cadena);
	fgets(cadena,40,f);

	i1=0;
	while(cadena[i1]!='\n' || cadena[i1]==' '){
		i1++;
	}
	cadena[i1]='\0';
	columnas=atoi(cadena);

	for (int i = 0; i < filas; ++i)
	{
		fgets(cadena,40,f);

		i1=0;
		while(cadena[i1]!='\n' || cadena[i1]==' '){
			i1++;
		}
		cadena[i1]='\0';
		int c=0;
    	char delimitador[] = " ";
		char *token=strtok(cadena, delimitador);

    	while(token != NULL ){
	    	matriz[i][c]=atoi(token);
	    	token = strtok(NULL, delimitador);
	    	c++;
    	}

	}

	fclose(f);
}
	
	

int * calcularVecR(int index,int indexf){
	int max=indexf-index;
	int vectorR[max];
	int o1=index;
	int o2=indexf;
	int i2=index;
	int i1=0;
	for(int i=0;i<filas;i++){
		vectorR[i]=0;
	}
	
	for(int i=o1;i<o2;i++){

		for(int j=0;j<columnas;j++){
			vectorR[i1]+=matriz[i][j]*vector[i2];
		}
		i1++;
		i2++;
	}
	

	int *p=vectorR;
	return p;
}

void crearvectorR(char nombre[40],int pv[40],int i0,int i1){
	int cantidad=i1-i0;
	int vaux[i1-i0];
	
	for (int i = 0; i < i1-i0; ++i){
		vaux[i]=pv[i];
	
	}

	FILE *f=fopen(nombre,"w");
	
	if(f==NULL){
		printf("error de apertura\n");
	}
	char data[]={'0','0','0','0','0','0'};
	
	for (int i = 0; i < cantidad; ++i){

		itoa(vaux[i],data,10);

		fputs(data,f);

		fputs("\n",f);
	}
	fputs("\n",f);
	fclose(f);

}
void mostrarRenPantalla(){
	FILE *f1=fopen("result_hijo1.txt","r");
	FILE *f2=fopen("result_hijo2.txt","r");
	char d;
	if(f1!=NULL && f2!=NULL){
		char data[40];
		int cont=0;
		for (int i = 0; i < (int)filas/2; ++i){
			fgets(data,40,f1);
			int i1=0;
			while(data[i1]!='\n' || data[i1]==' '){
				i1++;
			}
			data[i1]='\0';
			printf("%d:[%s] \n",cont,data);
			cont++;
		}
		for(int i = (int)filas/2; i <filas ; ++i)
		{
			fgets(data,40,f2);
			int i1=0;
			while(data[i1]!='\n' || data[i1]==' '){
				i1++;
			}
			data[i1]='\0';
			printf("%d:[%s]\n",cont,data);
			cont++;
		}
	}else{
		
		printf("error archivos 'result_hijo1.txt o result_hijo2.txt ' no allados ");

	}
	
	fclose(f1);
	fclose(f2);
	
}
void initM(){
	for (int i = 0; i < 40; ++i){
		for (int j = 0; j < 40; ++j){
			matriz[i][j]=0;
		}
	
	}
	for (int i = 0; i < 40; ++i){
		vector[i]=0;
	}
}
void fun1(){
	vectorR=calcularVecR(0,mitad);
	int vc[40];
	for (int i = 0; i < filas; ++i){
		vc[i]=0;
	}
	
	for (int i = 0; i < mitad; ++i){
		vc[i]=vectorR[i];
	}
	crearvectorR("result_hijo1.txt",vc,0,mitad);
}
void fun2(){
	int vc[40];
	vectorR=calcularVecR(mitad,filas);
	for (int i = 0; i < filas; ++i){
		vc[i]=0;
	}
	int k=0;
	for (int i = mitad; i <filas ; ++i){
		vc[k]=vectorR[k];
		k++;
	}
	crearvectorR("result_hijo2.txt",vc,mitad,filas);
}
int main(){
	initM();
	cargarData(datos);
	mitad=(int)filas/2;
	
	if(fork() ==0){
		fun1();
		exit(1);
	}
	if(fork() ==0){
		fun2();
		exit(1);
	}
	printf("\n Operacion terminada... \n");
	sleep(1);
	mostrarRenPantalla();
}


