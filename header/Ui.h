/*
 * Ui.h
 *
 *  Created on: 12-Sep-2015
 *      Author: harshit
 */
#ifndef HEADER_UI_H_
#define HEADER_UI_H_

#include <string>
#include <iostream>
#include <termios.h>
#include <stdio.h>

using namespace std;

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_ENTER 10
#define KEY_BACKSPACE 127
#define RESET_COLOR "\033[0m"
#define RED "\033[22;31m"
#define GREEN "\033[22;32m"
#define GOLD "\033[22;33m"
#define BLUE "\033[22;34m"


class Ui {
	string appName;
	string delim;
	string *history;
	string buffer;
	string promptColor;
	int historyStart;
	int historyEnd;
	int maxHistoryCount;
	int currentHistory;
	struct termios oldTerminalSettings;
	struct termios newTerminalSettings;
	int incr(int x);
	int decr(int x);
public:
	Ui();
	virtual ~Ui();

	void init(string appName, string delim, string promptColor);
	int getInputLine(string **input);
	void displayPrompt();
};

extern Ui ui;

#endif /* HEADER_UI_H_ */
