#include "db.h"

static cJSON * db_auth_json = NULL;

static pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;

int db_init(char *filename, char *db_name){

    if(db_name == NULL){
        return 1;
    }

    cJSON *root = cJSON_Parse(get_data(filename));

    if(root == NULL){
        return 2;
    }

    if(!strcmp(db_name, "db_auth")){

        cJSON *usuarios =
            cJSON_GetObjectItemCaseSensitive(root, "usuarios");

        if(usuarios == NULL){
            cJSON_Delete(root);
            return 3;
        }

        db_auth_json = cJSON_Duplicate(usuarios, 1);
    }

    cJSON_Delete(root);

    return 0;
}


cJSON * db_get_auth_info(){
    pthread_mutex_lock(&db_mutex);

    cJSON *copy =cJSON_Duplicate(db_auth_json, 1);
    
    pthread_mutex_unlock(&db_mutex);
    return copy;
}


int db_register_user(cJSON *user_data){

    cJSON * usuario = NULL;
    usuario = cJSON_GetObjectItem(user_data, "usuario");
    if(db_user_exist(usuario->valuestring) ==1){
        return 1;
    }
    else{
    pthread_mutex_lock(&db_mutex);
    cJSON_AddItemToArray(db_auth_json, user_data);
    cJSON *root = cJSON_CreateObject();

    cJSON_AddItemToObject(root,
                          "usuarios",
                          cJSON_Duplicate(db_auth_json, 1));
    
    char *json_str = cJSON_Print(root);
    
    file_db_save("src/servidor/datos.json", json_str);
    
    free(json_str);
    cJSON_Delete(root);
    pthread_mutex_unlock(&db_mutex);
    }
    return 0;

}

static int db_user_exist(const char *usuario){

    if(usuario == NULL || db_auth_json == NULL){
        return -1;
    }

    cJSON *user = NULL;

    cJSON_ArrayForEach(user, db_auth_json){

        cJSON *json_user =
            cJSON_GetObjectItemCaseSensitive(user, "usuario");

        if(cJSON_IsString(json_user) &&
           json_user->valuestring != NULL){

            if(strcmp(json_user->valuestring, usuario) == 0){
                return 1;
            }
        }
    }

    return 0;
}