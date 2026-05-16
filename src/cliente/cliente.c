#include "ncurses.h"
#include "gui/menu.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <pthread.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    sem_t * mutex_general, *solicitud, *respuesta;


    //Abrir los semaforos  generales:

    mutex_general = sem_open("/mutex_general", 0);
    solicitud = sem_open("/solicitud", 0);
    respuesta = sem_open("/respuesta", 0);


    //Mandar solicitud

    printf("Cieroo las solicitudes\n");
    sem_wait(mutex_general);
    printf("Soy el proceso\n");
    printf("Mando mi solicitud...\n");
    sleep(3);
    sem_post(solicitud);
    printf("Esperando respuesta\n");
    sem_wait(respuesta);
    printf("Respuesta recibida\n");
    sem_post(mutex_general);


    //log_menu();

}