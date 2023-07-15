#include <iostream>
#include "stack.h"

using namespace std;

const uint16_t
    IS_FILLED_IN = 0x8000,
    VALID_MASK = 0x7fc0,
    NUM_MASK = 0x000f;

Stack<int>
    cells;

uint16_t
    board[9][9];
int
	best;
char
	boardIn;

bool findBest();
void solve();
uint8_t countBits(uint16_t);
void inputBoard();
void displayBoard();
void negateOption(const uint8_t, const uint8_t);

int main() {
    // input the board
    inputBoard();
    // use solve to solve the puzzle
	solve();
    // output solved puzzle (or claim it's unsolvable)
	displayBoard();
    return 0;
}

bool findBest () {
	//Marks all empty cells on board as all choices being valid
	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			//if board empty
			if(!(board[i][j] & IS_FILLED_IN)) {
				//all choices valid
				board[i][j] |= VALID_MASK;
			}
		}
	}
	
	//Scans board rows for filled cells
	for(int r = 0; r < 9; r++) {
		//Scans board columns for filled cells
		for(int c = 0; c < 9; c++) {
			//If cell is filled
			if(board[r][c] & IS_FILLED_IN) {
				//Eliminate that option for that row, column, and block
				negateOption(r, c);
			}
		}
	}
	
	//Set baseline low for comparison
	int low = 10;
	//Scans board for !IS_FILLED_IN cells to find which one has the least valid choices
	for(int r = 0; r < 9; r++) {
		for(int c = 0; c < 9; c++) {
			//If board is not filled in
			if(!(board[r][c] & IS_FILLED_IN)) {
				if(countBits(board[r][c] & VALID_MASK) < low) {
					low = countBits(board[r][c] & VALID_MASK);
					best = r * 9 + c;
				}
			}
		}
	}
	//If no such choice available
	if(low == 10) {
		return false;
	}
	board[best/9][best%9] |= IS_FILLED_IN;
	cells.push(best);
	return true;
}

uint8_t countBits(uint16_t a) {
	uint8_t
		count = 0;
	while(a != 0) {
		if(a & 1) {
			count++;
		}
		a >>= 1;
	}
	return count;
}

void solve () {
	findBest();
	while(1) {
		int pos = cells.peek();
		int r = pos / 9;
		int c = pos % 9;
    	//cout << r << endl << c << endl;
		for(pos = 1; pos <= 9; pos++) {
			if((board[r][c] & (IS_FILLED_IN >> pos)) != 0) {
				break;
			}
		}
		if(pos == 10) {
			//Mark board ij as not filled
			board[r][c] &= ~(IS_FILLED_IN);
			//Pop stack
			//Go back one step to try next valid choice
			cells.pop();
			
			if(cells.isEmpty()) {
				cout << "No solution 1." << endl;
				break;
			}
			continue;
		}
		board[r][c] &= ~(IS_FILLED_IN >> pos);
		board[r][c] = (board[r][c] & ~NUM_MASK) | pos;
		if(!(findBest())) {
			break;
		}
	}
}

void displayBoard() {
	for (int r = 0; r < 9; r++) {
  		// loop over all cols
  		if(!(r%3)) {
  			cout << endl;
  		}
    	for(int c = 0; c < 9; c++) {
            // display one value
            if(!(c%3)) {
            	cout << ' ';
            }
            cout << (board[r][c] & NUM_MASK);
    	}
    	cout << endl;
    }
    cout << endl;
}

void negateOption (const uint8_t r, const uint8_t c) {
	for(int i = 0; i < 9; i++) {
	//If row element isn't marked as filled in
		if(!(board[i][c] & IS_FILLED_IN)) {
			//Row elements = 1000... right shifted by num for that element
			board[i][c] &= ~(IS_FILLED_IN >> (board[r][c] & NUM_MASK));
		}
	}
	for(int j = 0; j < 9; j++) {
		//If col element isn't marked as filled in
		if(!(board[r][j] & IS_FILLED_IN)) {
			//Col elements = 1000... right shifted by num for that element
			board[r][j] &= ~(IS_FILLED_IN >> (board[r][c] & NUM_MASK));
		}
	}
	//Eliminates options for the block the number is in
	//Specfies block row start and end point
	for(int i = r - r%3; i < (r - r%3) + 3; i++) {
		//Specifies block column start and end point
		for(int j = c - c%3; j < (c - c%3) + 3; j++) {
			//If block element isn't filled in
			if(!(board[i][j] & IS_FILLED_IN)) {
				//Board element = ~1000... right shifted by numm for the element
				board[i][j] &= ~(IS_FILLED_IN >> (board[r][c] & NUM_MASK));
			}
		}
	}
}

void inputBoard() {
	for (int i = 0; i < 9; i++) {
  		// loop over all cols
    	for(int j = 0; j < 9; j++) {
            // read one value
            cin >> boardIn;
            // if it's a dot, set board[i][j] to 0
            if(boardIn == '.') {
            	board[i][j] = 0;
            // else, set board[i][j] to IS_FILLED_IN | (value - '0')
            } else {
            	board[i][j] = IS_FILLED_IN | (boardIn - '0');
            }
    	}
    }
}
