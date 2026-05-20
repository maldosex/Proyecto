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
#include<string.h>
#include "../../include/cJSON.h"
#include "api/api.h"
#include "gui/form_login.h"
#include "menu.h"
#include "gui/hp.h"

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
    sem_post(solicitud);
    printf("Esperando respuesta\n");
    sem_wait(respuesta);


    char name_shmem[64];
    sprintf(name_shmem,"/shm_%d",shm_g->pid);



    printf("Mando mi solicitud...\n");


    printf("Respuesta recibida\n");
    sem_post(mutex_general);

    int shmp_fd = shm_open(name_shmem, O_CREAT|O_RDWR, 0666);        
    shm_privada * shm_p = mmap(NULL, sizeof(shm_privada), PROT_READ|PROT_WRITE, MAP_SHARED, shmp_fd, 0);
    int i = 0;
    char str_usuario[100];
    char str_contra[100];

    cJSON * json_req = cJSON_CreateObject();

    while (1){


        if(log_menu() == 0){
            form_login(str_usuario, str_contra);
        }

        cJSON*  req = get_login_req(str_usuario, str_contra);
        cJSON * res = cJSON_CreateObject();
        char * json_str = cJSON_PrintUnformatted(req);

        snprintf(shm_p->solicitud,sizeof(shm_p->solicitud),"%s",json_str);

        free(json_str);

        strcpy(shm_p->solicitud, cJSON_Print(req));
        sem_post(&shm_p->solicitud_lista);
        sem_wait(&shm_p->respuesta_lista);
        printf("%d Server dice: %s\n", i, shm_p->respuesta);
        cJSON * json_respuesta = cJSON_Parse(shm_p->respuesta);
        int estatus = (cJSON_GetObjectItem(json_respuesta, "estatus"))->valueint;
        printf("El estatus es: %d\n", estatus);
        if(estatus == 0){
            printf("Bienvenido login\n");
            hp_menu();
        }
        else{
            printf("Login fallido\n");
        }
        getchar();
        i++;
    }

    cJSON_Delete(json_req);

    return 0;


    //log_menu();

}