#include "Header.h"

void DrawTetris(int posx, int posy) {
	glColor3ub(255, 0, 0);
	glRasterPos2i(posx, posy);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'T');
	glColor3ub(0, 255, 0);
	glRasterPos2i(posx + 13, posy);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
	glColor3ub(255, 165, 0);
	glRasterPos2i(posx + 13 * 2, posy);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'T');
	glColor3ub(128, 0, 128);
	glRasterPos2i(posx + 13 * 3, posy);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
	glColor3ub(0, 191, 255);
	glRasterPos2i(posx + 13 * 4, posy);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'I');
	glColor3ub(255, 215, 0);
	glRasterPos2i(posx + 13 * 4 + 8, posy);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'S');
}

void DrawStaticObj(int W, int H, int scale, int* map, int activeTheme) {
	if (activeTheme == 0 || activeTheme == 1)
		glColor3f(0.1, 0.1, 0.1);
	else if (activeTheme == 2)
		glColor3f(0.5, 0.5, 0.5);
	else if (activeTheme == 3)
		glColor3f(0.647, 0.714, 0.682);
	glBegin(GL_LINES);
	for (int i = 0; i < W * scale; i += scale)
	{
		glVertex2f(i, 0); glVertex2f(i, H * scale);
	}
	for (int j = 0; j < H * scale; j += scale)
	{
		glVertex2f(0, j); glVertex2f(W * scale, j);
	}
	glEnd();

	if (activeTheme == 3) {
		glColor3ub(144, 156, 152);
		for (int i = 0; i < W * scale; i += scale)
			for (int j = 0; j < H * scale; j += scale) {
				glRectf(i + 1, j + 1, i + scale - 1, j + scale - 1);
			}
		glColor3f(0.647, 0.714, 0.682);
		for (int i = 0; i < W * scale; i += scale)
			for (int j = 0; j < H * scale; j += scale) {
				glRectf(i + 5, j + 5, i + scale - 5, j + scale - 5);
			}
		glColor3ub(144, 156, 152);
		for (int i = 0; i < W * scale; i += scale)
			for (int j = 0; j < H * scale; j += scale) {
				glRectf(i + 9, j + 9, i + scale - 9, j + scale - 9);
			}
	}

	int r = 150, g = 150, b = 150;
	for (int i = 0; i < W; i++)
		for (int j = 0; j < H; j++) {
			if (*(map + i * H + j) == 1) {
				if (activeTheme == 3) {
					glColor3ub(0, 0, 0);
					glRectf(i * scale + 1, j * scale + 1, (i + 1) * scale - 1, (j + 1) * scale - 1);
					glColor3f(0.647, 0.714, 0.682);
					glRectf(i * scale + 5, j * scale + 5, (i + 1) * scale - 5, (j + 1) * scale - 5);
					glColor3ub(0, 0, 0);
					glRectf(i * scale + 9, j * scale + 9, (i + 1) * scale - 9, (j + 1) * scale - 9);
				}
				else {
					glBegin(GL_TRIANGLES);
					glColor3ub(r * 0.6, g * 0.6, b * 0.6);
					glVertex2f(i * scale, j * scale);
					glVertex2f(i * scale + scale / 2, j * scale + scale / 2);
					glVertex2f(i * scale, (j + 1) * scale);
					glEnd();

					glBegin(GL_TRIANGLES);
					glColor3ub(r * 0.77, g * 0.77, b * 0.77);
					glVertex2f(i * scale, (j + 1) * scale);
					glVertex2f(i * scale + scale / 2, j * scale + scale / 2);
					glVertex2f((i + 1) * scale, (j + 1) * scale);
					glEnd();

					glBegin(GL_TRIANGLES);
					glColor3ub(r * 0.82, g * 0.82, b * 0.82);
					glVertex2f(i * scale, j * scale);
					glVertex2f(i * scale + scale / 2, j * scale + scale / 2);
					glVertex2f((i + 1) * scale, j * scale);
					glEnd();

					glBegin(GL_TRIANGLES);
					glColor3ub(r + 60, b + 60, b + 60);
					glVertex2f((i + 1) * scale, j * scale);
					glVertex2f(i * scale + scale / 2, j * scale + scale / 2);
					glVertex2f((i + 1) * scale, (j + 1) * scale);
					glEnd();

					glColor3ub(r, g, b);
					glRectf(i * scale + 4, j * scale + 4, (i + 1) * scale - 4, (j + 1) * scale - 4);			//квадрат
				}
			}
		}
}

