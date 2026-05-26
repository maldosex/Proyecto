#include "menu_myHabits.h"

#include <stdlib.h>
#include <string.h>
#include "menu.h"


int menu_available_habits(Habito *habitos, int count){

    int i;
    char choices[30][100];

    init_pair(1, COLOR_RED, COLOR_BLACK);

    for(i = 0; i < count; i++){

        snprintf(choices[i], sizeof(choices[i]), "%d - %s", habitos[i].id,  habitos[i].nombre);
    }


    ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices   ;
	
	init_pair(1, COLOR_RED, COLOR_BLACK);

	/* Create items */
        n_choices = count +1;
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < count; ++i){
                my_items[i] = new_item(choices[i], "");
        }
        my_items[count] = NULL;

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(10, 40, 4, 20);
        keypad(my_menu_win, TRUE);

        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "-----Habitos disponibles-----", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    mvprintw(1, 20, "Habit FLOW");
    attroff(COLOR_PAIR(1));

	menu_opts_off(my_menu, O_ONEVALUE);
	refresh();
        

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	int option = -1;
	while((c = wgetch(my_menu_win)) != KEY_F(1)){
		switch(c){	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case ' ':
				menu_driver(my_menu, REQ_TOGGLE_ITEM);
				break;
        	case 10:{
			    ITEM **items;
			    char temp[300];
			
			    temp[0] = '\0';
			
			    items = menu_items(my_menu);
			
			    for(i = 0; i < item_count(my_menu); i++){
				
			        if(item_value(items[i]) == TRUE){
					
			            strcat(temp, item_name(items[i]));
			            strcat(temp, " | ");
			        }
			    }
			
			    move(LINES - 3, 0);
			    clrtoeol();
			
			    mvprintw(LINES - 3, 0, "Seleccionados: %s", temp);
			
			    refresh();
			}
			break;
		}
        wrefresh(my_menu_win);
		if (option != -1) {
            break;
        }
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
		
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);

    
	clear();
    refresh();
	return option;
}
    
