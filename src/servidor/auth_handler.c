#include "auth_handler.h"
#include "bd/db.h"
#include <stdbool.h>

int handle_login(cJSON *json_solicitud, cJSON *json_respuesta) {

    //extraer (json_solicitudsolicitud
    printf("En handler, request: %s\n", cJSON_Print(json_solicitud));
    cJSON *usuario_req = cJSON_GetObjectItemCaseSensitive(json_solicitud, "usuario");
    cJSON *contra_req = cJSON_GetObjectItemCaseSensitive(json_solicitud, "contra");
    printf("En handle u = %s c = %s\n", usuario_req->valuestring, contra_req->valuestring);


    // verificar la solicitud
    if (!cJSON_IsString(usuario_req) || !cJSON_IsString(contra_req)) {
        cJSON_AddNumberToObject(json_respuesta, "estatus", -1); // Error de formato
        return -1;
    }

    // obtener base de datos
    cJSON *usuarios_auth = db_get_auth_info();
    if (usuarios_auth == NULL) {
        cJSON_AddNumberToObject(json_respuesta, "estatus", -2); // Error de BD
        return -2;
    }

    cJSON *usuario_db = NULL;
    bool login_exitoso = false;

    // 3. Buscar en el arreglo con ArrayForEach
    cJSON_ArrayForEach(usuario_db, usuarios_auth) {
        cJSON *username_db = cJSON_GetObjectItemCaseSensitive(usuario_db, "usuario");
        cJSON *contra_db = cJSON_GetObjectItemCaseSensitive(usuario_db, "contra");

        if (cJSON_IsString(username_db) && cJSON_IsString(contra_db)) {
            if((strcmp(usuario_req->valuestring, username_db->valuestring))== 0 && (strcmp(contra_req->valuestring, contra_db->valuestring))==0){
                login_exitoso = true;
                
                //Armar la respuesta
              

                cJSON_AddNumberToObject(json_respuesta, "estatus", 0);
                cJSON_AddItemToObject(json_respuesta, "usuario", cJSON_Duplicate(usuario_db, 1));
                
                break; // usuario encontrado
            }
        }
    }

    //no se encontro
    if (!login_exitoso) {
        cJSON_AddNumberToObject(json_respuesta, "estatus", 1); // Credenciales incorrectas
    }


    return login_exitoso ? 0 : 1;
}