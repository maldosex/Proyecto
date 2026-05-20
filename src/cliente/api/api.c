#include "api.h"

cJSON * get_login_req(char * usuario, char * contra){
    printf("Estoy en api login\n");

    cJSON * req = cJSON_CreateObject();

    cJSON_AddStringToObject(req, "action", "login");

    cJSON_AddStringToObject(req, "usuario", usuario);

    cJSON_AddStringToObject(req, "contra", contra);

    return req;


}
