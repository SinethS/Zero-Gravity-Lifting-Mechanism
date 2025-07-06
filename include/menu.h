// menu.h

#ifndef MENU_H
#define MENU_H

#include "display.h" // Include for the Page enum definition

void menu_init(void);
void menu_update(void);
void menu_process_button(int button_code);

#endif // MENU_H