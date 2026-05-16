#include "ncurses.h"
#include "gui/menu.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct
{
    pid_t pid;

}shm_general;

int main(){
    sem_t * mutex_general, *solicitud, *respuesta;


    //Abrir los semaforos  generales:

    mutex_general = sem_open("/mutex_general", 0);
    solicitud = sem_open("/solicitud", 0);
    respuesta = sem_open("/respuesta", 0);


    //Mandar solicitud

    printf("Cieroo las solicitudes\n");
    sem_wait(mutex_general);

    //Acceder a shm_general
    printf("Soy el proceso %d\n,", getpid());

    int shm_fd = shm_open("/shm_general", O_RDWR, 0666);
    shm_general * shm_g = mmap(NULL, sizeof(shm_general), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

    shm_g->pid = getpid();

    printf("Mando mi solicitud...\n");
    sleep(3);
    sem_post(solicitud);
    printf("Esperando respuesta\n");
    sem_wait(respuesta);
    printf("Respuesta recibida\n");
    sem_post(mutex_general);


    //log_menu();

}