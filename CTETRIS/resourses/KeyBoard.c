#include "Header.h"

void CheckKey(int Key, int* command, int max) {
	if (Key == GLUT_KEY_UP && *command != 0) *command = *command - 1;
	else if (Key == GLUT_KEY_UP && *command == 0) *command = max;
	else if (Key == GLUT_KEY_DOWN && *command != max) *command = *command + 1;
	else if (Key == GLUT_KEY_DOWN && *command == max) *command = 0;
}

void CheckSpace_record(unsigned char key, CMD* cmd, int checkcmd, int* newrecord) {
	if (key == ' ' && checkcmd == 0)
		*cmd = minimenu;
	else if (key == ' ' && checkcmd == 1)
		*cmd = loadmenu;
	else if (key == ' ' && checkcmd == 2) {
		*newrecord = 0;
		*cmd = leadermenu;
	}
	else if (key == ' ' && checkcmd == 3)
		*cmd = creditspage;
	else if (key == ' ' && checkcmd == 4)
		*cmd = settingsmenu;
	else if (key == ' ' && checkcmd == 5)
		exit(0);
}

void CheckSpace_pause(unsigned char key, CMD* cmd, int checkcmd, char savemode) {
	if (key == 27)              // esc
	{
		if (savemode == 'l')*cmd = level;
		else if (savemode == 'f')*cmd = freeplay;
	}
	else if (key == ' ' && checkcmd == 0)
	{
		if (savemode == 'l')*cmd = level;
		else if (savemode == 'f')*cmd = freeplay;
	}
	else if (key == ' ' && checkcmd == 1) *cmd = savemenu;
	else if (key == ' ' && checkcmd == 2) *cmd = menu;
}

void CheckSpace_settings(unsigned char key, CMD* cmd, int checkcmd) {
	if (key == 27) *cmd = menu;
	if (key == ' ' && checkcmd == 0) *cmd = themesmenu;
	else if (key == ' ' && checkcmd == 1) *cmd = gamemodemenu;
	else if (key == ' ' && checkcmd == 2) *cmd = menu;
}