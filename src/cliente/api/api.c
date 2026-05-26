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



int api_register(shm_privada *shm_p, Register_data data, char *msg){

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


int api_get_habits(shm_privada *shm_p, Habito * habitos, int *count){

    cJSON * req = cJSON_CreateObject();

    cJSON_AddStringToObject(req, "action", "get_habits");
    char * str_req = cJSON_PrintUnformatted(req);
    snprintf(shm_p->solicitud,sizeof(shm_p->solicitud),"%s", str_req);
    sem_post(&shm_p->solicitud_lista);


    sem_wait(&shm_p->respuesta_lista);
    cJSON * json_respuesta = cJSON_Parse(shm_p->respuesta);
    int estatus = (cJSON_GetObjectItem(json_respuesta, "estatus"))->valueint;
    cJSON * res_habitos = cJSON_GetObjectItem(json_respuesta, "habitos");
    
    cJSON *habito_json = NULL;
    int i = 0;

    cJSON_ArrayForEach(habito_json, res_habitos){

        habitos[i] = json_to_habito(habito_json);

        i++;
    }

    *count = i;

    cJSON_Delete(json_respuesta);

    free(str_req);
    return estatus;

}

int api_add_user_habits(shm_privada * shm_p, Habito * habitos, int count){
    cJSON * req = cJSON_CreateObject();

     if(req == NULL){
        return -1;
    }

    //Se agregan los campos a la peticion
    cJSON_AddStringToObject(req, "action", "add_user_habit");

    cJSON * habits_array = cJSON_CreateArray();

    if(habits_array == NULL){
        cJSON_Delete(req);
        return -1;
    }


     for(int i = 0; i < count; i++){

        cJSON *habit_obj = cJSON_CreateObject();

        if(habit_obj == NULL){
            cJSON_Delete(req);
            return -1;
        }

        cJSON_AddNumberToObject(habit_obj, "id", habitos[i].id);

        cJSON_AddItemToArray(habits_array, habit_obj);
    }
    
    cJSON_AddItemToObject(req, "habits", habits_array);

    //La peticioon se convierte en string
    char  * req_str = cJSON_PrintUnformatted(req);

    if (req_str == NULL){
        cJSON_Delete(req);
        return -1;
    }

    //Se hace la comunicacion con el servidor 
        //Se escribe la peticion en la memoria compartida
        snprintf(shm_p->solicitud, sizeof(shm_p->solicitud), "%s",req_str);
        //Se avisa al servidor que la solicitud esta lista despertandolo
        sem_post(&shm_p->solicitud_lista);
        //Se espera la respuesta del servidor
        sem_wait(&shm_p->respuesta_lista);

    //Se procesa la respuesta del sertvidor 
    cJSON * respuesta_json = cJSON_Parse(shm_p->respuesta);
    int estatus = (cJSON_GetObjectItem(respuesta_json, "estatus"))->valueint;
    free(req_str);
    return estatus;
}