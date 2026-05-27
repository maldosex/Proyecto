#include "auth_handler.h"
#include "bd/db.h"
#include <stdbool.h>


int handle_log(cliente_contexto * cliente_ctx, Usuario_t usuario_login){

    Usuario_t usuario_db;

    int existe = db_usuarios_get_usuario_by_username(&usuario_db, usuario_login.username);

    int contra_correcta = 0;

    if(existe){

        contra_correcta = (strcmp(usuario_db.contra, usuario_login.contra) == 0);
    }

    if(!existe || !contra_correcta){

        cliente_ctx->shm->respuesta = crear_respuesta(1, "usuario o contrasena incorrectos", NULL);

        return 1;
    }

    cJSON *usuario_json = usuario_to_json(usuario_db);
    char *usuario_str = cJSON_PrintUnformatted(usuario_json);
    cliente_ctx->usuario_id = usuario_db.id;
    cliente_ctx->autenticado =1;
    cliente_ctx->shm->respuesta = crear_respuesta(0,"login correcto",usuario_str);

    free(usuario_str);

    cJSON_Delete(usuario_json);

    return 0;
}

int handle_reg(Usuario_t usuario_a_registrar, Respuesta_t *respuesta){
    //Se intenta el registro con la base de datos
    int registro_estatus = db_user_register(usuario_a_registrar);
    char msg[50];

    //Estatus exitoso
    if(registro_estatus == 0){
        strcpy(msg, "Registro exitoso, haga login");
    }

    //El registro no fue posible, ya existe el usuario
    else if (registro_estatus == 1){
        strcpy(msg, "Registro incorrecto, el usuario ya existe");
    }


    *respuesta = crear_respuesta(registro_estatus, msg, NULL);
    return registro_estatus;

}








