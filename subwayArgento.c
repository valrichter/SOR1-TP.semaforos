#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>
#include <pthread.h>    
#include <semaphore.h>  
#include <unistd.h>
#include <stdbool.h>

#define LIMITE 50

struct semaforos {
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_agregarCarne;
	sem_t sem_empanar;
	sem_t sem_fritar;
	sem_t sem_requisito1;
	sem_t sem_requisito2;
	sem_t sem_requisito3;
};

/*Seteo de semaforos compartidos*/
pthread_mutex_t sem_salero;
pthread_mutex_t sem_sarten;
sem_t sem_horno;

struct paso {
	char accion [LIMITE];
	char ingredientes[4][LIMITE];
   
};


struct parametro {
	int equipo_param;
	struct semaforos semaforos_param;
	struct paso pasos_param[9];
};


void* imprimirAccion(void *data, char *accionIn) {

	FILE * salida = fopen("salida.txt", "a");/*se abre o se crea el archivo .txt donde se escribira la salida del programa*/
	
	struct parametro *mydata = data;
	
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	
	int i;
	for(i = 0; i < sizeArray; i ++){
		
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		
		/*salida del txt*/
		fputs("Equipo ", salida);	
		fprintf(salida, "%d", mydata->equipo_param);/*escribimos en el archivo el nro del equipo*/
		fputs(" - accion ", salida);
		fprintf(salida, "%s",mydata->pasos_param[i].accion);/*escribimos en el archivo la accion del equipo*/
		fputs("\n", salida);
		/*salida del txt*/
		
		
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
		
		/*salida del txt*/
		fputs("Equipo ", salida);
		fprintf(salida, "%d", mydata->equipo_param);
		fputs(" -----------ingredientes : ---------- \n", salida);
		/*salida del txt*/
		
			
			for(h = 0; h < sizeArrayIngredientes; h++) {
				
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
					printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
							
					/*salida del txt*/
					fputs("Equipo ", salida);
					fprintf(salida, "%d", mydata->equipo_param);
					fputs(" ingrediente  ", salida);
					fprintf(salida, "%d", h);
					fputs(" : ", salida);
					fprintf(salida, "%s", mydata->pasos_param[i].ingredientes[h]);
					fputs("\n", salida);
					/*salida del txt*/
						
				}
			}
		}
	}
	fclose(salida);/*cerramos el txt al terminar el for para no tener problemas*/
}


void* cortar(void *data) {
	
	char *accion = "cortar";
	
	struct parametro *mydata = data;
	
	imprimirAccion(mydata,accion);
	
	usleep( 10000 );
	
	/*damos senial para que pueda mesclar*/
    	sem_post(&mydata->semaforos_param.sem_mezclar);
	
	pthread_exit(NULL);
}

void* mezclar(void *data) {
	
	char *accion = "mezclar";

	struct parametro *mydata = data;

	/*se fija si puede mezclar*/
	sem_wait(&mydata->semaforos_param.sem_mezclar);
		
	imprimirAccion(mydata,accion);

	usleep( 20000 );

	/*damos senial para que pueda salar*/
	sem_post(&mydata->semaforos_param.sem_salar);

	pthread_exit(NULL);
}


void* salar(void *data) {
	
	char *accion = "salar";

	struct parametro *mydata = data;

	/*se fija si puede salar*/
	sem_wait(&mydata->semaforos_param.sem_salar);

	/*seccion critica, entra solo si el salero esta disponible*/
	pthread_mutex_lock(&sem_salero);

	imprimirAccion(mydata,accion);

	usleep( 3000000 );

	pthread_mutex_unlock(&sem_salero);
	/*fin de la seccion critica*/

	/*damos senial para que pueda agregar la carne*/
	sem_post(&mydata->semaforos_param.sem_agregarCarne);

	pthread_exit(NULL);
}

void* agregarCarne(void *data) {
	
	char *accion = "agregarCarne";

	struct parametro *mydata = data;
	
	/*se fija si puede agregar la carne*/
	sem_wait(&mydata->semaforos_param.sem_agregarCarne);
		
	imprimirAccion(mydata,accion);

	usleep( 10000 );

	/*damos senial para que pueda empanar*/
	sem_post(&mydata->semaforos_param.sem_empanar);

	pthread_exit(NULL);
}

