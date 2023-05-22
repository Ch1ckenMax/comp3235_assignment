// This program reads "blocks" data, fills the grid with the blocks, and displays the grid.
// No error checking! So make sure your input is 100% correct.

#include <stdio.h>
#include <string.h>

char s[10];

const char *itos(unsigned short);
void display();

char grid[32][32] = { 0 };

int i, j;

void main() {
	unsigned short ABC[3][4][4] = {
		{ {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} },
		{ {2,2,2,2}, {3,0,0,0}, {1,1,1,1}, {0,0,0,3} },
		{ {2,2,2,3}, {3,2,2,2}, {3,1,1,1}, {1,1,1,3} }
	};

	int x, y, t, r;
	char type;

	// read data (stdin) line by line:
	while (scanf("%d %d %c %d", &x, &y, &type, &r) != EOF) {
		x *= 4; y *= 4; // from 0..7 to 0..31
		t = type - 'A';
		for (i = 0; i < 4; i++) {
		  memcpy(&grid[x+i][y], itos(ABC[t][r][i]), 4);
		}
	}
	display();
}

const char *itos(unsigned short i){
	if (i==0) strcpy(s, "----");
	else if (i==1) strcpy(s, "---@");
	else if (i==2) strcpy(s, "@---");
	else strcpy(s, "@@@@"); // 3
	return s;
}

void display() {
	putchar('+');
	for (j = 0; j < 32; j++) putchar(j % 4 + 48);
	putchar('\n');
	for (i = 0; i < 32; i++) {
		putchar(i % 4 + 48);
		for (j = 0; j < 32; j++)
			putchar(grid[i][j] == 0 ? ' ' : grid[i][j]);
		putchar('\n');
	}
}
