#include "resourses/Header.h"
///////////////////////////////////////////////INITIALIZATION///////////////////////////////////////////////////////////////////////////////////////////////
int W = 10, H = 20;   // 10 20
int scale = 30; // размер квадрата 25

const int normalW = 10;
const int normalH = 20;
const int normalscale = 30;   // 30
int levelnumber = 1, speed = 10, score = 0, difficult = 2;
float levelfactor = 0;
int tic = 0;
int animationfac = 30;
int randomizer_type = 0;

int newfigure = 0, block;

Color newcolor;
Position s[4];
Position A[2000];
int* map = 0x00;

/////////FOR TEXT///////////////////////
unsigned char getkey = 0x00;
char name[22];
int name_i = 0;
int change;
////////////////////////////////////////

////////////////FOR MENU////////////////
CMD command = menu;   // menu

int menuintcmd = 0;
int pauseintcmd = 0;
int mini = 0;
int settingscmd = 0;
int gmcmd = 0;
////////////////////////////////////////

/////////////FOR THEMES////////////////
int activeTheme = 0;
int selectTheme = 0;
///////////////////////////////////////

/////////////FOR SAVE//////////////////
int svmcmd = 0;  // для сохранения и загрузки
char savemode = 'l';
int newrecord = 0;
//////////////////////////////////////

//new init optimization//dan
int botplaing = 0;
int schet = 0;
time_t bottime;
FILE* arch;
char filename[] = "arch.txt";

///////////////////////////////////////////////END OF INITIALIZATION////////////////////////////////////////////////////////////////////////////////////

void refreshmap(int* map, int W, int H) {
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			*(map + i * 100 + j) = 0;
}

int CalcScore(int lvl) {
	int f = 0;
	while (lvl > 0) {
		f = f + lvl * 1000; lvl--;
	}
	return f;
}

void WinAnimation(int what) {
	if (what == 0) {
		for (int i = 0; i < 2000; i++) {
			A[i].x = rand() % (W * scale);
			A[i].y = rand() % (H + 10) * scale + H * scale;
		}
	}
	else if (what == 1) {
		for (int i = 0; i < 1000; i++) {
			if (A[i].y < 0) A[i].y = (H + 1) * scale;
			A[i].x += rand() % (scale / 4);
			A[i].y += rand() % 3;
			A[i].x -= rand() % (scale / 4);
			A[i].y -= rand() % 4;
		}
	}
}

void SaveColor() {
	switch (newcolor.colorBlock) {
	case 0:
		newcolor.R = 150;
		newcolor.G = 0;
		newcolor.B = 0;
		break;
	case 1:
		newcolor.R = 0;
		newcolor.G = 150;
		newcolor.B = 0;
		break;
	case 2:
		newcolor.R = 0;
		newcolor.G = 0;
		newcolor.B = 150;
		break;
	case 3:
		newcolor.R = 150;
		newcolor.G = 150;
		newcolor.B = 0;
		break;
	case 4:
		newcolor.R = 0;
		newcolor.G = 150;
		newcolor.B = 150;
		break;
	case 5:
		newcolor.R = 150;
		newcolor.G = 0;
		newcolor.B = 150;
		break;
	}
}

void SaveBlock() {
	switch (block) {
	case 0:                                     // I
		s[0].x = W / 2 - 2, s[0].y = H - 1;
		s[1].x = W / 2 - 1, s[1].y = H - 1;
		s[2].x = W / 2, s[2].y = H - 1;
		s[3].x = W / 2 + 1, s[3].y = H - 1;
		break;
	case 1:                                     // J
		s[0].x = W / 2 - 2, s[0].y = H - 1;
		s[1].x = W / 2 - 2, s[1].y = H - 2;
		s[2].x = W / 2 - 1, s[2].y = H - 2;
		s[3].x = W / 2, s[3].y = H - 2;
		break;
	case 2:                                     // L
		s[0].x = W / 2 - 2, s[0].y = H - 2;
		s[1].x = W / 2 - 1, s[1].y = H - 2;
		s[2].x = W / 2, s[2].y = H - 2;
		s[3].x = W / 2, s[3].y = H - 1;
		break;
	case 3:                                     // O
		s[0].x = W / 2 - 1, s[0].y = H - 1;
		s[1].x = W / 2, s[1].y = H - 1;
		s[2].x = W / 2 - 1, s[2].y = H - 2;
		s[3].x = W / 2, s[3].y = H - 2;
		break;
	case 4:                                     // S
		s[0].x = W / 2 - 2, s[0].y = H - 2;
		s[1].x = W / 2 - 1, s[1].y = H - 2;
		s[2].x = W / 2 - 1, s[2].y = H - 1;
		s[3].x = W / 2, s[3].y = H - 1;
		break;
	case 5:                                     // T
		s[0].x = W / 2 - 2, s[0].y = H - 2;
		s[1].x = W / 2 - 1, s[1].y = H - 2;
		s[2].x = W / 2 - 1, s[2].y = H - 1;
		s[3].x = W / 2, s[3].y = H - 2;
		break;
	case 6:                                     // Z
		s[0].x = W / 2 - 2, s[0].y = H - 1;
		s[1].x = W / 2 - 1, s[1].y = H - 1;
		s[2].x = W / 2 - 1, s[2].y = H - 2;
		s[3].x = W / 2, s[3].y = H - 2;
		break;
	}
}

