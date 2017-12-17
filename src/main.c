#include "sudoku.h"

int
main(int argc, char const *argv[]) {
	if(argc != 2) {
		ERROR("Expected usage:\n%s %s",argv[0], "sudoku_puzzle_file_path");
		exit(1);
	}

	FILE *f = fopen(argv[1],"r");
	if(!f) {
		ERROR("UNABLE TO OPEN FILE: '%s'", argv[1]);
		exit(1);
	}

	parseSudokuFile(f);
	printf("\nThe starting sudoku puzzle:\n");
	printSudokuBoard();
	if(solveSudokuBoard()) {
		printf("\nGREAT! Found a solution!\n");
	} else {
		printf("\nTOO BAD! Could not find a solution. This could be because:\n"
				"1) The program isn't advanced enough to solve this puzzle\n"
				"2) The puzzle doesn't have a unique solution\n"
			);
	}
	printSudokuBoard();
	return 0;
}
