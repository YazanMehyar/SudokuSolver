#include "sudoku.h"

/*****************************************************************************************
*	HELPER FUNCTIONS
*****************************************************************************************/
static void
printSeparatorLine(void) {
	uint8_t i = (BOARD_SIZE + BLOCK_SIZE) * 3;
	while(i--) putchar('-');
	putchar('\n');
}
/*****************************************************************************************
*	PUZZLE FUNCTIONS
*****************************************************************************************/

void
parseSudokuFile(FILE* f) {
	int c;
	uint8_t row = 0;
	uint8_t col = 0;
	SudokuCell cell;
	while( (c = fgetc(f)) != EOF ) {
		switch (c) {
			case '1'...'9':
				cell.solved	= true;
				cell.value	= c - '0';
				break;

			case '#':
			case '0':
				cell.solved	= false;
				cell.value	= 0;
				break;

			case '\n':
				if(col == 0) continue;
				if(col != BOARD_SIZE) {
					ERROR("%s @ R: %hhu", "TOO FEW COLUMNS", row);
					exit(1);
				} row++; col=0;
				continue;

			case '|':
			case '-':
			case ' ':
			case '\t':
				continue;

			default:
				ERROR("%s @ R: %hhu C: %hhu","READ UNKNOWN CHARACTER", row, col);
				exit(1);
		}

		if(row == BOARD_SIZE) break;
		if(col == BOARD_SIZE) {
			ERROR("%s @ R: %hhu", "TOO MANY COLUMNS", row);
			exit(1);
		}

		sudoku_board[row][col++] = cell;
	}

	if(row != BOARD_SIZE || col != 0) {
		ERROR("%s","INCOMPLETE BOARD");
		exit(1);
	}
}

void
printSudokuBoard(void) {
	printSeparatorLine();
	for (uint8_t row = 0; row < BOARD_SIZE; row++) {
		printf("|");
		for (uint8_t col = 0; col < BOARD_SIZE; col++) {
			sudoku_board[row][col].solved?
				 printf(" %u ", sudoku_board[row][col].value)
				:printf(" # ");

			if( (col+1) % BLOCK_SIZE == 0) printf(" | ");
		} putchar('\n');
		if( (row+1) % BLOCK_SIZE == 0) printSeparatorLine();
	}
}

bool
solveSudokuBoard(void) {
	return false;
}

/*****************************************************************************************
*	MAIN
*****************************************************************************************/

int
main(int argc, char const *argv[]) {
	if(argc != 2) {
		ERROR("Expected usage:\n%s %s",argv[0], "sudoku_puzzle_file_path");
		exit(1);
	}

	FILE *f = fopen(argv[1],"r");
	if(!f) {
		ERROR("%s: '%s'", "UNABLE TO OPEN FILE", argv[1]);
		exit(1);
	}

	parseSudokuFile(f);
	printSudokuBoard();
	return 0;
}