void randomblock() {
	if (botplaing) {
		//	newcolor.colorBlock = rand() % 6; // 6
		block = fgetc(arch) - 48;

		//	SaveColor();
		SaveBlock();
	}
	else {
		newcolor.colorBlock = rand() % 6; // 6
		block = (randomizer_type == 0) ? random_block_TGM3() : random_block_TW();

		SaveColor();
		SaveBlock();
	}
}

void move(int dir) {
	switch (dir) {
	case -1:
		for (int i = 0; i < 4; i++) s[i].y--; break;
	case 1:
		for (int i = 0; i < 4; i++) s[i].y++; break;
	case 2:
		for (int i = 0; i < 4; i++) s[i].x++; break;
	case -2:
		for (int i = 0; i < 4; i++) s[i].x--; break;
	}
}

int edges(int dir) {
	if (dir == 0) {
		for (int i = W / 2 - 2; i < W / 2; i++) {
			if (*(map + i * H + H - 1) == 1) return 0;
		}
	}
	else if (dir == -1) {
		for (int i = 0; i < 4; i++) {
			if (s[i].y <= 0 || *(map + s[i].x * H + s[i].y - 1) == 1) return 0;
		}
	}
	else if (dir == 2) {
		for (int i = 0; i < 4; i++)
			if (s[i].x >= W - 1 || *(map + (s[i].x + 1) * H + s[i].y) == 1) return 0;
	}
	else if (dir == -2) {
		for (int i = 0; i < 4; i++)
			if (s[i].x <= 0 || *(map + (s[i].x - 1) * H + s[i].y) == 1) return 0;
	}
	return 1;
}

void selectDraw_GR(Color* colorFont, int cmd, int Value) {  // смотрит на активный элемент, если он доступен/работает/активен, то подсвечивается зеленым, иначе красным
	if (cmd == Value) {
		colorFont->R = 0;
		colorFont->G = 200;
		colorFont->B = 0;
	}
	else {
		colorFont->R = 200;
		colorFont->G = 0;
		colorFont->B = 0;
	}
}

void selectDraw_saveframe(Color* colorFont, int cmd, int Value, int posx0, int posy0, int posx, int posy) {			//ячейка сохранения
	if (cmd == Value) {
		DrawSaveFrame(posx0, posy0, posx, posy, 1);
		colorFont->R = 200;
		colorFont->G = 200;
		colorFont->B = 200;
	}
	else {
		DrawSaveFrame(posx0, posy0, posx, posy, 0);
		colorFont->R = 140;
		colorFont->G = 140;
		colorFont->B = 140;
	}
}

