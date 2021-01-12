#include "Header.h"

int* newmap = 0x00;

////////////////////////////////////////////////////////////////////////////////////

int attn[7];
int pred1[7];
int pred2[7];
int pred3[7];
int pred4[7];

void predv(int W) {
	pred1[0] = W;
	attn[0] = pred2[0] = pred1[0] + W - 3;

	pred1[2] = pred1[1] = W - 2;
	pred2[2] = pred2[1] = pred1[1] + W - 1;
	pred3[2] = pred3[1] = pred2[1] + W - 2;
	attn[2] = pred4[2] = attn[1] = pred4[1] = pred3[1] + W - 1;

	attn[3] = pred1[3] = W - 1;

	pred1[4] = W - 2;
	attn[4] = pred2[4] = pred1[4] + W - 1;

	pred1[5] = W - 2;
	pred2[5] = pred1[5] + W - 1;
	pred3[5] = pred2[5] + W - 2;
	attn[5] = pred4[5] = pred3[5] + W - 1;

	pred1[6] = W - 2;
	attn[6] = pred2[6] = pred1[6] + W - 1;
}

////////////////////////////////////////////////////////////////////////////////////

void copy_map(int* map, int* newmap, int W, int H) {
	for (int i = 0; i < W; i++)
		for (int j = 0; j < H; j++)
			*(newmap + i * H + j) = *(map + i * H + j);
}

int CHK_edges(int* newmap, Position* s, int H) {
	for (int i = 0; i < 4; i++) {
		if (s[0].y <= 0 || *(newmap + s[0].x * H + s[0].y - 1) == 1) return 0;
		if (s[1].y <= 0 || *(newmap + s[1].x * H + s[1].y - 1) == 1) return 0;
		if (s[2].y <= 0 || *(newmap + s[2].x * H + s[2].y - 1) == 1) return 0;
		if (s[3].y <= 0 || *(newmap + s[3].x * H + s[3].y - 1) == 1) return 0;
	}
	return 1;
}

int CheckFullLine(int* newmap, int W, int H) {                   // ïðîâåðêà íà çàïîëíåííóþ ëèíèþ
	int f;
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

	if (s[0].y > h) h = s[0].y;
	if (s[1].y > h) h = s[1].y;
	if (s[2].y > h) h = s[2].y;
	if (s[3].y > h) h = s[3].y;

	return h;
}

