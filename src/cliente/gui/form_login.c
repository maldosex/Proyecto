#include "form_login.h"

#include <string.h>
#include <stdlib.h>

char *trim(char *str);


int form_login(shm_privada *shm_p, char * usuario, char * contra){
    FIELD *field[3];
    FORM  *my_form;
    WINDOW *my_form_win;
    int ch, rows, cols;


    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_BLACK);

    // Definición de campos
    field[0] = new_field(1, 20, 2, 14, 0, 0); // Usuario
    field[1] = new_field(1, 20, 4, 14, 0, 0); // Contraseña
    field[2] = NULL;

    set_field_back(field[0], A_UNDERLINE);
    set_field_back(field[1], A_UNDERLINE);
    
    // Ocultar caracteres en el campo de contraseña
    field_opts_off(field[1], O_PUBLIC); 

    my_form = new_form(field);
    scale_form(my_form, &rows, &cols);

    // Crear ventana centrada
    my_form_win = newwin(10, 40, (LINES-10)/2, (COLS-40)/2);
    keypad(my_form_win, TRUE);

	wattron(my_form_win, COLOR_PAIR(3));
    box(my_form_win, 0, 0);
	wattroff(my_form_win, COLOR_PAIR(3));

    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));

    print_in_middle(my_form_win, 1, 0, 40, " LOGIN", COLOR_PAIR(1));
    
    post_form(my_form);

    
    // Etiquetas de texto
    mvwprintw(my_form_win, 4, 3, "Usuario:");
    mvwprintw(my_form_win, 6, 3, "Contrasea:");

	set_current_field(my_form, field[0]);
	pos_form_cursor(my_form);

	mvprintw(LINES-2, 2, "Presione F1 para salir");
	refresh();
    
    wrefresh(my_form_win);
    

    int respuesta_form = 0;
    char msg[50];

    Usuario_t usuario_login;
    //Se mantendra en en ciclo del formulario hasta que logra hacer login o decida salir

    while((ch = wgetch(my_form_win))) {
        switch(ch) {
            case KEY_F(1):
                respuesta_form = -1;
                goto cleanup;
                break;
			case KEY_BACKSPACE:
            case 127:
            case 8:
                // Le ordena al formulario borrar el carácter a la izquierda del cursor
                form_driver(my_form, REQ_DEL_PREV);
                break;
            case KEY_DOWN:
            case KEY_UP:
                form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;
            case 10: // Enter para aceptar
                form_driver(my_form, REQ_VALIDATION);
                strcpy(usuario_login.username, trim(field_buffer(field[0], 0)));
                strcpy(    usuario_login.contra,    trim(field_buffer(field[1], 0)));
                //Se mantendra en en ciclo del formulario hasta que logra hacer login o decida salir
                int status = api_login(shm_p,usuario_login,msg); 
                if(status == 1) {
					mvwprintw(my_form_win, 8, 2, "Usuario o contrasena incorrectos");
                    wrefresh(my_form_win);
                }
                if(status == 0){
                    respuesta_form = 1;
                    goto cleanup;
                }
                //Si la llamada a la api de login fue exitosa, se saldra del formulario para continuar con el siguiente           
                break;        
            default:            
                form_driver(my_form, ch);            
                break;    
            }
    }
    cleanup:
    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]);
    delwin(my_form_win); // Importante: eliminar ventana creada
    clear();
    refresh();
    return respuesta_form; 
    // Devolvemos 1 si quiere loguearse, 0 si canceló con F1
}


char *trim(char *str) {
    char *end;

    while(*str == ' ') str++;

    if(*str == 0) return str;

    end = str + strlen(str) - 1;
    while(end > str && *end == ' ') end--;

    *(end + 1) = '\0';

    return str;
}