void Draw()
{
	int posx = normalscale * normalW / 2 - 55;  //
	int posy = (normalH - 1) * normalscale;  // 0.9
	if (command != level && command != freeplay && command != pause && command != botplay) DrawTetris(posx, posy);
	if (command == level || command == freeplay || command == botplay) {
		DrawStaticObj(W, H, scale, map, activeTheme);
		Color colorFont;

		colorFont.R = 200;
		colorFont.G = 0;
		colorFont.B = 0;
		colorFont.font = GLUT_BITMAP_HELVETICA_10;

		if (command == level) {
			DrawText_constchar("LEVEL:", 6, scale * W - 50, scale * H - 20, colorFont);

			glRasterPos2i(scale * W - 10, scale * H - 20);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, levelnumber + '0');
		}

		DrawText_constchar("SCORE:", 6, 3, scale * H - 20, colorFont);
		int scorecpy = score, scorelen = 0;
		while (scorecpy > 0) {
			scorecpy /= 10;
			scorelen++;
		}

		DrawText_int(score, scorelen, 43, scale * H - 20, colorFont, 8);
	}
	else if (command == pause) {
		DrawTetris(scale * W / 2 - 55, (H - 2) * scale);
		Color colorFont;

		colorFont.font = GLUT_BITMAP_9_BY_15;
		selectDraw_GR(&colorFont, pauseintcmd, 0);
		DrawText_constchar("CONTINUE", 8, scale * W / 2 - 50, (H - 2) * scale - scale * 4, colorFont);

		selectDraw_GR(&colorFont, pauseintcmd, 1);
		DrawText_constchar("SAVE", 4, scale * W / 2 - 36, (H - 2) * scale - scale * 8, colorFont);

		selectDraw_GR(&colorFont, pauseintcmd, 2);
		DrawText_constchar("EXIT", 4, scale * W / 2 - 36, (H - 2) * scale - scale * 12, colorFont);
	}
	else if (command == menu) {
		Color colorFont;
		colorFont.font = GLUT_BITMAP_9_BY_15;

		selectDraw_GR(&colorFont, menuintcmd, 0);
		DrawText_constchar("NEW GAME", 8, posx, posy - normalscale * 2, colorFont);

		selectDraw_GR(&colorFont, menuintcmd, 1);
		DrawText_constchar("LOAD GAME", 9, posx, posy - normalscale * 4, colorFont);

		selectDraw_GR(&colorFont, menuintcmd, 2);
		DrawText_constchar("LEADERBOARD", 12, posx, posy - normalscale * 6, colorFont);

		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 0;
		if (newrecord != 0)
			if (animationfac > 0)
				DrawText_constchar("NEW!", 4, posx + 110, posy - normalscale * 6, colorFont);

		selectDraw_GR(&colorFont, menuintcmd, 3);
		DrawText_constchar("CREDITS", 7, posx, posy - normalscale * 8, colorFont);

		selectDraw_GR(&colorFont, menuintcmd, 4);
		DrawText_constchar("SETTINGS", 8, posx, posy - normalscale * 10, colorFont);

		selectDraw_GR(&colorFont, menuintcmd, 5);
		DrawText_constchar("EXIT", 4, posx, posy - normalscale * 12, colorFont);
	}
	else if (command == minimenu) {
		Color colorFont;
		colorFont.font = GLUT_BITMAP_9_BY_15;
		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 0;

		DrawText_constchar("NEW GAME", 8, posx, posy - normalscale * 2, colorFont);

		selectDraw_GR(&colorFont, mini, 0);
		DrawText_constchar("LEVELS", 6, posx + 5, posy - normalscale * 4, colorFont);

		selectDraw_GR(&colorFont, mini, 1);
		DrawText_constchar("FREE PLAY", 9, posx - 3, posy - normalscale * 6, colorFont);

		selectDraw_GR(&colorFont, mini, 2);
		DrawText_constchar("BOT GAME", 8, posx - 3, posy - normalscale * 8, colorFont);
	}
	else if (command == lose || command == win) {
		Color colorFont;

		colorFont.font = GLUT_BITMAP_TIMES_ROMAN_24;
		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 0;
		if (command == lose)
			DrawText_constchar("GAME OVER!", 10, posx - 30, posy - normalscale * 2, colorFont);
		else {
			DrawText_constchar("YOU WIN!!!", 10, posx - 30, posy - normalscale * 2, colorFont); WinAnimation(1);
			for (int i = 0; i < 2000; i++) {
				glColor3f(1.0f, 1.0f, 1.0f);
				glRectf(A[i].x, A[i].y, A[i].x + 1.8, A[i].y + 1.8);
			}
		}

		colorFont.font = GLUT_BITMAP_HELVETICA_18;

		DrawText_constchar("YOUR SCORE:", 11, posx - 30, posy - normalscale * 4, colorFont);

		colorFont.font = GLUT_BITMAP_9_BY_15;
		colorFont.R = 0;
		colorFont.G = 200;
		colorFont.B = 200;

		DrawText_constchar("ENTER YOUR NAME", 15, posx - 30, posy - normalscale * 5.5, colorFont);
		DrawText_constchar("(max 20 symbols)", 16, posx - 30, posy - normalscale * 6.5, colorFont);
		DrawText_constchar("AND PRESS TAB", 13, posx - 20, posy - normalscale * 7.5, colorFont);

		if (name_i >= 20) { command = menu; name[0] = 0x00; }
		if (change != 0) {
			name[name_i] = getkey;
			change--;
			name_i++;
			if (name_i >= 20) { if (savetable(name, score))newrecord++; command = menu; }
			name[name_i] = 0x00;
		}

		DrawText_char(name, name_i, 0, posy - normalscale * 8.5, colorFont);

		int scorecpy = score, scorelen = 0;
		while (scorecpy > 0) {
			scorecpy /= 10;
			scorelen++;
		}
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 0;

		DrawText_int(score, scorelen, posx + 100, posy - normalscale * 4, colorFont, 10);
	}
	else if (command == settingsmenu) {
		Color colorFont;
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 0;

		DrawText_constchar("SETTINGS", 8, posx, posy - 60, colorFont);

		colorFont.font = GLUT_BITMAP_9_BY_15;

		selectDraw_GR(&colorFont, settingscmd, 0);
		DrawText_constchar("THEMES", 6, posx + 5, posy - 120, colorFont);

		selectDraw_GR(&colorFont, settingscmd, 1);
		DrawText_constchar("GAMEMODE", 8, posx + 5, posy - 180, colorFont);

		selectDraw_GR(&colorFont, settingscmd, 2);
		DrawText_constchar("EXIT", 4, posx + 5, posy - 240, colorFont);
	}
	else if (command == gamemodemenu) {
		Color colorFont;
		colorFont.font = GLUT_BITMAP_9_BY_15;
		colorFont.R = 200;
		colorFont.G = 0;
		colorFont.B = 0;

		DrawText_constchar("DIFFICULT:", 10, posx - 10, posy - 60, colorFont);

		selectDraw_GR(&colorFont, gmcmd, 0);
		switch (difficult) {
		case 1: DrawText_constchar("EASY", 4, posx + 90, posy - 60, colorFont); break;
		case 2: DrawText_constchar("NORMAL", 6, posx + 90, posy - 60, colorFont); break;
		case 3: DrawText_constchar("HARD", 4, posx + 90, posy - 60, colorFont); break;
		case 4: DrawText_constchar("VERYHARD", 8, posx + 90, posy - 60, colorFont); break;
		}

		colorFont.R = 200;
		colorFont.G = 0;
		colorFont.B = 0;

		DrawText_constchar("SIZE_X:", 7, posx - 5, posy - 120, colorFont);

		selectDraw_GR(&colorFont, gmcmd, 1);
		glColor3ub(colorFont.R, colorFont.G, colorFont.B);
		DrawText_int(W, 2, posx + 62, posy - 120, colorFont, 8);

		colorFont.R = 200;
		colorFont.G = 0;
		colorFont.B = 0;

		DrawText_constchar("SIZE_Y:", 7, posx - 5, posy - 180, colorFont);

		selectDraw_GR(&colorFont, gmcmd, 2);
		glColor3ub(colorFont.R, colorFont.G, colorFont.B);
		DrawText_int(H, 2, posx + 62, posy - 180, colorFont, 8);
		selectDraw_GR(&colorFont, gmcmd, 3);
		switch (randomizer_type) {
		case 0: DrawText_constchar("TGM3", 4, posx + 90, posy - 240, colorFont); break;
		case 1: DrawText_constchar("TW", 2, posx + 90, posy - 240, colorFont); break;
		}

		colorFont.R = 200;
		colorFont.G = 0;
		colorFont.B = 0;

		DrawText_constchar("GENERATOR:", 10, posx - 10, posy - 240, colorFont);

		selectDraw_GR(&colorFont, gmcmd, 4);
		DrawText_constchar("EXIT", 4, posx + 5, posy - 300, colorFont);
	}
	else if (command == creditspage) {
		Color colorFont;
		colorFont.font = GLUT_BITMAP_9_BY_15;
		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 200;

		DrawText_constchar("This game was developed", 23, posx - 50, posy - 60, colorFont);
		DrawText_constchar("by Maxim && Danila", 18, posx - 35, posy - 90, colorFont);
		DrawText_constchar("Type any button", 15, posx - 30, posy - 150, colorFont);
	}
	else if (command == savemenu || command == loadmenu) {
		Color colorFont;
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		Date newDate;
		int   checkscore = 0;
		char checkgamemode = 0x00;
		int scorecpy = 0, scorelen = 0;
		int blockminiature = 0, colorminiature = 0;

		selectDraw_saveframe(&colorFont, svmcmd, 0, 1, posy - 90, normalW * normalscale - 1, posy - 40);

		DrawText_constchar("SAVE 1:", 7, 1, posy - 60, colorFont);
		colorFont.font = GLUT_BITMAP_9_BY_15;
		if (checksave_TAKESAVE(1, &newDate, &checkgamemode, &checkscore, &blockminiature, &colorminiature) == 0)
			DrawText_constchar("EMPTY", 5, 80, posy - 60, colorFont);
		else {
			DrawMiniature(blockminiature, colorminiature, 250, posy - 80);
			scorecpy = checkscore, scorelen = 0;
			while (scorecpy > 0) {
				scorelen++;
				scorecpy /= 10;
			}
			DrawText_constchar("Score:", 6, 75, posy - 60, colorFont);
			DrawText_int(checkscore, scorelen, 130, posy - 60, colorFont, 10);
			if (checkgamemode == 'l') DrawText_constchar("level", 5, 1, posy - 80, colorFont);
			else if (checkgamemode == 'f') DrawText_constchar("freeplay", 8, 1, posy - 80, colorFont);

			DrawText_int(newDate.year, 4, 78, posy - 80, colorFont, 10);
			DrawText_constchar(".", 2, 115, posy - 80, colorFont);
			DrawText_int(newDate.mon, 2, 123, posy - 80, colorFont, 10);
			DrawText_constchar(".", 2, 140, posy - 80, colorFont);
			DrawText_int(newDate.day, 2, 148, posy - 80, colorFont, 10);

			DrawText_int(newDate.hour, 2, 174, posy - 80, colorFont, 10);
			DrawText_constchar(":", 1, 191, posy - 80, colorFont);
			DrawText_int(newDate.min, 2, 199, posy - 80, colorFont, 10);
			DrawText_constchar(":", 1, 216, posy - 80, colorFont);
			DrawText_int(newDate.sec, 2, 224, posy - 80, colorFont, 10);
		}

		selectDraw_saveframe(&colorFont, svmcmd, 1, 1, posy - 150, normalW * normalscale - 1, posy - 100);

		checkscore = 0;
		checkgamemode = 0x00;
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		DrawText_constchar("SAVE 2:", 7, 0, posy - 120, colorFont);
		colorFont.font = GLUT_BITMAP_9_BY_15;
		if (checksave_TAKESAVE(2, &newDate, &checkgamemode, &checkscore, &blockminiature, &colorminiature) == 0)
			DrawText_constchar("EMPTY", 5, 80, posy - 120, colorFont);
		else {
			DrawMiniature(blockminiature, colorminiature, 250, posy - 140);
			scorecpy = checkscore, scorelen = 0;
			while (scorecpy > 0) {
				scorelen++;
				scorecpy /= 10;
			}
			DrawText_constchar("Score:", 6, 75, posy - 120, colorFont);
			DrawText_int(checkscore, scorelen, 130, posy - 120, colorFont, 10);
			if (checkgamemode == 'l') DrawText_constchar("level", 5, 0, posy - 140, colorFont);
			else if (checkgamemode == 'f') DrawText_constchar("freeplay", 8, 0, posy - 140, colorFont);

			DrawText_int(newDate.year, 4, 78, posy - 140, colorFont, 10);
			DrawText_constchar(".", 2, 115, posy - 140, colorFont);
			DrawText_int(newDate.mon, 2, 123, posy - 140, colorFont, 10);
			DrawText_constchar(".", 2, 140, posy - 140, colorFont);
			DrawText_int(newDate.day, 2, 148, posy - 140, colorFont, 10);

			DrawText_int(newDate.hour, 2, 174, posy - 140, colorFont, 10);
			DrawText_constchar(":", 1, 191, posy - 140, colorFont);
			DrawText_int(newDate.min, 2, 199, posy - 140, colorFont, 10);
			DrawText_constchar(":", 1, 216, posy - 140, colorFont);
			DrawText_int(newDate.sec, 2, 224, posy - 140, colorFont, 10);
		}

		selectDraw_saveframe(&colorFont, svmcmd, 2, 1, posy - 210, normalW * normalscale - 1, posy - 160);

		checkscore = 0;
		checkgamemode = 0x00;
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		DrawText_constchar("SAVE 3:", 7, 0, posy - 180, colorFont);
		colorFont.font = GLUT_BITMAP_9_BY_15;
		if (checksave_TAKESAVE(3, &newDate, &checkgamemode, &checkscore, &blockminiature, &colorminiature) == 0)
			DrawText_constchar("EMPTY", 5, 80, posy - 180, colorFont);
		else {
			DrawMiniature(blockminiature, colorminiature, 250, posy - 200);
			scorecpy = checkscore, scorelen = 0;
			while (scorecpy > 0) {
				scorelen++;
				scorecpy /= 10;
			}
			DrawText_constchar("Score:", 6, 75, posy - 180, colorFont);
			DrawText_int(checkscore, scorelen, 130, posy - 180, colorFont, 10);
			if (checkgamemode == 'l') DrawText_constchar("level", 5, 0, posy - 200, colorFont);
			else if (checkgamemode == 'f') DrawText_constchar("freeplay", 8, 0, posy - 200, colorFont);

			DrawText_int(newDate.year, 4, 78, posy - 200, colorFont, 10);
			DrawText_constchar(".", 2, 115, posy - 200, colorFont);
			DrawText_int(newDate.mon, 2, 123, posy - 200, colorFont, 10);
			DrawText_constchar(".", 2, 140, posy - 200, colorFont);
			DrawText_int(newDate.day, 2, 148, posy - 200, colorFont, 10);

			DrawText_int(newDate.hour, 2, 174, posy - 200, colorFont, 10);
			DrawText_constchar(":", 1, 191, posy - 200, colorFont);
			DrawText_int(newDate.min, 2, 199, posy - 200, colorFont, 10);
			DrawText_constchar(":", 1, 216, posy - 200, colorFont);
			DrawText_int(newDate.sec, 2, 224, posy - 200, colorFont, 10);
		}

		selectDraw_saveframe(&colorFont, svmcmd, 3, 1, posy - 270, normalW * normalscale - 1, posy - 220);

		checkscore = 0;
		checkgamemode = 0x00;
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		DrawText_constchar("SAVE 4:", 7, 0, posy - 240, colorFont);
		colorFont.font = GLUT_BITMAP_9_BY_15;
		if (checksave_TAKESAVE(4, &newDate, &checkgamemode, &checkscore, &blockminiature, &colorminiature) == 0)
			DrawText_constchar("EMPTY", 5, 80, posy - 240, colorFont);
		else {
			DrawMiniature(blockminiature, colorminiature, 250, posy - 260);
			scorecpy = checkscore, scorelen = 0;
			while (scorecpy > 0) {
				scorelen++;
				scorecpy /= 10;
			}
			DrawText_constchar("Score:", 6, 75, posy - 240, colorFont);
			DrawText_int(checkscore, scorelen, 130, posy - 240, colorFont, 10);
			if (checkgamemode == 'l') DrawText_constchar("level", 5, 0, posy - 260, colorFont);
			else if (checkgamemode == 'f') DrawText_constchar("freeplay", 8, 0, posy - 260, colorFont);

			DrawText_int(newDate.year, 4, 78, posy - 260, colorFont, 10);
			DrawText_constchar(".", 2, 115, posy - 260, colorFont);
			DrawText_int(newDate.mon, 2, 123, posy - 260, colorFont, 10);
			DrawText_constchar(".", 2, 140, posy - 260, colorFont);
			DrawText_int(newDate.day, 2, 148, posy - 260, colorFont, 10);

			DrawText_int(newDate.hour, 2, 174, posy - 260, colorFont, 10);
			DrawText_constchar(":", 1, 191, posy - 260, colorFont);
			DrawText_int(newDate.min, 2, 199, posy - 260, colorFont, 10);
			DrawText_constchar(":", 1, 216, posy - 260, colorFont);
			DrawText_int(newDate.sec, 2, 224, posy - 260, colorFont, 10);
		}

		selectDraw_saveframe(&colorFont, svmcmd, 4, 1, posy - 330, normalW * normalscale - 1, posy - 280);

		checkscore = 0;
		checkgamemode = 0x00;
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		DrawText_constchar("SAVE 5:", 7, 0, posy - 300, colorFont);
		colorFont.font = GLUT_BITMAP_9_BY_15;
		if (checksave_TAKESAVE(5, &newDate, &checkgamemode, &checkscore, &blockminiature, &colorminiature) == 0)
			DrawText_constchar("EMPTY", 5, 80, posy - 300, colorFont);
		else {
			DrawMiniature(blockminiature, colorminiature, 250, posy - 320);
			scorecpy = checkscore, scorelen = 0;
			while (scorecpy > 0) {
				scorelen++;
				scorecpy /= 10;
			}
			DrawText_constchar("Score:", 6, 75, posy - 300, colorFont);
			DrawText_int(checkscore, scorelen, 130, posy - 300, colorFont, 10);
			if (checkgamemode == 'l') DrawText_constchar("level", 5, 0, posy - 320, colorFont);
			else if (checkgamemode == 'f') DrawText_constchar("freeplay", 8, 0, posy - 320, colorFont);

			DrawText_int(newDate.year, 4, 78, posy - 320, colorFont, 10);
			DrawText_constchar(".", 2, 115, posy - 320, colorFont);
			DrawText_int(newDate.mon, 2, 123, posy - 320, colorFont, 10);
			DrawText_constchar(".", 2, 140, posy - 320, colorFont);
			DrawText_int(newDate.day, 2, 148, posy - 320, colorFont, 10);

			DrawText_int(newDate.hour, 2, 174, posy - 320, colorFont, 10);
			DrawText_constchar(":", 1, 191, posy - 320, colorFont);
			DrawText_int(newDate.min, 2, 199, posy - 320, colorFont, 10);
			DrawText_constchar(":", 1, 216, posy - 320, colorFont);
			DrawText_int(newDate.sec, 2, 224, posy - 320, colorFont, 10);
		}
	}
	else if (command == leadermenu) {
		Color colorFont;
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 200;
		DrawText_constchar("Name", 4, 20, posy - normalscale * 1.3, colorFont);
		DrawText_constchar("Score", 5, 220, posy - normalscale * 1.3, colorFont);
		DrawText_constchar("1", 1, 1, posy - normalscale * 2, colorFont);
		DrawText_constchar("2", 1, 1, posy - normalscale * 3, colorFont);
		DrawText_constchar("3", 1, 1, posy - normalscale * 4, colorFont);
		DrawText_constchar("4", 1, 1, posy - normalscale * 5, colorFont);
		DrawText_constchar("5", 1, 1, posy - normalscale * 6, colorFont);
		DrawText_constchar("6", 1, 1, posy - normalscale * 7, colorFont);
		DrawText_constchar("7", 1, 1, posy - normalscale * 8, colorFont);
		DrawText_constchar("8", 1, 1, posy - normalscale * 9, colorFont);
		DrawTable(1, 20, posy - normalscale * 2, normalscale);
	}
	else if (command == themesmenu) {
		Color colorFont;
		colorFont.font = GLUT_BITMAP_9_BY_15;

		selectDraw_GR(&colorFont, selectTheme, 0);
		DrawText_constchar("STANDART BLACK", 14, posx - 55, posy - 120, colorFont);
		colorFont.R = 200; colorFont.G = 200; colorFont.B = 200;
		if (activeTheme == 0) DrawActiveElement(posx + 100, posy - 120, colorFont, 1);
		else DrawActiveElement(posx + 100, posy - 120, colorFont, 0);

		selectDraw_GR(&colorFont, selectTheme, 1);
		DrawText_constchar("STANDART BLUE", 13, posx - 55, posy - 180, colorFont);
		colorFont.R = 200; colorFont.G = 200; colorFont.B = 200;
		if (activeTheme == 1) DrawActiveElement(posx + 100, posy - 180, colorFont, 1);
		else DrawActiveElement(posx + 100, posy - 180, colorFont, 0);

		selectDraw_GR(&colorFont, selectTheme, 2);
		DrawText_constchar("CONTRAST", 8, posx - 55, posy - 240, colorFont);
		colorFont.R = 200; colorFont.G = 200; colorFont.B = 200;
		if (activeTheme == 2) DrawActiveElement(posx + 100, posy - 240, colorFont, 1);
		else DrawActiveElement(posx + 100, posy - 240, colorFont, 0);

		selectDraw_GR(&colorFont, selectTheme, 3);
		DrawText_constchar("CLASSIC", 7, posx - 55, posy - 300, colorFont);
		colorFont.R = 200; colorFont.G = 200; colorFont.B = 200;
		if (activeTheme == 3) DrawActiveElement(posx + 100, posy - 300, colorFont, 1);
		else if (!unlockTheme())DrawActiveElement(posx + 100, posy - 300, colorFont, 2);
		else DrawActiveElement(posx + 100, posy - 300, colorFont, 0);
	}
	else if (command == botlose) {
		Color colorFont;

		colorFont.font = GLUT_BITMAP_TIMES_ROMAN_24;
		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 0;

		DrawText_constchar("GAME OVER!", 10, posx - 30, posy - normalscale * 2, colorFont);

		colorFont.font = GLUT_BITMAP_HELVETICA_18;

		DrawText_constchar("BOTS SCORE:", 11, posx - 30, posy - normalscale * 4, colorFont);

		///////////////dan
		DrawText_constchar("bot's time:", 11, posx - 30, posy - normalscale * 8, colorFont);
		///////////////

		int scorecpy = score, scorelen = 0;
		while (scorecpy > 0) {
			scorecpy /= 10;
			scorelen++;
		}
		colorFont.font = GLUT_BITMAP_HELVETICA_18;
		colorFont.R = 200;
		colorFont.G = 200;
		colorFont.B = 0;

		DrawText_int(score, scorelen, posx + 100, posy - normalscale * 4, colorFont, 10);

		///////////////dan
		if (!schet) { bottime = (clock() - bottime); schet = 1; } //dan
		DrawText_int(bottime, scorelen, posx + 100, posy - normalscale * 8, colorFont, 10);
		///////////////
	}
	if (command == pause || command == level || command == freeplay || command == botplay) {    //   РИСОВАНИЕ ГРАНИЦ
		glColor3ub(200, 200, 200);
		glBegin(GL_LINES);
		glVertex2f(0, 0); glVertex2f(W * scale, 0);
		glVertex2f(W * scale, 0); glVertex2f(W * scale, H * scale);
		glVertex2f(W * scale, H * scale); glVertex2f(0, H * scale);
		glVertex2f(0, H * scale); glVertex2f(0, 0);
		glEnd();
	}
}

