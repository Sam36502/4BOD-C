/*
 *	
 * 	4BOD User Interface
 * 
 */

#ifndef FBOD_UI_H
#define FBOD_UI_H

#include "vm.h"

#define PIXEL_SIZE 32
#define CLR_BG GOLD
#define CLR_FG BLACK
#define EDIT_GRID 1

#define ARROW_LEFT  1
#define ARROW_RIGHT 2
#define ARROW_UP    4
#define ARROW_DOWN  8

#define KEY_LEFT  263 // Left Arrow
#define KEY_RIGHT 262 // Right Arrow
#define KEY_UP    265 // Up Arrow
#define KEY_DOWN  264 // Down Arrow
#define KEY_ENTER 257 // Enter Key
#define KEY_BACK  259 // Backspace
#define KEY_SAVE   83 // 'S' Key
#define KEY_LOAD   76 // 'L' Key
#define MOUSE_LEFT 0

extern const unsigned short SCR_START_EDT[FBOD_SCREEN_SIZE];
extern const unsigned short SCR_START_RUN[FBOD_SCREEN_SIZE];

extern unsigned char g_menu_state;
extern unsigned char g_prog_page;

void draw_data(unsigned short data[FBOD_SCREEN_SIZE]);
void toggle_menu();
void draw_menu();
void draw_screen(FBOD *f);
void draw_editor(FBOD *f);

unsigned char get_arrow_nyble();
void handlein_menu();
void handlein_editor(FBOD *f);

#endif
