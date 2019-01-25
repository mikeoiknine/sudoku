#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <chrono>

// chrono stuff
using Time = std::chrono::system_clock;
using double_sec = std::chrono::duration<double>;
std::chrono::time_point<Time> start, end;

// Game grid
int sudoku[9][9];

// create position stack
std::vector<std::pair<int, int>> position; 

void printSudoku();
bool isValid(int, int, int);
bool attemptMove(int, int, int);
bool findNextCell();
void solve();

void createSudokuFromFile(std::string path) {
	std::ifstream infile(path);
	int r = 0, c = 0;
	while(r < 9 && infile >> sudoku[r][c]) {
		c++;
		if(c > 8) {
			r++;
			c = 0;
		}
	}
}



int main(int argc, char *argv[]) {
	if(argc < 2) {
		std::cout << "Expected path to sudoku .txt file" << std::endl;
		return -1;
	}

	createSudokuFromFile(argv[1]);

	printSudoku();
	start = Time::now();
	solve();
	end = Time::now();
	
	double_sec totalTimeElapsed = end - start;
	
	printSudoku();
	std::cout << "Generated solution in: " << totalTimeElapsed.count() << " seconds" << std::endl;
	return 0;
}

void solve() {
	// Get first editable cell
	findNextCell();
	int r, c;

	do {
		r = position.back().first;
		c = position.back().second;

		// Start brute forcing a solution
		while(!attemptMove(r, c, sudoku[r][c] + 1)) {
			// Remove and reset latest cell
			sudoku[r][c] = 0;
			position.pop_back();

			r = position.back().first;
			c = position.back().second;
		}

	} while(findNextCell());
}

// Looks through sudoku to find the next editable cell and push it 
// to Position vec. If none are available, return false
bool findNextCell() {
	for(int row = 0; row < 9; ++row) {
		for(int col = 0; col < 9; ++col) {
			if(sudoku[row][col] != 0)
				continue;

			// Push current location to stack
			position.push_back(std::make_pair(row, col));
			return true;
		}
	}

	return false;
}

// Attempt to sequentially place the numbers n - 9 
// if 9 is reached and move is still invalid, return false
bool attemptMove(int row, int col, int n) {
	while(n <= 9) {
		if(isValid(row, col, n)) {
			sudoku[row][col] = n;
			return true;
		}
		n++;
	}

	return false;
}

// Check if n is valid in position row, col
bool isValid(int row, int col, int n) {
	// Check row
	for(int i = 0; i < 9; ++i) {
		if(i != col and sudoku[row][i] == n) {
			return false;
		}
	}

	// Check col
	for(int i = 0; i < 9; ++i) {
		if(i != row and sudoku[i][col] == n) {
			return false;
		}
	}

	// Check cells in same square
	for(int i = (row/3 * 3); i < (row/3 * 3 + 3); ++i) {
		for(int j = (col/3 * 3); j < (col/3 * 3 + 3); ++j) {
			if(sudoku[i][j] == n) {
				return false;
			}
		}
	}
	
	return true;
}


// Pretty print the sudoku
void printSudoku() {
	int row = 0;
	while(row < 9) {
		if (row != 0 and row % 3 == 0) {
			printf("\n\n");
		} else {
			printf("\n");
		}

		for(int col = 0; col < 9; ++col) {

			if (col != 0 and col % 3 == 0) {
				if (sudoku[row][col] == 0)
					printf(" [ ]");
				else 
					printf(" [%d]", sudoku[row][col]);
				continue;
			}

			if (sudoku[row][col] == 0)
				printf("[ ]");
			else 
				printf("[%d]", sudoku[row][col]);
			
		}
		row++;
	}
	std::cout << std::endl << std::endl;
}