void LineDelete(int linenum) {       // удаление линии
	for (int j = linenum; j < H - 1; j++) {
		for (int i = 0; i < W; i++) {
			*(map + i * H + j) = *(map + i * H + j + 1);
		}
	}
	for (int i = 0; i < W; i++)
		*(map + i * H + H - 1) = 0;
}

void FullLine() {                   // проверка на заполненную линию
	int f = 0;
	for (int j = 0; j < H; j++) {
		f = 0;
		for (int i = 0; i < W; i++) {
			if (*(map + i * H + j) == 1) f++;
		}
		if (f == W) {
			LineDelete(j); // удаление линии и соответствующая анимация
			score = score + 100 * levelfactor;
		}
	}
}

void tick()
{
	levelfactor = levelnumber * 1.37 * difficult;  // 1.37  просто коэффициент, почему бы и да

	if (command == level) {
		if (score > CalcScore(levelnumber)) {
			levelnumber++;
			speed += 10;
		}
		if (levelnumber == 8) {
			command = win;
			WinAnimation(0);
			setunlock();
		}
	}
	if (edges(0) == 0) {
		if (command == level || command == freeplay) {
			command = lose; name_i = 0;
		}
		else if (command == botplay) {
			botplaing = 0; //dan
			command = botlose;
		}
	}
	if (edges(-1) == 0) {
		for (int i = 0; i < 4; i++)
			*(map + s[i].x * H + s[i].y) = 1;
		newfigure = 1;
	}
	FullLine();
	if (newfigure == 1) {
		randomblock();
		newfigure = 0;
	}
	if (edges(-1) != 0) {
		tic++;
		if (command == level) {
			if (tic >= 80 - speed) {
				move(-1);
				tic = 0;
			}
		}
		if (command == freeplay) {
			if (tic >= 50 - speed) {
				move(-1);
				tic = 0;
			}
		}
	}
	if (command == botplay) {
		alloc(map, W, H, block, 0);
		randomblock();
	}
}

