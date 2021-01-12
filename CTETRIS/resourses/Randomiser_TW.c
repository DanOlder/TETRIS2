#include "Header.h"
#include <stdlib.h>
int bag[14];
int max = 14;

void fillBag() {
	int j = 0;
	for (int i = 0; i < 14; i++) {
		bag[i] = j;
		j++;
		if (j == 7) j = 0;
	}
}

int random_block_TW() {
	int number, block;
	if (max == 0) {
		max = 14;
		fillBag();
	}
	number = rand() % max;
	block = bag[number];
	bag[number] = bag[max - 1];
	max--;
	return block;
}