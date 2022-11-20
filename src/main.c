#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../include/vm.h"
#include "../include/ui.h"

unsigned char get_nybl(unsigned char, int);
unsigned char set_nybl(unsigned char, int, int);

int main(int argc, char *argv[]) {

	printf("\n\nInitialising 4BOD VM...\n");
	FBOD *fvm = new_fbod();

	InitWindow(PIXEL_SIZE*FBOD_SCREEN_SIZE, PIXEL_SIZE*FBOD_SCREEN_SIZE, "4BOD");
	//SetTargetFPS(120);

	int screen_id = 0; // 0: menu; 1: editor; 2: run;
	unsigned char instruction_pointer = 0;
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(CLR_BG);

		switch(screen_id) {
			case 0:
				handlein_menu();
				draw_menu();
				break;
			case 1:
				handlein_editor(fvm);
				draw_editor(fvm);
				break;
			case 2:
				read_flags(fvm);
				instruction_pointer = perform_instruction(fvm, instruction_pointer);
				draw_screen(fvm);
				break;

		}

		if (IsKeyPressed(KEY_BACK)) {
			screen_id = 0;
		}

		if (IsKeyPressed(KEY_ENTER)) {
			if (g_menu_state == 0) {
				screen_id = 1;
			}
			if (g_menu_state == 1) {
				// Reset VM state
				fvm->acc = 0;
				for (int i=0; i<FBOD_RAM_SIZE; i++) fvm->mem[i] = 0;
				clear_screen(fvm);
				
				// Load flags
				for (int i=0; i<FBOD_PAGE_SIZE * FBOD_PROG_SIZE; i++) {
					Instruction ins = fvm->program[i];
					if (ins.instruction == FBOD_ASM_FLG) {
						fvm->flags[ins.arg1] = i;
					}
				}

				instruction_pointer = 0;

				screen_id = 2;
			}
		}

		// TODO: Make proper file loading/saving
		char *savefile = "resources/save";
		if (IsKeyPressed(KEY_SAVE)) {
			save_program(fvm, savefile);
		}
		if (IsKeyPressed(KEY_LOAD)) {
			load_program(fvm, savefile);
		}

		EndDrawing();
	}

	free(fvm);
	return 0;
}
