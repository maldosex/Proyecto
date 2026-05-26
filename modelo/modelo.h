#ifndef MODELO_H
#define MODELO_H

#include <string.h>

#include "../include/cJSON.h"

typedef struct{
    char id;
    char nombre[50];
}Habito;

Habito json_to_habito(cJSON * json);
cJSON * habito_to_json(Habito habito);


typedef struct{
    int id;
    int habito_id;
    int usuario_id;
    int activo;
}UsuarioHabito;

UsuarioHabito json_to_usuariohabito(cJSON * json);
cJSON * usuariohabito_to_json(UsuarioHabito usuariohabito);

typedef struct{
    int id;
    char username[25];
    char contra[25];
}Usuario_t;

Usuario_t json_to_usuario(cJSON * usuario_json);
cJSON * usuario_to_json(Usuario_t usuario);

typedef struct respuesta{
    int estatus;
    const char *msg;
    cJSON *data;
}Respuesta_t;

Respuesta_t crear_respuesta(int estatus, const char *msg, cJSON *data);

void respuesta_to_json(Respuesta_t respuesta, cJSON * respuesta_json);




#endif