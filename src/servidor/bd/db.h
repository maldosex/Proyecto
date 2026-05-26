#include <string.h>
#include "../../../include/cJSON.h"
#include "file_manager.h"
#include <pthread.h>
#include <stdlib.h>
#include "../../../modelo/modelo.h"
#include <stdio.h>

#ifndef DB_H
#define DB_H

//estructuras de bases de datos con el mutex

typedef struct{
    Usuario_t usuarios[50];

    int count;

    pthread_mutex_t mutex;
}db_usuarios_t;

typedef struct{

    Habito habitos[50];

    int count;

    pthread_mutex_t mutex;

}db_habit_t;

typedef struct{
    UsuarioHabito usuariohabitos[100];
    int count;

    pthread_mutex_t mutex;
    
}db_usuariohabito_t;

int db_usuarios_init(const char *filename);
int db_usuarios_get_usuario_by_username( Usuario_t *usuario, const char*username);

int db_habitos_init(const char* filename);
int db_habits_get(Habito * habitos, int * count);

int db_init(char*filename, char * db_name);
int db_register_user(cJSON *user_data);
static int db_user_exist(const char *usuario);

cJSON * db_get_auth_info();

#endif