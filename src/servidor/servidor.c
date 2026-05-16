#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    printf("Soy el servidor\n");

    //Crear Semaforo
    sem_t * mutex_general, *solicitud, *respuesta;

    //Abrir semaforos generales

    sem_unlink("/mutex_general");
    sem_unlink("/solicitud");
    sem_unlink("/respuesta");

    mutex_general = sem_open("/mutex_general", O_CREAT, 0666, 1);
    solicitud = sem_open("/solicitud", O_CREAT, 0666, 0);
    respuesta = sem_open("/respuesta", O_CREAT, 0666, 0);


    //Iniciar escucha de peticiones
    while(1){
        printf("A esperar clientes\n");
        sem_wait(solicitud);
        printf("Llego cliente:\n");
        printf("Lo atiendo...\n");
        sleep(3);
        printf("Servidor mando respuesta...\n");
        sleep(3);
        sem_post(respuesta);

    }
    return 0;
}
