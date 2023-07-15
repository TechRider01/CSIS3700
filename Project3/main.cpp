#include <iostream>
#include "pathfinder.h"
#include "disjointSet.h"
#include "Sampler.h"
#include "queue.h"
using namespace std;
uint8_t
    maze[MAX_ROWS][MAX_COLS];
    
//Init randomized chooser
random_device
	rd1;
mt19937
	mt1 = mt19937(rd1());
uniform_int_distribution<>
	dis1(0, 1);

void generateTreeMaze(uint32_t nRows,uint32_t nCols) {
	//Initialize maze with all walls
	uint32_t runCount;
	for(uint32_t row = 0; row < nRows; row++) {
		for(uint32_t col = 0; col < nCols; col++) {
			maze[row][col] = 15;
			//runCount++;
		}
	}
	//cout << runCount << endl;
	//Initialize disjoint set ds with r * c elements
	DisjointSet ds(nRows * nCols);
	//Initialize Sampler vWalls with r * (c - 1)
	Sampler vWalls(nRows * (nCols - 1));
	//Initialize Sample hWalls with (r - 1) * c 
	Sampler hWalls((nRows - 1) * nCols);
	
	//Remember amount of walls selected
	uint32_t
		nV = 0, 
		nH = 0,
		d = 0,
		e = 0,
		r1, c1,
		r2, c2,
		cell1, cell2;
		runCount = 0;
		
	for(int i = 0; i < (nRows * nCols - 1); i++) {
		//No vertical walls left
		if (nV == nRows * (nCols - 1)) {
			d = 1;
			//No horizontal walls left
		} else if (nH == (nRows - 1) * nCols) {
			d = 0;
			//Base case
		} else {
			d = dis1(mt1);
		}
		//runCount++;
		//cout << "NV: " << nV << "\nNH: " << nH << endl;
		//Vertical wall chosen
		if(d == 0) {
			do {
				e = vWalls.getSample();
				nV++;
				//Find cells on either side of wall
				r1 = e/(nCols-1);
				c1 = e%(nCols-1);
				//Make cell to right the "cell pair"
				r2 = r1;
				c2 = c1 + 1;
				//Convert to one number
				cell1 = r1 * nCols + c1;
				cell2 = r2 * nCols + c2;
				//Continue until cells aren't already connected
			} while (ds.find(cell1) == ds.find(cell2));
			//Connect them by removing walls
			ds.join(cell1, cell2);
			maze[r1][c1] &= ~WALL_RIGHT;
			maze[r2][c2] &= ~WALL_LEFT;
		} else {
			do {
				//Select random horizontal wall
				e = hWalls.getSample();
				nH++;
				//Find two cells on either side of wall
				r1 = e/nCols;
				c1 = e%nCols;
				//Set r2 c2 to cell below r1 c1
				r2 = r1 + 1;
				c2 = c1;
				cell1 = r1 * nCols + c1;
				cell2 = r2 * nCols + c2;
			} while (ds.find(cell1) == ds.find(cell2));
			ds.join(cell1, cell2);
			maze[r1][c1] &= ~WALL_DOWN;
			maze[r2][c2] &= ~WALL_UP;
		}
	}
	//cout << "NH: " << nH << " NV: " << nV << endl;
	//cout << runCount;
}

