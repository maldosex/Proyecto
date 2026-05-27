#include "router.h"




int route_request(cliente_contexto *cliente_ctx){



    printf("Accion recibida: %d\n", cliente_ctx->shm->solicitud.action);
    //Se identifica la accion que solicita el cliente
    switch(cliente_ctx->shm->solicitud.action){

        case ACTION_LOGIN:{

            cJSON * usuario_json = cJSON_Parse(cliente_ctx->shm->solicitud.data);

            //Se valida el contenido de la solicitud
            if(usuario_json == NULL){

                cliente_ctx->shm->respuesta = crear_respuesta(-1,"json invalido",NULL);

                return -1;
            }

            Usuario_t usuario_login = usuario_from_json(usuario_json);

            cJSON_Delete(usuario_json);

            return handle_log(cliente_ctx, usuario_login);
        }

        case ACTION_REGISTER:{
            printf("El cliente solicita Registrar usuario\n");
            cJSON * usuario_solicitud_json = cJSON_Parse(cliente_ctx->shm->solicitud.data);

            //Se valida el contenido de la solicitud
            if(usuario_solicitud_json == NULL){

                cliente_ctx->shm->respuesta = crear_respuesta(-1,"json invalido",NULL);
                return -1;
            }
            Usuario_t usuario_a_registrar = usuario_from_json(usuario_solicitud_json);

            cJSON_Delete(usuario_solicitud_json);

            return handle_reg(usuario_a_registrar, &cliente_ctx->shm->respuesta);
        }

        case ACTION_GET_HABITS:{
            printf("El cliente solicita obtener la lista de habitos\n");

            handle_get_habits(cliente_ctx);
            break;
        }
        case ACTION_ADD_USER_HABITS: {

            cJSON *ids_json = cJSON_Parse(cliente_ctx->shm->solicitud.data);
            
            if(ids_json == NULL){
            
                cliente_ctx->shm->respuesta =crear_respuesta(-1,"json invalido",NULL);
                return -1;
            }
        
            if(!cJSON_IsArray(ids_json)){
            
                cJSON_Delete(ids_json);
                cliente_ctx->shm->respuesta = crear_respuesta(-1,"se esperaba arreglo",NULL); 
                return -1;
            }
        
            int ids[50];
        
            int count = 0;
        
            cJSON *id_json = NULL;
        
            cJSON_ArrayForEach(id_json, ids_json){
            
                if(cJSON_IsNumber(id_json)){
                
                    ids[count] = id_json->valueint;
                
                    count++;
                }
            }
        
            cJSON_Delete(ids_json);
        
            return handle_add_user_habits(cliente_ctx, ids, count);
        }

        case ACTION_GET_USER_HABITS:{
            return handle_get_user_habits(cliente_ctx);
        }


/*
            return handle_register(solicitud,respuesta);

        case ACTION_GET_HABITS:

            printf("Recibo get habits\n");

            return get_habits(solicitud,respuesta);
            */

        default:

            cliente_ctx->shm->respuesta = crear_respuesta(-1,"accion invalida",NULL);
            return -1;
    }
}