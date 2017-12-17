#include "sudoku.h"

/*****************************************************************************************
*	Local definitions
*****************************************************************************************/
typedef struct {
	uint16_t	value;
	bool		solved;
} SudokuCell;

// NOTE:	BLOCK_SIZE is a simple place holder
//			It DOES NOT allow the program to cope with different size puzzles
#define BLOCK_SIZE		3u
#define BOARD_SIZE		(BLOCK_SIZE*BLOCK_SIZE)

// NOTE:	We need integer division here
#define BLK_IDX(r,c)	( ((r)/BLOCK_SIZE)*BLOCK_SIZE + (c)/BLOCK_SIZE )
#define GET_LSb(x)		( (x) & -(x) )
#define CELL_MASK		( (1<<BOARD_SIZE)-1 )

/*****************************************************************************************
*	Local variables
*****************************************************************************************/
static SudokuCell sudoku_board[BOARD_SIZE][BOARD_SIZE];

/*****************************************************************************************
*	HELPER FUNCTIONS
*****************************************************************************************/
static void
printSeparatorLine(void) {
	uint8_t i = (BOARD_SIZE + BLOCK_SIZE) * 3;
	while(i--) putchar('-');
	putchar('\n');
}

static uint8_t
getLSbPos(uint16_t bits) {
	static const uint16_t BRUIJN_CONST = 0x9AF;
	static const uint8_t BRUIJN_SEQ[16]= {
		0,	1,	2,	5,	3,	9,	6,	11,
		15,	4,	8,	10,	14,	7,	13,	12
	};

	bits = GET_LSb(bits) * BRUIJN_CONST;
	return BRUIJN_SEQ[bits >> 12]+1;
}

static void
analyseUnsolvedCells(void) {
	uint16_t row_mask[BOARD_SIZE];
	uint16_t col_mask[BOARD_SIZE];
	uint16_t blk_mask[BOARD_SIZE];

	for(uint8_t i = 0; i < BOARD_SIZE; i++) {
		row_mask[i] = col_mask[i] = blk_mask[i] = 0;
	}

	for(uint8_t row = 0; row < BOARD_SIZE; row++) {
		for (uint8_t col = 0; col < BOARD_SIZE; col++) {
			if( !sudoku_board[row][col].solved ) continue;

			const uint16_t BIT_SET = 1 << (sudoku_board[row][col].value-1);
			row_mask[row] |= BIT_SET;
			col_mask[col] |= BIT_SET;
			blk_mask[BLK_IDX(row,col)] |= BIT_SET;

		}
	}

	for(uint8_t row = 0; row < BOARD_SIZE; row++) {
		for (uint8_t col = 0; col < BOARD_SIZE; col++) {
			if( sudoku_board[row][col].solved ) continue;

			sudoku_board[row][col].value &=
				~(	  row_mask[row]
					| col_mask[col]
					| blk_mask[BLK_IDX(row,col)]
				);
		}
	}
}