void Try(int block, int place, float* attempts, int* newmap, int W, int H, int rotation, int number, int strategy) {
	int wellscoef = 0;
	Position s[4];
	switch (block) {
	case 0:										// I
		if (rotation == 0) {
			s[0].y = s[1].y = s[2].y = s[3].y = H - 1;

			int t = place;
			s[0].x = t++;
			s[1].x = t++;
			s[2].x = t++;
			s[3].x = t;
			break;
		}
		else {
			s[0].x = s[1].x = s[2].x = s[3].x = place;

			int t = H;
			s[0].y = --t;
			s[1].y = --t;
			s[2].y = --t;
			s[3].y = --t;
			if (place == W - 1) wellscoef = 2;
			break;
		}
	case 1:                                     // J
		if (rotation == 0) {
			int t = place;
			s[0].x = s[1].x = t;
			s[2].x = ++t;
			s[3].x = ++t;

			t = H;
			s[0].y = --t;
			s[3].y = s[2].y = s[1].y = --t;
			break;
		}
		if (rotation == 1) {
			s[0].x = s[2].x = s[3].x = place;
			s[1].x = place + 1;

			int t = H;
			s[1].y = s[0].y = --t;
			s[2].y = --t;
			s[3].y = --t;
			break;
		}
		if (rotation == 2) {
			int t = place;
			s[0].x = t;
			s[1].x = ++t;
			s[3].x = s[2].x = ++t;

			t = H;
			s[2].y = s[1].y = s[0].y = --t;
			s[3].y = --t;
			break;
		}
		if (rotation == 3) {
			s[1].x = s[2].x = s[3].x = place + 1;
			s[0].x = place;

			int t = H;
			s[3].y = --t;
			s[2].y = --t;
			s[0].y = s[1].y = --t;
			break;
		}
	case 2:                                     // L
		if (rotation == 0) {
			int t = place;
			s[0].x = t;
			s[1].x = ++t;
			s[3].x = s[2].x = ++t;

			t = H;
			s[3].y = --t;
			s[0].y = s[1].y = s[2].y = --t;
			break;
		}
		if (rotation == 1) {
			s[0].x = s[1].x = s[2].x = place;
			s[3].x = place + 1;

			int t = H;
			s[0].y = --t;
			s[1].y = --t;
			s[3].y = s[2].y = --t;
			break;
		}
		if (rotation == 2) {
			int t = place;
			s[1].x = s[0].x = t;
			s[2].x = ++t;
			s[3].x = ++t;

			t = H;
			s[2].y = s[3].y = s[1].y = --t;
			s[0].y = --t;
			break;
		}
		if (rotation == 3) {
			int t = place;
			s[0].x = t;
			s[3].x = s[2].x = s[1].x = ++t;

			t = H;
			s[0].y = s[1].y = --t;
			s[2].y = --t;
			s[3].y = --t;
			break;
		}
	case 3:                                     // O
		s[1].x = s[0].x = place;
		s[3].x = s[2].x = place + 1;

		int t = H;
		s[3].y = s[0].y = --t;
		s[2].y = s[1].y = --t;
		break;
	case 4:                                     // S
		if (rotation == 0) {
			int t = place;
			s[0].x = t;
			s[2].x = s[1].x = ++t;
			s[3].x = ++t;

			t = H;
			s[3].y = s[2].y = --t;
			s[1].y = s[0].y = --t;
			break;
		}
		else {
			s[1].x = s[0].x = place;
			s[3].x = s[2].x = place + 1;

			int t = H;
			s[0].y = --t;
			s[2].y = s[1].y = --t;
			s[3].y = --t;
			break;
		}
	case 5:                                     // T
		if (rotation == 0) {
			int t = place;
			s[0].x = t;
			s[2].x = s[1].x = ++t;
			s[3].x = ++t;

			t = H;
			s[2].y = --t;
			s[3].y = s[1].y = s[0].y = --t;
			break;
		}
		if (rotation == 1) {
			s[3].x = s[1].x = s[0].x = place;
			s[2].x = place + 1;

			int t = H;
			s[0].y = --t;
			s[2].y = s[1].y = --t;
			s[3].y = --t;
			break;
		}
		if (rotation == 2) {
			int t = place;
			s[0].x = t;
			s[2].x = s[1].x = ++t;
			s[3].x = ++t;

			t = H;
			s[3].y = s[0].y = s[1].y = --t;
			s[2].y = --t;
			break;
		}
		if (rotation == 3) {
			s[0].x = place;
			s[3].x = s[1].x = s[2].x = place + 1;

			int t = H;
			s[1].y = --t;
			s[2].y = s[0].y = --t;
			s[3].y = --t;
			break;
		}
	case 6:                                     // Z
		if (rotation == 0) {
			int t = place;
			s[0].x = t;
			s[2].x = s[1].x = ++t;
			s[3].x = ++t;

			t = H;
			s[1].y = s[0].y = --t;
			s[3].y = s[2].y = --t;
			break;
		}
		else {
			s[1].x = s[0].x = place;
			s[3].x = s[2].x = place + 1;

			int t = H;
			s[3].y = --t;
			s[2].y = s[1].y = --t;
			s[0].y = --t;
			break;
		}
	}
	if (place == W - 1) wellscoef--;

	if ((*(newmap + s[0].x * H + s[0].y) == 1) || (*(newmap + s[1].x * H + s[1].y) == 1) || (*(newmap + s[2].x * H + s[2].y) == 1) || (*(newmap + s[3].x * H + s[3].y) == 1)) {
		attempts[number] = 99999999.0;
		return;
	}

	while (CHK_edges(newmap, s, H) != 0) {
		s[0].y--;
		s[1].y--;
		s[2].y--;
		s[3].y--;
	}

	*(newmap + s[0].x * H + s[0].y) = 1;
	*(newmap + s[1].x * H + s[1].y) = 1;
	*(newmap + s[2].x * H + s[2].y) = 1;
	*(newmap + s[3].x * H + s[3].y) = 1;

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
	static int a = 0;
	if (!a) {
		predv(W);
		a++;
	}

	float* attempts = (float*)malloc(attn[block] * sizeof(float));
	memset(newmap, 0, 4);
	int rotation = 0; int place = 0;

	for (int i = 0; i < attn[block]; i++) {
		if (i < pred1[block]) { rotation = 0; place = i; }
		else if (i < pred2[block]) { rotation = 1; place = i - pred1[block]; }
		else if (i < pred3[block]) { rotation = 2; place = i - pred2[block]; }
		else if (i < pred4[block]) { rotation = 3; place = i - pred3[block]; }

		copy_map(map, newmap, W, H);
		Try(block, place, attempts, newmap, W, H, rotation, i, strategy);
		memset(newmap, 0, 4);
	}

	int maximum = find_min(attempts, attn[block]);
	if (maximum < pred1[block]) { rotation = 0; place = maximum; }
	else if (maximum < pred2[block]) { rotation = 1; place = maximum - pred1[block]; }
	else if (maximum < pred3[block]) { rotation = 2; place = maximum - pred2[block]; }
	else if (maximum < pred4[block]) { rotation = 3; place = maximum - pred3[block]; }
	Try(block, place, attempts, map, W, H, rotation, -1, strategy);
}

void INI() {
	newmap = (int*)malloc(40000);
}