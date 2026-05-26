#include "modelo.h"

Habito json_to_habito(cJSON * json){
    Habito habito;
    cJSON * id_json = cJSON_GetObjectItem(json, "id");
    cJSON * nombre_json = cJSON_GetObjectItem(json, "nombre");

    strcpy(habito.nombre, nombre_json->valuestring);
    habito.id = id_json->valueint;
    return  habito;
}

cJSON * habito_to_json(Habito habito){
    cJSON * habito_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(habito_json, "id", habito.id);
    cJSON_AddStringToObject(habito_json, "nombre", habito.nombre);

    return habito_json;
}

UsuarioHabito json_to_usuariohabito(cJSON * json){
    UsuarioHabito usuariohabito;
    cJSON * id_json = cJSON_GetObjectItem(json, "id");
    cJSON * usuario_id_json = cJSON_GetObjectItem(json, "usuario_id");
    cJSON * habito_id_json = cJSON_GetObjectItem(json, "habito_id");
    cJSON * activo = cJSON_GetObjectItem(json, "activo");

    usuariohabito.id = id_json->valueint;
    usuariohabito.usuario_id = usuario_id_json->valueint;
    usuariohabito.habito_id = habito_id_json->valueint;
    usuariohabito.activo = activo->valueint;
    
}


cJSON * usuariohabito_to_json(UsuarioHabito usuariohabito){
    cJSON * usuariohabito_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(usuariohabito_json, "id", usuariohabito.id);
    cJSON_AddNumberToObject(usuariohabito_json, "usuario_id", usuariohabito.usuario_id);
    cJSON_AddNumberToObject(usuariohabito_json, "habito_id", usuariohabito.habito_id);

    return usuariohabito_json;
}


Usuario_t json_to_usuario(cJSON * usuario_json){
    Usuario_t usuario;

    cJSON * id_json = cJSON_GetObjectItem(usuario_json, "id");
    cJSON * username_json = cJSON_GetObjectItem(usuario_json, "usuario");
    cJSON * contra_json = cJSON_GetObjectItem(usuario_json, "contra");

    usuario.id = id_json->valueint;
    strcpy(usuario.username, username_json->valuestring);
    strcpy(usuario.contra, contra_json->valuestring);
    return usuario;
}
cJSON * usuario_to_json(Usuario_t usuario){
    cJSON * usuario_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(usuario_json, "id", usuario.id);
    cJSON_AddStringToObject(usuario_json, "usuario", usuario.username);

    return usuario_json;
}


Respuesta_t crear_respuesta(int estatus, const char *msg, cJSON *data){
    Respuesta_t respuesta;

    respuesta.estatus = estatus;
    respuesta.msg = msg;
    respuesta.data = data;

    return respuesta;
}

void respuesta_to_json(Respuesta_t respuesta, cJSON * respuesta_json){


    cJSON_AddNumberToObject(respuesta_json,"estatus",respuesta.estatus);

    cJSON_AddStringToObject(respuesta_json,"msg",respuesta.msg);

    if(respuesta.data != NULL){

        cJSON_AddItemToObject(respuesta_json,"data",respuesta.data);
    }
}