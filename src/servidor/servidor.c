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


typedef struct
{
    pid_t pid;

}shm_general;





void * atender_cliente();

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

    //Iniciar escucha de peticiones
    while(1){
        printf("A esperar clientes\n");
        sem_wait(solicitud);
        printf("Llego cliente: %d\n", shm_g->pid);
        printf("Lo atiendo...\n");
        
        
        sleep(3);
        printf("Servidor mando respuesta...\n");
        sleep(3);
        sem_post(respuesta);
        
        //Hilo para atender al cliente
        pthread_t thread;
        pthread_create(&thread, NULL, atender_cliente, NULL);
    }
    return 0;
}

void *atender_cliente(){
    printf("Hilo para cliente\n");
    pthread_detach(pthread_self());
    return NULL;
}
