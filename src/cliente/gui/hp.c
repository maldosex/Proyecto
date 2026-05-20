#include <stdlib.h>
#include <string.h>
#include "hp.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

char *module_choices[] = {
                        "Habit Store",
                        "My Habits",
                        "My progress",
                        "Settings",
                        (char *)NULL,
                  };

int hp_menu()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;
	
	/* Initialize curses */
	initscr();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);

	/* Create items */
        n_choices = ARRAY_SIZE(module_choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(module_choices[i], " ");

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(10, 40, 4, 20);
        keypad(my_menu_win, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Habit Flow", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    attron(A_BLINK );
    mvprintw(1, 20, "Habit FLOW");
    attroff(A_BLINK);
    attroff(COLOR_PAIR(1));
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
        	case 10:
            	mvprintw(LINES-2, 3, "selected: ");
				option = item_index(current_item(my_menu));
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

