#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>

#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include "bd/db.h"
#include "auth_handler.h"
#include "router.h"
#include "bd/file_manager.h"
#include "../../include/cJSON.h"
#include "router.h"


typedef struct
{
    pid_t pid;

}shm_general;

typedef struct{
    sem_t solicitud_lista;
    sem_t respuesta_lista;
    char solicitud[1024];
    char respuesta[1024];
}shm_privada;



void * atender_cliente(void * shmem);

int main(){
    printf("Soy el servidor\n");

    //Crear Semaforo
    sem_t * mutex_general, *solicitud, *respuesta;


    //Borrar semaforos preexistentes
    sem_unlink("/mutex_general");
    sem_unlink("/solicitud");
    sem_unlink("/respuesta");


    //Abrir semaforos generales
    mutex_general = sem_open("/mutex_general", O_CREAT, 0666, 1);
    solicitud = sem_open("/solicitud", O_CREAT, 0666, 0);
    respuesta = sem_open("/respuesta", O_CREAT, 0666, 0);

    //Memoria compartida general escucha servidor
    int shm_fd = shm_open("/shm_general", O_CREAT|O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shm_general));
    shm_general * shm_g = mmap(NULL, sizeof(shm_general), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

    db_init("src/servidor/datos.json", "db_auth");

    printf("%s\n", get_data("src/servidor/datos.json"));



    //Iniciar escucha de peticiones
    while(1){
        printf("A esperar clientes\n");
        sem_wait(solicitud);
        printf("Llego cliente: %d\n", shm_g->pid);
        printf("Lo atiendo...\n");
        
        //Crear memoria privada

        char name_shmem[64];
        sprintf(name_shmem,"/shm_%d",shm_g->pid);


        int shmp_fd = shm_open(name_shmem, O_CREAT|O_RDWR, 0666);
        ftruncate(shmp_fd, sizeof(shm_privada));
        shm_privada * shm_p = mmap(NULL, sizeof(shm_privada), PROT_READ|PROT_WRITE, MAP_SHARED, shmp_fd, 0);

        sem_init(&shm_p->respuesta_lista, 1, 0);
        sem_init(&shm_p->solicitud_lista, 1, 0);

        //Crear hilo para el cliente
        pthread_t thread;
        pthread_create(&thread, NULL, atender_cliente, shm_p);
        sem_post(respuesta);
        
        
    }
    return 0;
}

void *atender_cliente(void * shmem){
    pthread_detach(pthread_self());
    printf("Hilo para cliente\n");

    shm_privada *shm_p = (shm_privada *)shmem;

    int i = 0;
    while (1)
    {
        sem_wait(&shm_p->solicitud_lista);
        printf("Cliente dice: %s\n", shm_p->solicitud);
        cJSON *request = cJSON_Parse(shm_p->solicitud);
        cJSON * response = cJSON_CreateObject();
        route_request(request, response);
        printf ("La respuesta es %s\n", cJSON_Print(response));
        char * resp_str = cJSON_PrintUnformatted(response);
        strcpy(shm_p->respuesta,resp_str);
        

        sem_post(&shm_p->respuesta_lista);
    }
    return NULL;
}