void Space(unsigned char key, int a, int b)
{
	switch (command) {
	case menu: CheckSpace_record(key, &command, menuintcmd, &newrecord); break;
	case creditspage: command = menu; break;
	case settingsmenu: CheckSpace_settings(key, &command, settingscmd); break;
	case pause: CheckSpace_pause(key, &command, pauseintcmd, savemode); break;
	case leadermenu: if (key == 27) { command = menu; } break;
	case savemenu:
		if (key == 27) { svmcmd = 0; command = pause; }
		else if (key == ' ' && svmcmd >= 0 && svmcmd < 5) {    // здесь последние две проверки по-факту не нужны из-за структуры проверки активной вкладки, но во избежание непредвиденных ошибок и чтобы не нарушать общую структуру checkspace оставим это
			Save2File(svmcmd, score, savemode, newcolor.colorBlock, s, block, map, W, H);
			command = pause;
		}
		break;
	case loadmenu:
		if (key == 27) { svmcmd = 0; command = menu; }
		else if (key == ' ' && svmcmd >= 0 && svmcmd < 5) {
			if (checksave_exist(svmcmd + 1) == 1) {
				LoadFromFile(svmcmd, &score, &savemode, &newcolor.colorBlock, s, &block, map, &W, &H);
				SaveColor();
				if (savemode == 'l')command = level;
				else if (savemode == 'f') command = freeplay;
			}
		}
		break;
	case gamemodemenu:
		if (key == ' ' && gmcmd == 4)
			command = settingsmenu;
		break;
	case level: if (key == 27) {
		if (command == level) savemode = 'l';
		else if (command == freeplay) savemode = 'f';
		command = pause;
	}
			  else if (key == ' ' && edges(-1) != 0) move(-1);
		break;
	case freeplay: if (key == 27) command = pause;
				 else if (key == ' ' && edges(-1) != 0) move(-1);
		break;
	case minimenu:
		if (key == ' ' && mini == 0) {
			score = 0;
			levelnumber = 1;
			savemode = 'l';
			randomblock();
			refreshmap(map, W, H);
			command = level;
		}
		if (key == ' ' && mini == 1) {
			score = 0;
			savemode = 'f';
			randomblock();
			refreshmap(map, W, H);
			command = freeplay;
		}
		if (key == ' ' && mini == 2) {
			///////////////////dan
			botplaing = 1;
			bottime = clock();
			///////////////////
			score = 0;
			randomblock();
			refreshmap(map, W, H);
			command = botplay;
		}
		break;

	case lose:
		if (key != 9 && ((key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123) || key == 95)) {
			getkey = key;
			change++;
		}
		else if (key == 9 && name[0] != 0x00) {
			if (savetable(name, score)) newrecord++;
			command = menu;
			name[0] = 0x00;
		}
		break;
	case win:
		if (key != 9 && ((key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123) || key == 95)) {
			getkey = key;
			change++;
		}
		else if (key == 9 && name[0] != 0x00) {
			if (savetable(name, score)) newrecord++;
			command = menu;
			name[0] = 0x00;
		}
		break;
	case themesmenu:
		if (key == 27)
			command = settingsmenu;
		if (key == ' ' && selectTheme >= 0 && selectTheme <= 2) activeTheme = selectTheme;
		else if (key == ' ' && selectTheme == 3 && unlockTheme()) activeTheme = 3;
		break;
	case botlose:
		if (key == 27) command = menu;
		break;
	}
}

