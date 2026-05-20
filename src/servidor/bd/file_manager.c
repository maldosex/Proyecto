#include <stdio.h>
#include <stdlib.h>



char * get_data(const char * filename){
    FILE *file = fopen(filename, "r");
    
    if(file == NULL){
        return(NULL);
        printf("Error al encontrar el archivo\n");
    }
    

    //Llevar a la posicion final el archivo
    fseek(file, 0, SEEK_END);

    //Obtener el tamaño en numero de posiciones;
    long file_size = ftell(file);

    //Regresar la  posicion al inicio
    rewind(file);

    //Asignar memoria del tamaño del archivo
    char * buffer = (char*)malloc(file_size + 1);
    //Si fallla la signacion
    if(buffer == NULL){
        fclose(file);
        return NULL;
    }

    //Leer el archivo completo
    fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0';

    //Cerrar el archivo
    fclose(file);

    return buffer;
}