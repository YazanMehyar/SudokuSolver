#ifndef __SUDOKU_H__
#define __SUDOKU_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET "\x1B[0m"

#define ERROR(fmt, ...)	fprintf(stderr,RED "ERROR: " fmt RESET"\n",\
						##__VA_ARGS__)
#define DEBUG(fmt, ...)	fprintf(stderr, "%s:%d\t"MAG fmt RESET"\n",\
						__FILE__, __LINE__, ##__VA_ARGS__)

void	parseSudokuFile	(FILE*);
void	printSudokuBoard(void);
bool	solveSudokuBoard(void);

#endif