static bool
filterExclusiveCells(void) {
	struct {
		uint16_t _1;
		uint16_t _2;
		uint16_t _3;
		uint16_t _4;
	} row_data[BOARD_SIZE], col_data[BOARD_SIZE], blk_data[BOARD_SIZE];

	for (uint8_t i = 0; i < BOARD_SIZE; i++) {
		row_data[i]._1 = row_data[i]._2 = row_data[i]._3 = row_data[i]._4 = 0;
		col_data[i]._1 = col_data[i]._2 = col_data[i]._3 = col_data[i]._4 = 0;
		blk_data[i]._1 = blk_data[i]._2 = blk_data[i]._3 = blk_data[i]._4 = 0;
	}

	for(uint8_t row = 0; row < BOARD_SIZE; row++) {
		for (uint8_t col = 0; col < BOARD_SIZE; col++) {
			if( sudoku_board[row][col].solved ) continue;

			row_data[row]._4 |= sudoku_board[row][col].value & row_data[row]._3;
			row_data[row]._3 |= sudoku_board[row][col].value & row_data[row]._2;
			row_data[row]._2 |= sudoku_board[row][col].value & row_data[row]._1;
			row_data[row]._1 |= sudoku_board[row][col].value;

			col_data[col]._4 |= sudoku_board[row][col].value & col_data[col]._3;
			col_data[col]._3 |= sudoku_board[row][col].value & col_data[col]._2;
			col_data[col]._2 |= sudoku_board[row][col].value & col_data[col]._1;
			col_data[col]._1 |= sudoku_board[row][col].value;

			const uint8_t IDX = BLK_IDX(row,col);
			blk_data[IDX]._4 |= sudoku_board[row][col].value & blk_data[IDX]._3;
			blk_data[IDX]._3 |= sudoku_board[row][col].value & blk_data[IDX]._2;
			blk_data[IDX]._2 |= sudoku_board[row][col].value & blk_data[IDX]._1;
			blk_data[IDX]._1 |= sudoku_board[row][col].value;
		}
	}

	bool solved = false;
	for (uint8_t i = 0; i < BOARD_SIZE; i++) {
		row_data[i]._1 ^= row_data[i]._2;
		row_data[i]._2 ^= row_data[i]._3;
		row_data[i]._3 ^= row_data[i]._4;

		col_data[i]._1 ^= col_data[i]._2;
		col_data[i]._2 ^= col_data[i]._3;
		col_data[i]._3 ^= col_data[i]._4;

		blk_data[i]._1 ^= blk_data[i]._2;
		blk_data[i]._2 ^= blk_data[i]._3;
		blk_data[i]._3 ^= blk_data[i]._4;

		uint8_t  idx;
		uint16_t match;
		while( row_data[i]._1 ) {
			for(idx = 0; idx < BOARD_SIZE; idx++) {
				if(sudoku_board[i][idx].solved) continue;
				if((match = row_data[i]._1 & sudoku_board[i][idx].value)) break;
			}

			solved = sudoku_board[i][idx].solved = true;
			sudoku_board[i][idx].value = getLSbPos(match);

			row_data[i]._1 &= ~match;

		} if(solved) break;
		while( col_data[i]._1 ) {
			for(idx = 0; idx < BOARD_SIZE; idx++) {
				if(sudoku_board[idx][i].solved) continue;
				if((match = col_data[i]._1 & sudoku_board[idx][i].value)) break;
			}

			solved = sudoku_board[idx][i].solved = true;
			sudoku_board[idx][i].value = getLSbPos(match);

			col_data[i]._1 &= ~match;

		} if(solved) break;
		while( blk_data[i]._1 ) {
			uint8_t idx_2;
			const uint8_t ROW = (i/BLOCK_SIZE)*BLOCK_SIZE;
			const uint8_t COL = (i%BLOCK_SIZE)*BLOCK_SIZE;
			for( idx = ROW; idx < ROW+BLOCK_SIZE; idx++ ) {
				for( idx_2 = COL; idx_2 < COL+BLOCK_SIZE; idx_2++) {
					if(sudoku_board[idx][idx_2].solved) continue;
					if((match = blk_data[i]._1 & sudoku_board[idx][idx_2].value)) goto BREAK;
				}
			}
			BREAK:
			solved = sudoku_board[idx][idx_2].solved = true;
			sudoku_board[idx][idx_2].value = getLSbPos(match);

			blk_data[i]._1 &= ~match;
		} if(solved) break;
	} return solved;
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
				cell.value	= CELL_MASK;
				break;

			case '\n':
				if(col == 0) continue;
				if(col != BOARD_SIZE) {
					ERROR("TOO FEW COLUMNS @ R: %hhu", row);
					exit(1);
				} row++; col=0;
				continue;

			case '|':
			case '-':
			case ' ':
			case '\t':
				continue;

			default:
				ERROR("READ UNKNOWN CHARACTER @ R: %hhu C: %hhu", row, col);
				exit(1);
		}

		if(row == BOARD_SIZE) break;
		if(col == BOARD_SIZE) {
			ERROR("TOO MANY COLUMNS @ R: %hhu", row);
			exit(1);
		}

		sudoku_board[row][col++] = cell;
	}

	if(row != BOARD_SIZE || col != 0) {
		ERROR("INCOMPLETE BOARD");
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

	do {
		analyseUnsolvedCells();
	} while (
		filterExclusiveCells()
	);

	for(uint8_t row = 0; row < BOARD_SIZE; row++) {
		for (uint8_t col = 0; col < BOARD_SIZE; col++) {
			if( !sudoku_board[row][col].solved ) return false;
		}
	} return true;
}
