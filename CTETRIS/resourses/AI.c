#include "Header.h"

int* newmap = 0x00;

void copy_map(int* map, int* newmap, int W, int H) {
	for (int i = 0; i < W; i++)
		for (int j = 0; j < H; j++)
			*(newmap + i * H + j) = *(map + i * H + j);
}

int CHK_edges(int* newmap, Position* s, int H) {
	for (int i = 0; i < 4; i++) {
		if (s[i].y <= 0 || *(newmap + s[i].x * H + s[i].y - 1) == 1) return 0;
	}
	return 1;
}

int CheckFullLine(int* newmap, int W, int H) {                   // ïðîâåðêà íà çàïîëíåííóþ ëèíèþ
	int f = 0;
	int line = 0;
	for (int j = 0; j < H; j++) {
		f = 0;
		for (int i = 0; i < W; i++) {
			if (*(newmap + i * H + j) == 1) f++;
		}
		if (f == W) {
			line++;
		}
	}
	return line;
}

int CheckHoles(int* newmap, int W, int H) {  // ïîèñê êëåòîê-êîëîäöåâ
	int hole = 0;

	for (int i = 0; i < W; i++) {
		int j = H - 1;
		while (*(newmap + i * H + j) == 0 && j > 1) j--;
		j--;
		while (j >= 0) {
			if (*(newmap + i * H + j) == 0) hole++;
			j--;
		}
	}
	return hole;
}

int Check_wells(int* newmap, int W, int H) {
	int well = 0;
	int busy = 0;
	int check = 0;
	for (int i = 0; i < W; i++) {
		busy = 0;
		check = 0;
		for (int j = H - 1; j >= 0; j--) {
			if (*(newmap + i * H + j) == 0 && busy == 0) {
				if (*(newmap + (i - 1) * H + j) == 1 || i == 0) {
					if (*(newmap + (i + 1) * H + j) == 1 || i == W - 1) { well++; check++; }
				}
			}
			else busy = 1;
		}
		if (busy == 0) well -= check;
	}
	return well;
}

int transition_column(int* newmap, int W, int H) {   // ïåðåõîäû â ñòîëáöàõ
	int trans = 0;
	for (int i = 0; i < W; i++) {
		for (int j = 0; j < H; j++) {
			if (*(newmap + i * H + j) == 0) {
				if (*(newmap + i * H + j + 1) == 1) trans++;
			}
			else if (*(newmap + i * H + j + 1) == 0) trans++;
		}
	}
	trans--;    // óáèðàåì ñàìûé âåðõíèé ñðàâíèâàåìûé áëîê
	return trans;
}

int transition_line(int* newmap, int W, int H) {   // ïåðåõîäû â ñòðîêàõ
	int trans = 0;
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			if (*(newmap + i * H + j) == 0) {
				if (*(newmap + (i + 1) * H + j) == 1 || i == 0 || i == W - 1) trans++;
			}
			else if (*(newmap + (i + 1) * H + j) == 0) trans++;
		}
	}
	return trans;
}

int Findmax_Pos(Position* s) {
	int h = s[0].y;
	for (int i = 1; i < 4; i++) {
		if (s[i].y > h) h = s[i].y;
	}
	return h;
}