int canturn() {
	int k = 4 / 2;
	int x1, y1;
	for (int i = 0; i < 4; i++) {
		x1 = s[k].x - s[i].x;
		y1 = s[k].y - s[i].y;

		if ((s[k].y + x1) > H - 1 || (s[k].y + x1) < 0 || (s[k].x - y1) > W - 1 || (s[k].x - y1) < 0 || *(map + (s[k].x - y1) * H + (s[k].y + x1)) == 1)
			return 0;
	}
	return 1;
}

void turn() {
	int x1, y1;
	for (int i = 0; i < 4; i++) {
		x1 = s[2].x - s[i].x;
		y1 = s[2].y - s[i].y;

		s[i].x = s[2].x - y1;
		s[i].y = s[2].y + x1;
	}
}

void Key(int key, int a, int b)
{
	switch (command) {
	case menu: CheckKey(key, &menuintcmd, 5); break;
	case minimenu: CheckKey(key, &mini, 2); break;
	case pause:  CheckKey(key, &pauseintcmd, 2); break;
	case savemenu:  CheckKey(key, &svmcmd, 4); break;
	case loadmenu:  CheckKey(key, &svmcmd, 4); break;
	case themesmenu:  CheckKey(key, &selectTheme, 3); break;
	case gamemodemenu: CheckKey(key, &gmcmd, 4);
		if (key == GLUT_KEY_RIGHT && difficult != 4 && gmcmd == 0) { difficult++; speed += 10; }
		if (key == GLUT_KEY_LEFT && difficult != 1 && gmcmd == 0) { difficult--; speed -= 10; }
		if (key == GLUT_KEY_RIGHT && randomizer_type != 1 && gmcmd == 3) { randomizer_type = 1; }
		if (key == GLUT_KEY_LEFT && randomizer_type != 0 && gmcmd == 3) { randomizer_type = 0; }
		if (key == GLUT_KEY_RIGHT && W != 99 && gmcmd == 1) W++;
		if (key == GLUT_KEY_LEFT && W != 10 && gmcmd == 1) W--;
		if (key == GLUT_KEY_RIGHT && H != 99 && gmcmd == 2) H++;
		if (key == GLUT_KEY_LEFT && H != 20 && gmcmd == 2) H--;
		break;
	case settingsmenu: CheckKey(key, &settingscmd, 2); break;
	case level:
		if (key == GLUT_KEY_LEFT && edges(-2) != 0)
			move(-2);
		else if (key == GLUT_KEY_RIGHT && edges(2) != 0)
			move(2);
		else if (key == GLUT_KEY_UP && block != 3 && canturn() != 0)  // если квадрат, то не изменяет позицию
			turn();
		break;
	case freeplay:
		if (key == GLUT_KEY_LEFT && edges(-2) != 0)
			move(-2);
		else if (key == GLUT_KEY_RIGHT && edges(2) != 0)
			move(2);
		else if (key == GLUT_KEY_UP && canturn() != 0)
			turn();
		break;
	}
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (command == level || command == freeplay) {
		if (activeTheme == 1)
			glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		else if (activeTheme == 3)
			glClearColor(0.647f, 0.714f, 0.682f, 1.0f);
	}

	int scaleH = normalH * normalscale / H;			//при изменении W или H
	int scaleW = normalW * normalscale / W;
	if (scaleH > scaleW) scale = scaleW;
	else scale = scaleH;

	Draw();

	if (command == level || command == freeplay)
		DrawDynamicObj(s, scale, newcolor, activeTheme);

	glFlush();
	glutSwapBuffers();
}

