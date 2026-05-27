#include "habit_handler.h"


int handle_get_habits(cliente_contexto * cliente_ctx){

    Habito habitos[50];
    int count;

    db_habits_get(habitos, &count);
    cJSON * arreglo = cJSON_CreateArray();

    cliente_ctx->shm->respuesta.estatus = 0;

    for (int i = 0; i < count; i++)
    {
        cJSON_AddItemToArray(arreglo, habito_to_json(habitos[i]));

    }

    char *data = cJSON_PrintUnformatted(arreglo);

    cliente_ctx->shm->respuesta = crear_respuesta(0, "Datos obtenidos", data);
    free(data);
    cJSON_Delete(arreglo);
    return 0;
}

int handle_add_user_habits(cliente_contexto *cliente_ctx,int *ids,int count){
    for(int i = 0; i < count; i++){

        UsuarioHabito uh;

        uh.usuario_id =
            cliente_ctx->usuario_id;

        uh.habito_id = ids[i];

        uh.activo = 1;

        db_usuariohabito_insert(uh);
    }

    cliente_ctx->shm->respuesta =
        crear_respuesta(
            0,
            "habitos agregados",
            NULL
        );

    return 0;
}



int handle_get_user_habits(cliente_contexto *cliente_ctx){

    if(!cliente_ctx->autenticado){

        cliente_ctx->shm->respuesta = crear_respuesta(1,"usuario no autenticado",NULL);

        return 1;
    }

    Habito habitos[50];

    int count = 0;

    db_habitos_get_by_usuario_id(cliente_ctx->usuario_id,habitos,&count);

    cJSON *arreglo = cJSON_CreateArray();

    for(int i = 0; i < count; i++){

        cJSON_AddItemToArray(arreglo,habito_to_json(habitos[i]));
    }

    char *data = cJSON_PrintUnformatted(arreglo);

    cliente_ctx->shm->respuesta = crear_respuesta(0,"habitos obtenidos",data);

    free(data);

    cJSON_Delete(arreglo);

    return 0;
}