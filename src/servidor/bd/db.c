#include "db.h"

static cJSON * db_usuarios_json = NULL;
static cJSON * db_auth_json = NULL;

static cJSON *database = NULL;

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

