#ifndef HEAD_H
#define HEAD_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#if defined (_WIN32) || defined (_WIN64)
#include <windows.h>
#endif

////////////////////////////////////INITIALIZATION//////////////////////////////////
typedef struct Color {
	int R;
	int G;
	int B;
	int colorBlock;
	void* font;
} Color;
typedef struct Position
{
	int x;
	int y;
} Position;
typedef enum CMD { level, pause, menu, minimenu, settingsmenu, lose, freeplay, gamemodemenu, themesmenu, creditspage, win, savemenu, loadmenu, leadermenu, botplay, botlose } CMD;   // общие разделы
/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////DRAWOBJECTS///////////////////////////////////////////
void DrawTetris(int posx, int posy);
void DrawStaticObj(int W, int H, int scale, int* map, int activetTheme);
void DrawText_constchar(const char* text, int lengh, int posx, int posy, Color newC);
void DrawText_char(char* text, int lengh, int posx, int posy, Color newC);
void DrawText_int(int text, int lengh, int posx, int posy, Color newC, int d1);
void DrawDynamicObj(Position* s, int scale, Color newC, int activeTheme);
void DrawSaveFrame(int x0, int y0, int x, int y, int isActive);
void DrawMiniature(int Blocktype, int color, int x0, int y0);
void DrawTable(int number, int posx, int posy, int normalscale);
void DrawActiveElement(int posx, int posy, Color newC, int isActive);
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////KEYBOARD////////////////////////////////////////////
void CheckKey(int, int*, int);
void CheckSpace_record(unsigned char key, CMD* cmd, int checkcmd, int* newrecord); // for main menu
void CheckSpace_settings(unsigned char key, CMD* cmd, int checkcmd);   // for settingsmenu
void CheckSpace_pause(unsigned char key, CMD* cmd, int checkcmd, char savemode);  // For pause
/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////SAVE//////////////////////////////////////////////
typedef struct Date {
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
} Date;
int checksave_TAKESAVE0x00(int checkcmd, Date* savedate, char* level, int* score, int* Blocktype, int* color);
int checksave_exist(int);
void Save2File(int, int, char, int, Position*, int, int*, int, int);
void LoadFromFile(int, int*, char*, int*, Position*, int*, int*, int*, int*);
bool savetable(char* savename, int savescore);
bool unlockTheme();
void setunlock();
/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////RANDOM////////////////////////////////////////////
void fillPool();
int random_block_TGM3();
void fillBag();
int random_block_TW();
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////AI//////////////////////////////////////////////
void alloc(int* map, int W, int H, int block, int strategy);
void INI();
/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////MAIN//////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////

#endif
