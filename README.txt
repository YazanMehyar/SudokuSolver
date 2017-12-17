@Author: Yazan Mehyar <zen>
@Filename: README.txt
@Last modified time: 17-Dec-2017

******************************************************************************************
*	Introduction
******************************************************************************************

	This program was written to solve traditional 9x9 sudoku puzzles. It aims to do so
without backtracking. In other words, it analyses the sudoku board using mathematical
properties set by the rules of the game and the intrinsic structure of the board.
However, I should point out that this program cannot solve a sudoku puzzle that does not
have a unique solution.

TO COMPILE:
	- Ensure you have a (empty) bin directory
	- run :
		$ make
	- then run :
		$ make main

TO RUN:
	- run :
		$ ./main path_to_puzzle_file

******************************************************************************************
*	Syntax & Semantics of Puzzle File
******************************************************************************************

	The file containing the sudoku puzzle must bide by the following syntax rules:

	- PUZZLE		:: 9#(<SURPLUS_LINE>*<ROW>)
	- ROW			:: <START_OF_LINE>9#<ENTRY><newline>
	- SURPLUS_LINE	:: <SURPLUS>*<newline>
	- ENTRY			:: <SURPLUS>*<PLACE_HOLDER><SURPLUS>*
	- PLACE_HOLDER	:: <DIGIT> | <BLANK>
	- SURPLUS		:: '|' | '-' | ' ' | '\t'
	- DIGIT			:: '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
	- BLANK			:: '#' | '0'

	Example:

		# # # | # 8 # | 3 2 6
		# # # | # # 1 | # 4 #
		# # # | 9 6 # | # # 8
		---------------------
		# # 8 | # 3 6 | 9 # 2
		5 # 3 | 2 # # | # # #
		# 1 # | 7 # # | # # #
		---------------------
		3 # # | 6 # # | # # 4
		1 6 # | # # # | # # #
		4 # 9 | 3 # # | 8 # #

******************************************************************************************
*	Status
******************************************************************************************

	So far the program is capable of:
		- Identifying exclusive board cells. An exclusive cell is one that a certain digit
		  must exist in due restraints set by the groups the cell belongs in (row, column,
		  block).

	Complexity of puzzles solved: SIMPLE
