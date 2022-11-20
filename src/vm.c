/*
 *	Implementation of 4BOD VM
 *
 *	Header: ../include/vm.h
 */

#include <stdlib.h>
#include <stdio.h>

#include "../include/vm.h"
#include "../include/ui.h"

// nyble help functions
unsigned char set_nybl(unsigned char n, int index, int val) {
	val = val % 0x10;
	unsigned char mask = 0x0F;
	mask <<= index * 4;
	val <<= index * 4;
	return (n & ~mask) | (val & mask);
}

unsigned char get_nybl(unsigned char n, int index) {
	unsigned char mask = 0x0F;
	mask <<= index * 4;
	return (n & mask) >> (index * 4);
}

// Initialise new FBOD
FBOD *new_fbod() {
	FBOD *f = calloc(sizeof(FBOD), 1);
	return f;
}

unsigned char perform_instruction(FBOD *f, unsigned char index) {
	unsigned char next_cmd = index + 1;
	Instruction ins = f->program[index];
	int x = get_mem(f, ins.arg1);
	int y = get_mem(f, ins.arg2);
	// printf("[%3d] 0x%X, %d, %d\n", index, ins.instruction, ins.arg1, ins.arg2);

	switch (ins.instruction) {
		default:
		case FBOD_ASM_NOP:
			break;
		case FBOD_ASM_MVA:
			f->acc = x;
			break;
		case FBOD_ASM_MVM:
			set_mem(f, ins.arg1, f->acc);
			break;
		case FBOD_ASM_STA:
			f->acc = ins.arg1;
			break;
		case FBOD_ASM_INA:
			f-> acc = get_arrow_nyble();
			break;
		case FBOD_ASM_INC:
			f->acc++;
			if (f->acc > 15) f->acc = 0;
			break;
		case FBOD_ASM_CLS:
			clear_screen(f);
			break;
		case FBOD_ASM_SHL:
			f->acc <<= 1;
			break;
		case FBOD_ASM_SHR:
			f->acc >>= 1;
			break;
		case FBOD_ASM_RDP:
			f->acc = get_pixel(f, x, y);
			break;
		case FBOD_ASM_FLP:
			flip_pixel(f, x, y);
			break;
		case FBOD_ASM_FLG:
			// Does nothing (Flags are added during program loading)
			break;
		case FBOD_ASM_JMP:
			next_cmd = f->flags[ins.arg1];
			break;
		case FBOD_ASM_CEQ:
			next_cmd += !(x == f->acc);
			break;
		case FBOD_ASM_CGT:
			next_cmd += !(x > f->acc);
			break;
		case FBOD_ASM_CLT:
			next_cmd += !(x < f->acc);
			break;
	}
	return next_cmd;
}

// Get/Set various memory locations
void set_instruction(FBOD *f, int addr, Instruction ins) {
	f->program[addr] = ins;
}

Instruction get_instruction(FBOD *f, int addr) {
	return f->program[addr];
}

void set_mem(FBOD *f, int addr, int val) {
	unsigned char byte = f->mem[addr/2];
	byte = set_nybl(byte, addr % 2, val);
	f->mem[addr/2] = byte;
}

int get_mem(FBOD *f, int addr) {
	unsigned char byte = f->mem[addr/2];
	return (int) get_nybl(byte, addr % 2);
}

void flip_pixel(FBOD *f, int x, int y) {
	unsigned short row = 1 << (FBOD_SCREEN_SIZE - 1 - x);
	f->screen[y] = f->screen[y] ^ row;
}

int get_pixel(FBOD *f, int x, int y) {
	unsigned short row = f->screen[y];
	return (row >> (FBOD_SCREEN_SIZE - 1 - x)) % 2;
}

void clear_screen(FBOD *f) {
	for (int i=0; i<FBOD_SCREEN_SIZE; i++) {
		f->screen[i] = 0;
	}
}

void read_flags(FBOD *f) {
	for (int i=0; i<FBOD_PAGE_SIZE * FBOD_PROG_SIZE; i++) {
		Instruction ins = f->program[i];
		if (ins.instruction == FBOD_ASM_FLG) {
			f->flags[ins.arg1] = i;
		}
	}
}

void save_program(FBOD *f, char *filename) {
	FILE *file = fopen(filename, "wb");

	for (int i=0; i<FBOD_PAGE_SIZE * FBOD_PROG_SIZE; i++) {
		Instruction ins = f->program[i];
		unsigned short insbits = (ins.instruction << 8) | (ins.arg1 << 4) | ins.arg2;
		fprintf(file, "%c%c", (insbits >> 8) % 256, insbits % 256);
	}

	fclose(file);
}

void load_program(FBOD *f, char *filename) {
	FILE *file = fopen(filename, "rb");

	for (int i=0; i<FBOD_PAGE_SIZE * FBOD_PROG_SIZE; i++) {
		unsigned char lower, upper;
		unsigned short insbits;
		Instruction ins;

		fscanf(file, "%c%c", &upper, &lower);
		insbits = (upper << 8) | lower;
		ins.instruction = (insbits >> 8) % 16;
		ins.arg1 = (insbits >> 4) % 16;
		ins.arg2 = insbits % 16;
		f->program[i] = ins;
	}

	fclose(file);
}