void Try(int block, int place, float* attempts, int* newmap, int W, int H, int rotation, int number, int strategy) {
	int wellscoef = 0;
	Position s[4];
	switch (block) {
	case 0:										// I
		if (rotation == 0) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place + 1;
			s[1].y = H - 1;
			s[2].x = place + 2;
			s[2].y = H - 1;
			s[3].x = place + 3;
			s[3].y = H - 1;
		}
		else if (rotation == 1) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place;
			s[1].y = H - 2;
			s[2].x = place;
			s[2].y = H - 3;
			s[3].x = place;
			s[3].y = H - 4;
			if (place == W - 1) wellscoef = 2;
		}
		break;
	case 1:                                     // J
		if (rotation == 0) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place;
			s[1].y = H - 2;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place + 2;
			s[3].y = H - 2;
		}
		else if (rotation == 1) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place + 1;
			s[1].y = H - 1;
			s[2].x = place;
			s[2].y = H - 2;
			s[3].x = place;
			s[3].y = H - 3;
		}
		else if (rotation == 2) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place + 1;
			s[1].y = H - 1;
			s[2].x = place + 2;
			s[2].y = H - 1;
			s[3].x = place + 2;
			s[3].y = H - 2;
		}
		else if (rotation == 3) {
			s[0].x = place;
			s[0].y = H - 3;
			s[1].x = place + 1;
			s[1].y = H - 3;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place + 1;
			s[3].y = H - 1;
		}
		break;
	case 2:                                     // L
		if (rotation == 0) {
			s[0].x = place;
			s[0].y = H - 2;
			s[1].x = place + 1;
			s[1].y = H - 2;
			s[2].x = place + 2;
			s[2].y = H - 2;
			s[3].x = place + 2;
			s[3].y = H - 1;
		}
		else if (rotation == 1) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place;
			s[1].y = H - 2;
			s[2].x = place;
			s[2].y = H - 3;
			s[3].x = place + 1;
			s[3].y = H - 3;
		}
		else if (rotation == 2) {
			s[0].x = place;
			s[0].y = H - 2;
			s[1].x = place;
			s[1].y = H - 1;
			s[2].x = place + 1;
			s[2].y = H - 1;
			s[3].x = place + 2;
			s[3].y = H - 1;
		}
		else if (rotation == 3) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place + 1;
			s[1].y = H - 1;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place + 1;
			s[3].y = H - 3;
		}
		break;
	case 3:                                     // O
		s[0].x = place;
		s[0].y = H - 1;
		s[1].x = place;
		s[1].y = H - 2;
		s[2].x = place + 1;
		s[2].y = H - 2;
		s[3].x = place + 1;
		s[3].y = H - 1;
		break;
	case 4:                                     // S
		if (rotation == 0) {
			s[0].x = place;
			s[0].y = H - 2;
			s[1].x = place + 1;
			s[1].y = H - 2;
			s[2].x = place + 1;
			s[2].y = H - 1;
			s[3].x = place + 2;
			s[3].y = H - 1;
		}
		else if (rotation == 1) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place;
			s[1].y = H - 2;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place + 1;
			s[3].y = H - 3;
		}
		break;
	case 5:                                     // T
		if (rotation == 0) {
			s[0].x = place;
			s[0].y = H - 2;
			s[1].x = place + 1;
			s[1].y = H - 2;
			s[2].x = place + 1;
			s[2].y = H - 1;
			s[3].x = place + 2;
			s[3].y = H - 2;
		}
		else if (rotation == 1) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place;
			s[1].y = H - 2;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place;
			s[3].y = H - 3;
		}
		else if (rotation == 2) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place + 1;
			s[1].y = H - 1;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place + 2;
			s[3].y = H - 1;
		}
		else if (rotation == 3) {
			s[0].x = place;
			s[0].y = H - 2;
			s[1].x = place + 1;
			s[1].y = H - 1;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place + 1;
			s[3].y = H - 3;
		}
		break;
	case 6:                                     // Z
		if (rotation == 0) {
			s[0].x = place;
			s[0].y = H - 1;
			s[1].x = place + 1;
			s[1].y = H - 1;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place + 2;
			s[3].y = H - 2;
		}
		else if (rotation == 1) {
			s[0].x = place;
			s[0].y = H - 3;
			s[1].x = place;
			s[1].y = H - 2;
			s[2].x = place + 1;
			s[2].y = H - 2;
			s[3].x = place + 1;
			s[3].y = H - 1;
		}
		break;
	}
	if (place == W - 1) wellscoef--;
	for (int i = 0; i < 4; i++) {
		if (*(newmap + s[i].x * H + s[i].y) == 1) { attempts[number] = 99999999.0; return; }
	}
	while (CHK_edges(newmap, s, H) != 0) {
		for (int i = 0; i < 4; i++)
			s[i].y--;
	}
	for (int i = 0; i < 4; i++) {
		*(newmap + s[i].x * H + s[i].y) = 1;
	}
	if (number == -1) return;
	else {
		if (strategy == 0) wellscoef = 0;
		attempts[number] = (12.885008263218383 * (float)Findmax_Pos(s) + 1.00000 * (float)CheckFullLine(newmap, W, H) + 15.842707182438396 * (float)Check_wells(newmap, W, H) + 26.894496507795950 * (float)CheckHoles(newmap, W, H) + 27.616914062397015 * (float)transition_column(newmap, W, H) + 30.185110719279040 * (float)transition_line(newmap, W, H) - 2.0 * (float)wellscoef);
	}
}