void* empanar(void *data) {
	
	char *accion = "empanar";

	struct parametro *mydata = data;

	/*se fija si puede empanar*/
	sem_wait(&mydata->semaforos_param.sem_empanar);
		
	imprimirAccion(mydata,accion);

	usleep( 10000 );

	/*damos senial para que pueda fritar*/
	sem_post(&mydata->semaforos_param.sem_fritar);

	pthread_exit(NULL);
}

void* fritar(void *data) {
	
	char *accion = "fritar";

	struct parametro *mydata = data;

	/*se fija si puede fritar*/
	sem_wait(&mydata->semaforos_param.sem_fritar);

	/*seccion critica, estra solo si la sarten esta disponible*/
	pthread_mutex_lock(&sem_sarten);
		
	imprimirAccion(mydata,accion);

	usleep( 5000000 );

	pthread_mutex_unlock(&sem_sarten);
	/*fin de la seccion critica*/
		
	/*damos senial de que el requisito1 (hacer la milanesa) se completo*/
	sem_post(&mydata->semaforos_param.sem_requisito1);

	pthread_exit(NULL);
}

void* hornear(void *data) {
	
	char *accion = "hornear";

	struct parametro *mydata = data;

	/*seccion critica, entra solo si el horno tiene espacio para panes, es este caso entrar 2 panes*/
	sem_wait(&sem_horno);
		
	imprimirAccion(mydata,accion);

	usleep( 9000000 );

	sem_post(&sem_horno);
	/*fin de la seccion critica*/
		
	/*damos senial de que el requisito2 (hornear los panes) se completo*/
	sem_post(&mydata->semaforos_param.sem_requisito2);

	pthread_exit(NULL);
}

void* cortarExtras(void *data) {
	
	char *accion = "cortarExtras";

	struct parametro *mydata = data;

	imprimirAccion(mydata,accion);

	usleep( 10000 );

	/*damos senial de que el requisito3 (cortar los vegetales) se completo*/
	sem_post(&mydata->semaforos_param.sem_requisito3);

	pthread_exit(NULL);
}

void* armado(void *data) {

	char *accion = "armado";

	struct parametro *mydata = data;

	sem_wait(&mydata->semaforos_param.sem_requisito1);
	sem_wait(&mydata->semaforos_param.sem_requisito2);
	sem_wait(&mydata->semaforos_param.sem_requisito3);
		
	imprimirAccion(mydata,accion);

	usleep( 3000000 );
		
	printf("\tEquipo %d - GANO! \n " , mydata->equipo_param);

	/*salida del txt*/
	FILE * salida = fopen("salida.txt", "a");
	fputs("Equipo ", salida);
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - GANO! ", salida);
	fputs("\n---------------- FIN ---------------\n", salida);
	fclose(salida);
	/*salida del txt*/

	exit(1); /*salimos del programa*/
	pthread_exit(NULL);
 
}

