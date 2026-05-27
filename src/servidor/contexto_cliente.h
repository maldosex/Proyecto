#ifndef CONTEXTO_CLIENTE_H
#define CONTEXTO_CLIENTE_H

#include <semaphore.h>
#include "../../modelo/modelo.h"

typedef struct{
    sem_t solicitud_lista;
    sem_t respuesta_lista;
    Solicitud_t solicitud;
    Respuesta_t respuesta;
} shm_privada;

typedef struct{

    int usuario_id;
    int autenticado;

    shm_privada *shm;

} cliente_contexto;

#endif