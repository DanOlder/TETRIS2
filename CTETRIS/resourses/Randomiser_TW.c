#include "Header.h"
#include <stdlib.h>
int bag[14];
int max = 14;

void fillBag() {
	__asm {
		lea eax, bag
		xor ecx, ecx
		xor edx, edx
		lp :
		mov[eax + edx * 4], ecx
			inc ecx
			inc edx
			cmp ecx, 7
			jne end
			xor ecx, ecx
			end :
		cmp edx, 14
			jl lp
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