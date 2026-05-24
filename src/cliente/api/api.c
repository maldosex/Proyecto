#include "api.h"


int api_login(shm_privada *shm_p, char * usuario, char * contra, char *msg){

    cJSON * req = cJSON_CreateObject();

    cJSON_AddStringToObject(req, "action", "login");

    cJSON_AddStringToObject(req, "usuario", usuario);

    cJSON_AddStringToObject(req, "contra", contra);

    char * str_req = cJSON_PrintUnformatted(req);

    snprintf(shm_p->solicitud,sizeof(shm_p->solicitud),"%s", str_req);
    sem_post(&shm_p->solicitud_lista);
    sem_wait(&shm_p->respuesta_lista);
    cJSON * json_respuesta = cJSON_Parse(shm_p->respuesta);
    int estatus = (cJSON_GetObjectItem(json_respuesta, "estatus"))->valueint;
    strcpy(msg, "Login invalido");
    free(str_req);
    return estatus;
}



int api_register(shm_privada *shm_p, Register_data data){

    cJSON * req = cJSON_CreateObject();

    cJSON_AddStringToObject(req, "action", "register");

    cJSON_AddStringToObject(req, "usuario", data.usuario);

    cJSON_AddStringToObject(req, "contra", data.contra);
    cJSON_AddStringToObject(req, "nombre", data.nombre);

    char * str_req = cJSON_PrintUnformatted(req);

    snprintf(shm_p->solicitud,sizeof(shm_p->solicitud),"%s", str_req);
    sem_post(&shm_p->solicitud_lista);
    sem_wait(&shm_p->respuesta_lista);
    cJSON * json_respuesta = cJSON_Parse(shm_p->respuesta);
    int estatus = (cJSON_GetObjectItem(json_respuesta, "estatus"))->valueint;
    free(str_req);
    return estatus;
}
