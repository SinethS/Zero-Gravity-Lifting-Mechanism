// src/menu.h

#ifndef MENU_H
#define MENU_H

#include "display.h" // This now includes the public Page enum
#include <util/delay.h>

void menu_init(void);
void menu_update(void);

#endif // MENU_H