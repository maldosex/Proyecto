#include "ncurses.h"
#include "gui/menu.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "../../include/cJSON.h"
#include "api/api.h"
#include "gui/form_login.h"
#include "gui/form_register.h"
#include "gui/menu_available_habits.h"
#include "gui/men_myhabits.h"
#include "menu.h"
#include "gui/hp.h"

#include "structs.h"

typedef struct
{
    pid_t pid;

}shm_general;





int main(){
    sem_t * mutex_general, *solicitud, *respuesta;


    //Abrir los semaforos  generales:

    mutex_general = sem_open("/mutex_general", 0);
    solicitud = sem_open("/solicitud", 0);
    respuesta = sem_open("/respuesta", 0);


    //Mandar solicitud

    printf("Cieroo las solicitudes\n");
    sem_wait(mutex_general);

    //Acceder a shm_general
    printf("Soy el proceso %d\n,", getpid());

    int shm_fd = shm_open("/shm_general", O_RDWR, 0666);
    shm_general * shm_g = mmap(NULL, sizeof(shm_general), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

    shm_g->pid = getpid();
    sem_post(solicitud);
    printf("Esperando respuesta\n");
    sem_wait(respuesta);


    char name_shmem[64];
    sprintf(name_shmem,"/shm_%d",shm_g->pid);



    printf("Mando mi solicitud...\n");


    printf("Respuesta recibida\n");
    sem_post(mutex_general);

    int shmp_fd = shm_open(name_shmem, O_CREAT|O_RDWR, 0666);        
    shm_privada * shm_p = mmap(NULL, sizeof(shm_privada), PROT_READ|PROT_WRITE, MAP_SHARED, shmp_fd, 0);
    int i = 0;
    char str_usuario[100];
    char str_contra[100];
    

    initscr();
    start_color();
	curs_set(1);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    Screen current = SCREEN_LOGIN_MENU;

    while (current != SCREEN_EXIT) {

        switch(current) {

            case SCREEN_LOGIN_MENU: {
                int op = log_menu();

                if (op == 0)
                    current = SCREEN_LOGIN;
                else if (op == 1)
                    current = SCREEN_REGISTER;
                else
                    current = SCREEN_EXIT;

                break;
            }

            case SCREEN_LOGIN: {

                int status = form_login(shm_p, str_usuario, str_contra);

                if (status == 1)
                    current = SCREEN_HOME;
                else
                    current = SCREEN_LOGIN_MENU;

                break;
            }

            case SCREEN_REGISTER: {

                int op = form_register(shm_p);

                if (op == -1)
                    current = SCREEN_HOME;
                else
                    current = SCREEN_LOGIN_MENU;

                break;
            }

            case SCREEN_HOME: {

                int op = hp_menu();

                if (op == 0)
                    current = SCREEN_HABITS;

                else if(op == 1)
                    current = SCREEN_MY_HABITS;

                else
                    current = SCREEN_LOGIN_MENU;

                break;
            }           

            case SCREEN_HABITS: {

                Habito habitos[50];
                int count = 0;
                int status = api_get_all_habits(shm_p, habitos, &count);

                int habitos_ids[50];
                int selected_count = 0;

                if(status == 0){
                    int result = menu_available_habits(habitos, count, habitos_ids, &selected_count);

                    if(result == 1 && selected_count > 0){ 
                        api_register_usuariohabitos(shm_p, habitos_ids, selected_count);
                    }
                }
                else{
                
                    clear();
                
                    mvprintw(LINES / 2,(COLS - 30) / 2,"Error obteniendo habitos");
                
                    refresh();
                
                    getch();
                }

                api_register_usuariohabitos(shm_p, habitos_ids, selected_count);
            
                current = SCREEN_HOME;
            
                break;
            }

            case SCREEN_MY_HABITS: {

                Habito habitos[50];
                int count;

                api_get_user_habits(shm_p,habitos,&count);

                menu_my_habits(habitos, count,  NULL, NULL);
                current = SCREEN_HOME;
                break;
        }
    }

    }
    endwin();

    return 0;
}