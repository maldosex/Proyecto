#include "db.h"

static cJSON * db_auth_json = NULL;

static pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;


//Base catalogo de habitos::
static db_usuarios_t db_usuarios;

static db_habit_t db_habitos;

static db_usuariohabito_t db_usuariohabito;

int db_usuarios_init(const char* filename){
    cJSON *usuarios_json = cJSON_Parse(get_data(filename));

    pthread_mutex_init(&db_usuarios.mutex, NULL);

    db_usuarios.count = 0;

    cJSON *usuario_json = NULL;

    cJSON_ArrayForEach(usuario_json, usuarios_json){
        int index = db_usuarios.count;

        db_usuarios.usuarios[index] = json_to_usuario(usuario_json);

        db_usuarios.count++;
    }

    cJSON_Delete(usuarios_json);
    return 0;
}

int db_usuarios_get_usuario_by_username( Usuario_t *usuario, const char*username){
    int existe = 0;
    pthread_mutex_lock(&db_usuarios.mutex);
        for(int i = 0; i<db_usuarios.count; i++){
            if(strcmp(db_usuarios.usuarios[i].username, username)==0){
                *usuario = db_usuarios.usuarios[i];
                existe = 1;
                break;
            }
        }
    pthread_mutex_unlock(&db_usuarios.mutex);
    return existe;
}



int db_habitos_init(const char* filename){
    cJSON *habitos = cJSON_Parse(get_data(filename));

    pthread_mutex_init(&db_habitos.mutex, NULL);

    db_habitos.count = 0;

    cJSON *habito = NULL;


    cJSON_ArrayForEach(habito, habitos){
        int index = db_habitos.count;

        cJSON *id_json = cJSON_GetObjectItem(habito, "id");

        cJSON *nombre_json = cJSON_GetObjectItem(habito, "nombre");

        db_habitos.habitos[index].id = id_json->valueint;

        strncpy(db_habitos.habitos[index].nombre, nombre_json->valuestring, sizeof(db_habitos.habitos[index].nombre));

        db_habitos.count++;
    }

    cJSON_Delete(habitos);

    return 0;
}


int db_habits_get(Habito * habitos, int  *count){

    pthread_mutex_lock(&db_habitos.mutex);

    int n = db_habitos.count;

    memcpy(habitos, db_habitos.habitos, sizeof(Habito) * n);
    *count = n;

    pthread_mutex_unlock(&db_habitos.mutex);

    return 0;
}






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

int db_usuariohabito_init(const char *filename){
    cJSON *usuariohabitos = cJSON_Parse(get_data(filename));

    pthread_mutex_init(&db_usuariohabito.mutex, NULL);

    db_usuariohabito.count = 0;

    cJSON *usuariohabito = NULL;


    cJSON_ArrayForEach(usuariohabito, usuariohabitos){
        int index = db_usuariohabito.count;

        db_usuariohabito.usuariohabitos[index] = json_to_usuariohabito(usuariohabito);

        db_usuariohabito.count++;
    }

    cJSON_Delete(usuariohabitos);

    return 0;
}

/*
int db_usuariohabito_get(UsuarioHabito * usuariohabito, int *count){
    pthread_mutex_lock(&db_usuariohabito.mutex)
}

int db_get_usuarioHabitos_by_usuario_id(UsuarioHabito * usuariohabito, int *count, ){
    pthread_mutex_lock(&db_usuariohabito.mutex)
}
    */