#include "Header.h"

// ¬ файле сохранение хранитс€ следующим образом: <leveltype><score>YYYYMMDDHHMMSS;.x0.y0.x1.y1.x2.y2.x3.y3.colortype.blocktype;<map>.W.H

int checksave_TAKESAVE(int checkcmd, Date* savedate, char* level, int* score, int* Blocktype, int* color) {
	char SAVE[26] = "saves/gamesaves/save";
	char number[6] = { 0x00, '.','t', 'x', 't', '\0' };
	number[0] = checkcmd + '0';
	FILE* f;
	if ((f = fopen(strcat(SAVE, number), "r")) == 0x00)
		return 0;
	char temp[10000];
	*level = fgetc(f);
	int i = 0;
	while (!feof(f)) {
		temp[i] = fgetc(f);
		i++;
	}
	while (temp[i] != ';') {
		i--;
	}
	i--;
	*Blocktype = temp[i] - '0';
	*color = temp[i - 2] - '0';
	while (temp[i] != ';') {
		i--;
	}
	i--;
	savedate->sec = temp[i] - '0' + (temp[i - 1] - '0') * 10; i -= 2;
	savedate->min = temp[i] - '0' + (temp[i - 1] - '0') * 10; i -= 2;
	savedate->hour = temp[i] - '0' + (temp[i - 1] - '0') * 10; i -= 2;
	savedate->day = temp[i] - '0' + (temp[i - 1] - '0') * 10; i -= 2;
	savedate->mon = temp[i] - '0' + (temp[i - 1] - '0') * 10; i -= 2;
	savedate->year = temp[i] - '0' + (temp[i - 1] - '0') * 10 + (temp[i - 2] - '0') * 100 + (temp[i - 3] - '0') * 1000; i -= 4;
	int d = 1;
	*score = 0;
	while (i >= 0) {
		*score += (temp[i] - '0') * d;
		i--;
		d *= 10;
	}
	fclose(f);
	return 1;
}

int checksave_exist(int checkcmd) {
	char SAVE[26] = "saves/gamesaves/save";
	char number[6] = { 0x00, '.','t', 'x', 't', '\0' };
	int len = 0;
	number[0] = checkcmd + '0';
	FILE* f;
	if ((f = fopen(strcat(SAVE, number), "r")) == 0x00)
		return 0;
	return 1;
}

void Save2File(int checkcmd, int score, char lvl, int colortype, Position* pos, int blocktype, int* map, int W, int H) {
	char SAVE[26] = "saves/gamesaves/save";
	char number[6] = { 0x00, '.','t', 'x', 't', '\0' };
	number[0] = (checkcmd + 1) + '0';
	FILE* f = fopen(strcat(SAVE, number), "w");

	time_t t = time(0x00);
	struct tm* aTm = localtime(&t);
	fprintf(f, "%c", lvl);
	fprintf(f, "%d%04d%02d%02d%02d%02d%02d;.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d;", score, aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec, pos[0].x, pos[0].y, pos[1].x, pos[1].y, pos[2].x, pos[2].y, pos[3].x, pos[3].y, colortype, blocktype);
	for (int i = H - 1; i >= 0; i--)
		for (int j = W - 1; j >= 0; j--) {
			fprintf(f, "%d", *(map + j * H + i));
		}
	fprintf(f, ".%d.%d", W, H);
	fclose(f);
}

void LoadFromFile(int checkcmd, int* score, char* lvl, int* colortype, Position* pos, int* blocktype, int* map, int* W, int* H) {
	char SAVE[26] = "saves/gamesaves/save";
	char number[6] = { 0x00, '.','t', 'x', 't', '\0' };
	int d = 1, i = 0, len = 0;
	*W = 0; *H = 0;
	number[0] = (checkcmd + 1) + '0';
	FILE* f = fopen(strcat(SAVE, number), "r");
	char temp[10000];
	while (!feof(f)) {
		temp[i] = fgetc(f);
		i++;
	}
	i -= 2;
	while (temp[i] != '.') {
		*H += (temp[i] - '0') * d;
		d *= 10;
		i--;
	}
	i--;
	d = 1;
	while (temp[i] != '.') {
		*W += (temp[i] - '0') * d;
		d *= 10;
		i--;
	}
	i--;
	d = 1;
	for (int k = 0; k < *H; k++)
		for (int j = 0; j < *W; j++) {
			*(map + j * *H + k) = temp[i] - '0';
			i--;
		}
	i--;
	*blocktype = temp[i] - '0';
	i -= 2;
	*colortype = temp[i] - '0';
	i -= 2;
	for (int k = 0; k < 4; k++) {
		pos[k].x = 0;
		pos[k].y = 0;
	}
	for (int k = 3; k >= 0; k--) {     //  сначала y, потом x
		d = 1;
		while (temp[i] != '.') {
			pos[k].y += (temp[i] - '0') * d;
			d *= 10;
			i--;
		}
		i--;
		d = 1;
		while (temp[i] != '.') {
			pos[k].x += (temp[i] - '0') * d;
			d *= 10;
			i--;
		}
		i--;
	}
	*lvl = temp[0];
	i -= 15;
	d = 1;
	*score = 0;
	while (i >= 1) {
		*score += (temp[i] - '0') * d;
		i--;
		d *= 10;
	}
	fclose(f);
}

void printtable(int number, char* name, int score) {
	char SAVE[34] = "saves/tablesaves/savetable";
	char num[6] = { 0x00, '.','t', 'x', 't', '\0' };
	num[0] = number + '0';
	FILE* f;
	f = fopen(strcat(SAVE, num), "w");
	fprintf(f, "%s;%d", name, score);
	fclose(f);
}

bool checktable(int number, char* name, int score) {
	if (number == 9) return false;
	char SAVE[34] = "saves/tablesaves/savetable";
	char num[6] = { 0x00, '.','t', 'x', 't', '\0' };

	num[0] = number + '0';
	FILE* f;
	if ((f = fopen(strcat(SAVE, num), "r+")) == 0x00) {
		printtable(number, name, score);
		return true;
	}
	else {
		char tmp[50];
		int i = 0;
		int savescore = 0;
		char savename[21];
		int d = 1;
		while (!feof(f)) {
			tmp[i] = getc(f);
			i++;
		}
		i -= 2;
		while (tmp[i] != ';') {
			savescore += (tmp[i] - '0') * d;
			i--;
			d *= 10;
		}
		i = 0;
		while (tmp[i] != ';') {
			savename[i] = tmp[i];
			i++;
		}
		savename[i] = 0x00;
		if (score >= savescore) {
			printtable(number, name, score);
			checktable(number + 1, savename, savescore);
			fclose(f);
			return true;
		}
		else if (score < savescore)
			if (checktable(number + 1, name, score)) {
				fclose(f);
				return true;
			}
	}
	fclose(f);
	return false;  // подумать
}

bool  savetable(char* savename, int savescore) {
	if (checktable(1, savename, savescore)) {
		return true;
	}
	return false;
}

bool unlockTheme() {
	int isunlock = 0;
	FILE* f = fopen("saves/themes/unlock.txt", "r");
	isunlock = fgetc(f);
	fclose(f);
	if (isunlock == 48) return false;
	else if (isunlock == 49)  return true;
	else return false;
}

void setunlock() {
	FILE* f = fopen("saves/themes/unlock.txt", "w");
	fprintf(f, "%c", '1');
	fclose(f);
}