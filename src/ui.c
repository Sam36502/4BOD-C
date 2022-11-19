/*
 *
 *	4BOD User Interface implementation
 *	
 *	This handles all the displaying and user input stuff
 *	using raylib
 *
 *	Header: ../include/ui.h
 *
 */

#include <stdio.h>
#include <raylib.h>

#include "../include/vm.h"
#include "../include/ui.h"

// Screen data for menu with edit selected
const unsigned short SCR_START_EDT[FBOD_SCREEN_SIZE] = {
	0x0000, 0x3FF8, 0x44C4, 0x456C,
	0x5D6C, 0x44EC, 0x3FF8, 0x0000,
	0x0000, 0x0000, 0x3AB0, 0x2AA8,
	0x32A8, 0x2BA8, 0x0000, 0x0000,
};

// Screen data for menu with run selected
const unsigned short SCR_START_RUN[FBOD_SCREEN_SIZE] = {
	0x0000, 0x0000, 0x3B38, 0x3A90,
	0x2290, 0x3B10, 0x0000, 0x0000,
	0x0000, 0x3FF8, 0x454C, 0x5554,
	0x4D54, 0x5454, 0x3FF8, 0x0000,
};

unsigned char g_menu_state = 0; // 0 = edit; 1 = run
unsigned char g_prog_page = 0;

void draw_data(unsigned short data[FBOD_SCREEN_SIZE]) {
	for (int y=0; y<FBOD_SCREEN_SIZE; y++) {
		for (int x=0; x<FBOD_SCREEN_SIZE; x++) {
			if ((data[y] >> (FBOD_SCREEN_SIZE - 1 - x)) % 2) {
				DrawRectangle(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, CLR_FG);
			}
		}
	}
}

void toggle_menu() {
	g_menu_state = !g_menu_state;
}

void draw_menu() {
	if (g_menu_state == 0) {
		draw_data(SCR_START_EDT);
	} else if (g_menu_state == 1) {
		draw_data(SCR_START_RUN);
	} else {
		g_menu_state = 0;
	}
}

void draw_screen(FBOD *f) {
	draw_data(f->screen);
}

void draw_editor(FBOD *f) {
	// Draw "scrollbar"
	DrawRectangle((16 - 3) * PIXEL_SIZE, g_prog_page * PIXEL_SIZE, 2 * PIXEL_SIZE, PIXEL_SIZE, CLR_FG);

	// Draw program data
	for (int y=0; y<FBOD_PAGE_SIZE; y++) {
		Instruction ins = f->program[g_prog_page * FBOD_PAGE_SIZE + y];
		unsigned short insbits = (ins.instruction << 8) | (ins.arg1 << 4) | ins.arg2;
		for (int x=0; x<12; x++) {
			if((insbits >> (12 - 1 - x)) % 2) {
				DrawRectangle(x*PIXEL_SIZE, y*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, CLR_FG);
			}
		}
	}
}

unsigned char get_arrow_nyble() {
	unsigned char state = 0;
	if (IsKeyDown(KEY_LEFT)) {
		state |= ARROW_LEFT;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		state |= ARROW_RIGHT;
	}
	if (IsKeyDown(KEY_UP)) {
		state |= ARROW_UP;
	}
	if (IsKeyDown(KEY_DOWN)) {
		state |= ARROW_DOWN;
	}
	return state;
}

void handlein_menu() {
	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
		toggle_menu();
	}
}

void handlein_editor(FBOD *f) {

	// Handle "scrolling" up & down
	if (IsKeyPressed(KEY_UP) && g_prog_page > 0) {
		g_prog_page--;
	}
	if (IsKeyPressed(KEY_DOWN) && g_prog_page < 15) {
		g_prog_page++;
	}

	// Draw grid
	if (EDIT_GRID) {
		for (int i=1; i<=12; i++) {
			DrawLine(i * PIXEL_SIZE, 0, i * PIXEL_SIZE, FBOD_SCREEN_SIZE * PIXEL_SIZE, CLR_FG);
		}
		for (int i=1; i<=FBOD_SCREEN_SIZE; i++) {
			DrawLine(0, i * PIXEL_SIZE, 12 * PIXEL_SIZE, i * PIXEL_SIZE, CLR_FG);
		}
	}

	// Handle mouse clicks
	if (IsMouseButtonPressed(MOUSE_LEFT)) {
		int x = GetMouseX() / PIXEL_SIZE;
		int y = GetMouseY() / PIXEL_SIZE;
		Instruction ins = f->program[g_prog_page * FBOD_PAGE_SIZE + y];
		unsigned short insbits = (ins.instruction << 8) | (ins.arg1 << 4) | ins.arg2;
		insbits ^= 1 << (12 - 1 - x);
		ins.instruction = (insbits >> 8) % 16;
		ins.arg1 = (insbits >> 4) % 16;
		ins.arg2 = insbits % 16;
		f->program[g_prog_page * FBOD_PAGE_SIZE + y] = ins;
	}
}

