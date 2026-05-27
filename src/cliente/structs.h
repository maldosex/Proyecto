#ifndef STRUCTS_H
#define STRUCTS_H

#define OPTION_SALIR -3

#include <semaphore.h>

#include "../../modelo/modelo.h"


typedef struct{
    sem_t solicitud_lista;
    sem_t respuesta_lista;
    Solicitud_t solicitud;
    Respuesta_t respuesta;
}shm_privada;



typedef struct {
    char usuario[50];
    char contra[50];
    char nombre[50];
} Habits_data;


typedef enum {
    SCREEN_LOGIN_MENU,
    SCREEN_LOGIN,
    SCREEN_REGISTER,
    SCREEN_HOME,
    SCREEN_HABITS,
    SCREEN_EXIT,
    SCREEN_MY_HABITS
} Screen;
#endif