void DrawText_constchar(const char* text, int lengh, int posx, int posy, Color newC) {
	glColor3ub(newC.R, newC.G, newC.B);
	glRasterPos2i(posx, posy);
	for (int i = 0; i < lengh; i++)
		glutBitmapCharacter(newC.font, (int)text[i]);
}

void DrawText_char(char* text, int lengh, int posx, int posy, Color newC) {
	glColor3ub(newC.R, newC.G, newC.B);
	glRasterPos2i(posx, posy);
	for (int i = 0; i < lengh; i++)
		glutBitmapCharacter(newC.font, (int)text[i]);
}

void DrawText_int(int text, int lengh, int posx, int posy, Color newC, int d1) {
	glColor3ub(newC.R, newC.G, newC.B);
	if (lengh == 2 && text / 10 == 0) { lengh--; glRasterPos2i(posx, posy); glutBitmapCharacter(newC.font, '0'); posx += 10; }
	if (text == 0) { glRasterPos2i(posx, posy); glutBitmapCharacter(newC.font, '0'); }
	else {
		int* mas = (int*)malloc(lengh * sizeof(int));
		for (int i = 0; i < lengh; i++) {
			mas[i] = text % 10;
			text /= 10;
		}
		int d = 0;
		for (int i = lengh - 1; i >= 0; i--) {
			glRasterPos2i(posx + d, posy);
			glutBitmapCharacter(newC.font, mas[i] + '0');
			d += d1;
		}
	}
}

void DrawDynamicObj(Position* s, int scale, Color newC, int activeTheme) {
	for (int i = 0; i < 4; i++) {
		if (activeTheme == 3) {
			glColor3ub(0, 0, 0);
			glRectf(s[i].x * scale + 1, s[i].y * scale + 1, (s[i].x + 1) * scale - 1, (s[i].y + 1) * scale - 1);
			glColor3f(0.647, 0.714, 0.682);
			glRectf(s[i].x * scale + 5, s[i].y * scale + 5, (s[i].x + 1) * scale - 5, (s[i].y + 1) * scale - 5);
			glColor3ub(0, 0, 0);
			glRectf(s[i].x * scale + 9, s[i].y * scale + 9, (s[i].x + 1) * scale - 9, (s[i].y + 1) * scale - 9);
		}
		else {
			glBegin(GL_TRIANGLES);
			glColor3ub(newC.R * 0.6, newC.G * 0.6, newC.B * 0.6);
			glVertex2f(s[i].x * scale, s[i].y * scale);
			glVertex2f(s[i].x * scale + scale / 2, s[i].y * scale + scale / 2);
			glVertex2f(s[i].x * scale, (s[i].y + 1) * scale);
			glEnd();

			glBegin(GL_TRIANGLES);
			glColor3ub(newC.R * 0.77, newC.G * 0.77, newC.B * 0.77);
			glVertex2f(s[i].x * scale, (s[i].y + 1) * scale);
			glVertex2f(s[i].x * scale + scale / 2, s[i].y * scale + scale / 2);
			glVertex2f((s[i].x + 1) * scale, (s[i].y + 1) * scale);
			glEnd();

			glBegin(GL_TRIANGLES);
			glColor3ub(newC.R * 0.82, newC.G * 0.82, newC.B * 0.82);
			glVertex2f(s[i].x * scale, s[i].y * scale);
			glVertex2f(s[i].x * scale + scale / 2, s[i].y * scale + scale / 2);
			glVertex2f((s[i].x + 1) * scale, s[i].y * scale);
			glEnd();

			glBegin(GL_TRIANGLES);
			glColor3ub(newC.R + 60, newC.G + 60, newC.B + 60);
			glVertex2f((s[i].x + 1) * scale, s[i].y * scale);
			glVertex2f(s[i].x * scale + scale / 2, s[i].y * scale + scale / 2);
			glVertex2f((s[i].x + 1) * scale, (s[i].y + 1) * scale);
			glEnd();

			glColor3ub(newC.R, newC.G, newC.B);
			glRectf(s[i].x * scale + 4, s[i].y * scale + 4, (s[i].x + 1) * scale - 4, (s[i].y + 1) * scale - 4);
		}
	}
}

void DrawSaveFrame(int x0, int y0, int x, int y, int isActive) {		//€чейка загрузки
	if (isActive == 1)glColor3ub(200, 200, 200);
	else glColor3ub(100, 100, 100);
	glBegin(GL_LINES);
	glVertex2f(x0, y0); glVertex2f(x, y0);
	glVertex2f(x, y0); glVertex2f(x, y);
	glVertex2f(x, y); glVertex2f(x0, y);
	glVertex2f(x0, y); glVertex2f(x0, y0);
	glEnd();
}