void Animator() {
	if (animationfac > 0) {
		animationfac--;
		if (animationfac == 0) animationfac = -30;
	}
	if (animationfac < 0) {
		animationfac++;
		if (animationfac == 0) animationfac = 30;
	}
}

void timer(int value)
{
	Animator();
	Display();
	if (command == level || command == freeplay || command == botplay) tick();
	glutTimerFunc(1, timer, 0);
}

void Reshape(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glutReshapeWindow(normalW * normalscale, normalH * normalscale);
}

int main(int argc, char** argv)
{
	//////////////dan
	arch = fopen(filename, "r");
	//////////////
	HWND hWND = GetConsoleWindow();
	ShowWindow(hWND, SW_HIDE);

	srand(time(0x00));
	fillPool();
	for (int i = 0; i < 10; i++)
		random_block_TGM3();
	fillBag();

	map = (int*)malloc(100 * 100 * sizeof(int));
	refreshmap(map, W, H);
	INI();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(normalW * normalscale, normalH * normalscale);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TETRIS");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, scale * W, 0.0, scale * H, -1.0, 1.0);

	glutDisplayFunc(Display);
	glutSpecialFunc(Key);
	glutKeyboardFunc(Space);
	gluPerspective(45, (float)(W * scale) / (float)(H * scale), 0, 0);
	glutReshapeFunc(Reshape);
	glutTimerFunc(50, timer, 0);
	glutMainLoop();

	//////////////dan
	fclose(arch);
	//////////////

	return 0;
}