/*
 *	
 *	4BOD Virtual Machine
 *
 */

#ifndef FBODVM_H
#define FBODVM_H

#define FBOD_PAGE_SIZE   16 // No. of instructions per page
#define FBOD_PROG_SIZE   16 // No. of pages in program memory
#define FBOD_RAM_SIZE    16 // 16N = 8B
#define FBOD_FLAGS_SIZE  16 // 16 possible flags
#define FBOD_SCREEN_SIZE 16 // 16 * 16 display

/*
 * 4BOD Instructions
 */
#define FBOD_ASM_NOP 0x0 // Do nothing
#define FBOD_ASM_MVA 0x1 // Move arg1 mem value to acc
#define FBOD_ASM_MVM 0x2 // Move acc value to arg1 mem
#define FBOD_ASM_STA 0x3 // Set acc value to arg1
#define FBOD_ASM_INA 0x4 // Set acc to arrow key state (1 L; 2 R; 4 U; 8 D)
#define FBOD_ASM_INC 0x5 // Increment acc
#define FBOD_ASM_CLS 0x6 // Clear screen
#define FBOD_ASM_SHL 0x7 // Bitwise shift acc left
#define FBOD_ASM_SHR 0x8 // Bitwise shift acc right
#define FBOD_ASM_RDP 0x9 // Read pixel at x=mem arg1, y=mem arg2 to acc
#define FBOD_ASM_FLP 0xA // Toggle pixel at x=mem arg1, y=mem arg2
#define FBOD_ASM_FLG 0xB // Create flag named arg1
#define FBOD_ASM_JMP 0xC // Jump to flag mem arg1 (last in file)
#define FBOD_ASM_CEQ 0xD // Only perform next instruction if mem arg1 == acc
#define FBOD_ASM_CGT 0xE // Only perform next instruction if mem arg1 > acc
#define FBOD_ASM_CLT 0xF // Only perform next instruction if mem arg1 < acc

typedef struct Instruction {
	unsigned short instruction : 4;
	unsigned short arg1 : 4;
	unsigned short arg2 : 4;
} Instruction;

typedef struct FBOD {
	Instruction program[FBOD_PROG_SIZE*FBOD_PAGE_SIZE];
	unsigned char acc;
	unsigned char mem[FBOD_RAM_SIZE / 2];      // Halved because we're storing nybles in bytes
	unsigned short screen[FBOD_SCREEN_SIZE];   // 16 shorts = 16 * 16
	unsigned char flags[FBOD_FLAGS_SIZE];
} FBOD;

// 4BOD Methods
FBOD *new_fbod();
unsigned char perform_instruction(FBOD *f, unsigned char index); // Returns the index of the next command

// Get/Set various memory locations
void set_instruction(FBOD *f, int addr, Instruction ins);
Instruction get_instruction(FBOD *f, int addr);
void set_mem(FBOD *f, int addr, int val);
int get_mem(FBOD *f, int addr);
void flip_pixel(FBOD *f, int x, int y);
int get_pixel(FBOD *f, int x, int y);
void clear_screen(FBOD *f);
void read_flags(FBOD *f);
void save_program(FBOD *f, char *filename);
void load_program(FBOD *f, char *filename);

#endif
