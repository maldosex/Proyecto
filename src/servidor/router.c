#include "router.h"




int route_request(cJSON *request, cJSON *response){


    cJSON *action = NULL;

    //Extraer la accion del request
    action = cJSON_GetObjectItem(request, "action");

    char* string_action = action->valuestring;

    //Se valida que no este vacio y sea estring la accion
    if(cJSON_IsString(action) && (action->valuestring !=NULL)){
        
        printf("El cliente solicita: %s\n", string_action);

        //Se dirige de acuerdo a la ccion solicitada
        if(!strcmp(string_action, "login")){

            cJSON_DeleteItemFromObject(request, "action");
            printf("Recibo login\n");

            handle_login(request, response);
            return 0;

        }
        return 1;

    }
    return-1;

}