void removeAdditionalWalls(uint32_t nRows,uint32_t nCols,uint32_t nWalls) {
	//Initialize Sampler vWalls with r * (c - 1)
	Sampler vWalls(nRows * (nCols - 1));
	//Initialize Sample hWalls with (r - 1) * c 
	Sampler hWalls((nRows - 1) * nCols);
	//Other stuff
	uint32_t
		nV = 0, 
		nH = 0,
		d = 0,
		e = 0,
		r1, c1,
		r2, c2;
	bool
		haveWall = false;
	
	for(int i = 0; i < nWalls; i++) {
		haveWall = false;
		while (!haveWall) {
			if(nV == nRows*(nCols-1)) {
				d = 1;
			} else if (nH == (nRows-1)*nCols) {
				d = 0;
			} else {
				d = dis1(mt1);
			}
			//Remove vertical wall
			if(d == 0) {
				e = vWalls.getSample();
				nV++;
				//Find cells on either side of wall
				r1 = e/(nCols-1);
				c1 = e%(nCols-1);
				//Make cell to right the "cell pair"
				r2 = r1;
				c2 = c1 + 1;
				if(maze[r1][c1] & WALL_RIGHT) {
					maze[r1][c1] &= ~WALL_RIGHT;
					maze[r2][c2] &= ~WALL_LEFT;
					haveWall = true;
				}
			} else {
				//Select random vertical wall
				e = hWalls.getSample();
				nH++;
				//Find two cells on either side of wall
				r1 = e/nCols;
				c1 = e%nCols;
				//Set r2 c2 to cell below r1 c1
				r2 = r1 + 1;
				c2 = c1;
				if(maze[r1][c1] & WALL_DOWN) {
					maze[r1][c1] &= ~WALL_DOWN;
					maze[r2][c2] &= ~WALL_UP;
					haveWall = true;
				}
			}
		}
	}
}

void findPath(uint32_t nRows,uint32_t nCols) {
	//Set count for entire maze to -1
	int16_t count[nRows][nCols];
	for(int r = 0; r < nRows; r++) {
		for(int c = 0; c < nCols; c++) {
			count[r][c] = -1;
		}
	}
	
	//Variable defintions for loops
	uint32_t e = (nRows - 1)*nCols+nCols-1;
	Queue<uint16_t>
		q;
	q.enqueue(e);
	count[nRows-1][nCols-1] = 0;

	int rowN[] = {-1, 0, 1, 0};
	int colN[] = {0, 1, 0 ,-1};
	
	//While queue is not empty
	while(!q.isEmpty()) {
		e = q.dequeue();
		cout << "";
		//cout << e;
		int r = e/nCols;
		int c = e%nCols;
		//cout << "Row: " << r << " Col: " << c << endl;
		//For each neighbor of rc index
		for(int i = 0; i < 4; i++) {
			//If no wall between rc and r'c' and r'c' == -1
			if(((maze[r][c] & (WALL_UP << i)) == 0) && 
				(count[r+rowN[i]][c+colN[i]] == -1)) {
				//e = r'*nCols+c'
				e = (r + rowN[i])*nCols+(c+colN[i]);
				//cout << "E in loop: " << e << endl;
				q.enqueue(e);
				//count of indexed col = current count + 1
				count[r+rowN[i]][c+colN[i]] = count[r][c] + 1; 
				//cout << "Row: " << r << " Col: " << c << endl;
				//cout << count[r+rowN[i]][c+colN[i]] << endl;
			}
		}
	}
	
	maze[0][0] |= VISITED;
	int r = 0;
	int c = 0;
	
	//While count != 0
	while(count[r][c] != 0) {
		for(int i = 0; i < 4; i++) {
			//If no wall between neighbor and row'col' == current count - 1
			if(((maze[r][c] & (WALL_UP << i)) == 0) && 
				(count[r+rowN[i]][c+colN[i]] == (count[r][c] - 1))) {
				//cout << "Row: " << r << " Col: " << c << endl;
				r = (r+rowN[i]);
				c = (c+colN[i]);
				break;
			}
			//cout << "Row: " << r << " Col: " << c << endl;
		}
		maze[r][c] |= VISITED;
	}
}

int main(int argc, char *argv[]) {
	 uint32_t
	 	r = 0,
	 	c = 0,
	 	w = 0;
    // make sure command line is proper
	if(argc != 4) {
		printf("Usage error\n");
		return 1;
	} 
	
    // get r, c, w from command line
	r = strtoul(argv[1], nullptr, 10);
	c = strtoul(argv[2], nullptr, 10);
	w = strtoul(argv[3], nullptr, 10);
	//cout << r << ' ' << c << ' ' << w << ' ' << endl;
    // generate maze
	generateTreeMaze(r, c);
	removeAdditionalWalls(r, c, w);
    // solve maze
	findPath(r, c);
    // print the maze
	printMaze(maze, r, c);
    return 0;
}
