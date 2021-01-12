#include "Header.h"

int x0 = 0;
int pool[35];
int Drought[7] = { 0,0,0,0,0,0,0 };
int History[4] = { 4, 6, 4, 6 };    // шанс Z и S меньше
int history_pos = 0;

int droughtBlock() {
	int d = Drought[0];
	int num = 0;
	for (int i = 0; i < 7; i++) {
		if (Drought[i] > d) {
			d = Drought[i];
			num = i;
		}
	}
	return num;
}

bool history_includes(int block) {
	for (int i = 0; i < 4; i++) {
		if (History[i] == block) return true;
	}
	return false;
}

void setNewHistoryBlock(int block) {
	for (int i = 0; i < 3; i++)
		History[i] = History[i + 1];
	History[3] = block;
}

int random_block_TGM3() {
	int block;
	int number;
	for (int i = 0; i < 6; i++) {
		number = rand() % 35;
		if (history_includes(pool[number]) == false)
			break;
	}
	block = pool[number];
	setNewHistoryBlock(block);
	Drought[pool[number]] = 0;
	for (int i = 0; i < 7; i++) {
		if (i != pool[number])
			Drought[i]++;
	}
	pool[number] = droughtBlock();
	return block;
}

void fillPool() {
	int j = 0;
	for (int i = 0; i < 35; i++) {
		pool[i] = j;
		j++;
		if (j == 7) j = 0;
	}
}