int find_min(float* attempts, int max) {
	int maximum = 0;
	float max_att = attempts[0];
	for (int i = 0; i < max; i++) {
		if (attempts[i] < max_att) {
			max_att = attempts[i];
			maximum = i;
		}
	}
	return maximum;
}

void alloc(int* map, int W, int H, int block, int strategy) {
	int sq[4];    //êîëè÷åñòâî êâàäðàòîâ â øèðèíó ïðè îïðåäåëåííîì ïîâîðîòå
	sq[0] = W + 1; sq[1] = W + 1; sq[2] = W + 1; sq[3] = W + 1;
	switch (block) {
	case 0:                                     // I
		sq[0] = 1;
		sq[1] = 4;
		break;
	case 1:                                     // J
		sq[0] = 3;
		sq[1] = 2;
		sq[2] = 3;
		sq[3] = 2;
		break;
	case 2:                                     // L
		sq[0] = 3;
		sq[1] = 2;
		sq[2] = 3;
		sq[3] = 2;
		break;
	case 3:                                     // O
		sq[0] = 2;
		break;
	case 4:                                     // S
		sq[0] = 3;
		sq[1] = 2;
		break;
	case 5:                                     // T
		sq[0] = 3;
		sq[1] = 2;
		sq[2] = 3;
		sq[3] = 2;
		break;
	case 6:                                     // Z
		sq[0] = 3;
		sq[1] = 2;
		break;
	}

	int att_number = 4 * W - (sq[0] + sq[1] + sq[2] + sq[3] - 4);
	float* attempts = (float*)malloc(att_number * sizeof(float));
	memset(newmap, 0, sizeof(int));
	int rotation = 0; int place = 0;

	for (int i = 0; i < att_number; i++) {
		if (i < W - (sq[0] - 1)) { rotation = 0; place = i; }
		else if (i < (W - (sq[0] - 1) + W - (sq[1] - 1))) { rotation = 1; place = i - (W - (sq[0] - 1)); }
		else if (i < (W - (sq[0] - 1) + W - (sq[1] - 1) + W - (sq[2] - 1))) { rotation = 2; place = i - (W - (sq[0] - 1) + W - (sq[1] - 1)); }
		else if (i < (W - (sq[0] - 1) + W - (sq[1] - 1) + W - (sq[2] - 1) + W - (sq[3] - 1))) { rotation = 3; place = i - (W - (sq[0] - 1) + W - (sq[1] - 1) + W - (sq[2] - 1)); }

		copy_map(map, newmap, W, H);
		Try(block, place, attempts, newmap, W, H, rotation, i, strategy);
		memset(newmap, 0, sizeof(int));
	}

	int maximum = find_min(attempts, att_number);
	if (maximum < W - (sq[0] - 1)) { rotation = 0; place = maximum; }
	else if (maximum < (W - (sq[0] - 1) + W - (sq[1] - 1))) { rotation = 1; place = maximum - (W - (sq[0] - 1)); }
	else if (maximum < (W - (sq[0] - 1) + W - (sq[1] - 1) + W - (sq[2] - 1))) { rotation = 2; place = maximum - (W - (sq[0] - 1) + W - (sq[1] - 1)); }
	else if (maximum < (W - (sq[0] - 1) + W - (sq[1] - 1) + W - (sq[2] - 1) + W - (sq[3] - 1))) { rotation = 3; place = maximum - (W - (sq[0] - 1) + W - (sq[1] - 1) + W - (sq[2] - 1)); }
	Try(block, place, attempts, map, W, H, rotation, -1, strategy);
}

void INI() {
	newmap = (int*)malloc(100 * 100 * sizeof(int));
}