void* ejecutarReceta(void *i) {
	
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_agregarCarne;
	sem_t sem_empanar;
	sem_t sem_fritar;
	sem_t sem_requisito1;
	sem_t sem_requisito2;
	sem_t sem_requisito3;


	pthread_t p1; 
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t pHorneado;
	pthread_t pCortarExtras;
	pthread_t pArmado;
	
	
	int p = *((int *) i);
	
	
	printf("Ejecutando equipo %d \n", p);


	struct parametro *pthread_data = malloc(sizeof(struct parametro));


	pthread_data->equipo_param = p;


	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_salar = sem_salar;
	pthread_data->semaforos_param.sem_agregarCarne = sem_agregarCarne;
	pthread_data->semaforos_param.sem_empanar = sem_empanar;
	pthread_data->semaforos_param.sem_fritar = sem_fritar;
	pthread_data->semaforos_param.sem_requisito1 = sem_requisito1;
	pthread_data->semaforos_param.sem_requisito2 = sem_requisito2;
	pthread_data->semaforos_param.sem_requisito3 = sem_requisito3;
	
	
	FILE * receta = fopen("receta.txt", "r"); /*abrimos la receta para leerla*/
		
		int a=0;
		int b=0;
		bool esAccion=true;
		
	    	char texto[50];
		
		while(feof(receta) == 0){ /*recorremos todo el archivo*/
			fscanf(receta, "%s", texto); /*con fscanf agarramos una linea hasta \n y la insertamos en la variable texto*/
			
			for (char *palabra = strtok(texto, "|"); palabra; palabra = strtok(NULL, "|")){ /*spliteamos texto por "|" para separar las palabras*/
				
				if(esAccion==true){ /*sabemos que la primera palabra es siempre una accion*/
					strcpy(pthread_data->pasos_param[a].accion, palabra); /*insertamos la palabra*/
					esAccion=false; /*ponemos la variable en false ya que las demas palabras son ingredientes*/
					
				}else{
					strcpy(pthread_data->pasos_param[a].ingredientes[b], palabra);
					b++;/*incrementamos para poder poner el seiguiente ingrediente*/
				}
			}

			a++; /*aumentamos los pasos*/
			b=0; /*reseteamos los ingredientes*/
			esAccion=true; /*reseteamos el valor de esAccion ya que vamos a recorrer el sigueinte paso*/
		}
		fclose(receta);


	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_agregarCarne),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_fritar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_requisito1),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_requisito2),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_requisito3),0,0);


	int rc;
	rc = pthread_create(&p1,                           
		            NULL,                      
		                cortar,             
		                pthread_data);                    

	rc = pthread_create(&p2,                           
		            NULL,                      
		                mezclar,             
		                pthread_data);       
		                
	rc = pthread_create(&p3,                           
		            NULL,                      
		                salar,             
		                pthread_data);   
		                
	rc = pthread_create(&p4,                           
		            NULL,                      
		                agregarCarne,             
		                pthread_data);   
		                                  
	rc = pthread_create(&p5,                           
		            NULL,                      
		                empanar,             
		                pthread_data); 
		                 
	rc = pthread_create(&p6,                           
		            NULL,                      
		                fritar,             
		                pthread_data);  
		                
	rc = pthread_create(&pHorneado,                           
		            NULL,                      
		                hornear,             
		                pthread_data); 
		                
	rc = pthread_create(&pCortarExtras,                           
		            NULL,                      
		                cortarExtras,             
		                pthread_data);   
		                                              
	rc = pthread_create(&pArmado,                           
		            NULL,                      
		                armado,             
		                pthread_data);  
		                          
		                                                                               	
	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (pHorneado,NULL);
	pthread_join (pCortarExtras,NULL);
	pthread_join (pArmado,NULL);
	
	
	if (rc){
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	}

	
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	sem_destroy(&sem_agregarCarne);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_fritar);
	sem_destroy(&sem_requisito1);
	sem_destroy(&sem_requisito2);
	sem_destroy(&sem_requisito3);
	
	pthread_exit(NULL);
}



int main ()
{
	
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));

  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;


	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3; 
	pthread_t equipo4;

  	/*inicializo los semaforos compartidos*/
	pthread_mutex_init(&sem_salero, NULL); /*salero (mutex porque solo se puede acceder de a uno por vez)*/
	pthread_mutex_init(&sem_sarten, NULL); /*sarten (mutex)*/
	sem_init(&sem_horno, 0, 2); /*horno (contador inicializado en 2 ya que tiene capacidad para dos panes)*/
	
	
	rc = pthread_create(&equipo1,                          
		            NULL,                          
		                ejecutarReceta,            
		                equipoNombre1); 

	rc = pthread_create(&equipo2,                         
		            NULL,                          
		                ejecutarReceta,           
		                equipoNombre2);
		                
	rc = pthread_create(&equipo3,                          
		            NULL,                          
		                ejecutarReceta,            
		                equipoNombre3); 

	rc = pthread_create(&equipo4,                         
		            NULL,                          
		                ejecutarReceta,           
		                equipoNombre4);


	if (rc){
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	} 


	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);

	/*destruimos los semaforos compartidos*/
	pthread_mutex_destroy(&sem_salero);
	pthread_mutex_destroy(&sem_sarten);
	sem_destroy(&sem_horno);
	
	pthread_exit(NULL);
}


/* Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread */
/* Para ejecutar:   ./ejecutable */