void DrawMiniature(int Blocktype, int color, int x0, int y0) {
	int R = 0, G = 0, B = 0;
	switch (color) {
	case 0:
		R = 150;
		G = 0;
		B = 0;
		break;
	case 1:
		R = 0;
		G = 150;
		B = 0;
		break;
	case 2:
		R = 0;
		G = 0;
		B = 150;
		break;
	case 3:
		R = 150;
		G = 150;
		B = 0;
		break;
	case 4:
		R = 0;
		G = 150;
		B = 150;
		break;
	case 5:
		R = 150;
		G = 0;
		B = 150;
		break;
	}
	glColor3ub(R, G, B);
	switch (Blocktype) {
	case 0:                                     // I
		glRectf(x0, y0 + 16, x0 + 32, y0 + 24);
		break;
	case 1:                                     // J
		glRectf(x0, y0 + 16, x0 + 24, y0 + 24);
		glRectf(x0, y0 + 24, x0 + 8, y0 + 32);
		break;
	case 2:                                     // L
		glRectf(x0, y0 + 16, x0 + 24, y0 + 24);
		glRectf(x0 + 16, y0 + 24, x0 + 24, y0 + 32);
		break;
	case 3:                                     // O
		glRectf(x0 + 8, y0 + 16, x0 + 24, y0 + 32);
		break;
	case 4:                                     // S
		glRectf(x0, y0 + 16, x0 + 16, y0 + 24);
		glRectf(x0 + 8, y0 + 24, x0 + 24, y0 + 32);
		break;
	case 5:                                     // T
		glRectf(x0, y0 + 16, x0 + 24, y0 + 24);
		glRectf(x0 + 8, y0 + 24, x0 + 16, y0 + 32);
		break;
	case 6:                                     // Z
		glRectf(x0, y0 + 24, x0 + 16, y0 + 32);
		glRectf(x0 + 8, y0 + 16, x0 + 24, y0 + 24);
		break;
	}
}

void DrawTable(int number, int posx, int posy, int normalscale) {
	if (number == 9) return;
	char SAVE[34] = "saves/tablesaves/savetable";
	char num[6] = { 0x00, '.','t', 'x', 't', '\0' };
	num[0] = number + '0';
	FILE* f;

	if ((f = fopen(strcat(SAVE, num), "r")) == 0x00) {
		return;
	}
	else {
		Color newC;
		newC.font = GLUT_BITMAP_9_BY_15;
		newC.R = 200;
		newC.G = 200;
		newC.B = 100;
		char tmp[50];
		int i = 0;
		int printscore = 0;
		char printname[21];
		int d = 1;
		while (!feof(f)) {
			tmp[i] = getc(f);
			i++;
		}
		i -= 2;
		int scorelen = 0;
		while (tmp[i] != ';') {
			printscore += (tmp[i] - '0') * d;
			i--;
			d *= 10;
			scorelen++;
		}
		int namelen = i;
		i = 0;
		while (tmp[i] != ';') {
			printname[i] = tmp[i];
			i++;
		}
		printname[i] = 0x00;
		DrawText_char(printname, namelen, posx, posy, newC);
		DrawText_int(printscore, scorelen, posx + 200, posy, newC, 10);
		posy -= normalscale;
		DrawTable(number + 1, posx, posy, normalscale);
	}
	fclose(f);
}

void DrawActiveElement(int posx, int posy, Color newC, int isActive) {
	glColor3ub(newC.R, newC.G, newC.B);
	if (isActive == 1)
		glRectf(posx, posy, posx + 10, posy + 10);
	else if (isActive == 0) {
		glBegin(GL_LINES);
		glVertex2f(posx, posy); glVertex2f(posx + 10, posy);
		glVertex2f(posx + 10, posy); glVertex2f(posx + 10, posy + 10);
		glVertex2f(posx + 10, posy + 10);  glVertex2f(posx, posy + 10);
		glVertex2f(posx, posy + 10); glVertex2f(posx, posy);
		glEnd();
	}
	else if (isActive == 2) {
		glColor3ub(200, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(posx, posy); glVertex2f(posx + 10, posy + 10);
		glVertex2f(posx + 10, posy); glVertex2f(posx, posy + 10);
		glEnd();

		glColor3ub(newC.R, newC.G, newC.B);
		glBegin(GL_LINES);
		glVertex2f(posx, posy); glVertex2f(posx + 10, posy);
		glVertex2f(posx + 10, posy); glVertex2f(posx + 10, posy + 10);
		glVertex2f(posx + 10, posy + 10);  glVertex2f(posx, posy + 10);
		glVertex2f(posx, posy + 10); glVertex2f(posx, posy);
		glEnd();
	}
}