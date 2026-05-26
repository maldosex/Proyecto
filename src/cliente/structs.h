#ifndef STRUCTS_H
#define STRUCTS_H


#include <semaphore.h>

#include "../../modelo/modelo.h"


typedef struct{
    sem_t solicitud_lista;
    sem_t respuesta_lista;
    char solicitud[1024];
    char respuesta[1024];
}shm_privada;

typedef struct {
    char usuario[50];
    char contra[50];
    char nombre[50];
    char apellido[50];
    char correo [50];
} Register_data;

typedef struct {
    char usuario[50];
    char contra[50];
    char nombre[50];
} Habits_data;

#endif