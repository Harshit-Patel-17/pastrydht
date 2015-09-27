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

/*! \brief Terminal like User Interface.
 */
class Ui {
	string appName; /*!< Name of application in prompt */
	string delim; /*!< Delimiter in prompt */
	string *history; /*!< History of commands. */
	string buffer; /*!< Command buffer. */
	string promptColor; /*!< Color of prompt. */
	int historyStart; /*!< Start index of history. */
	int historyEnd; /*!< End index of history. */
	int maxHistoryCount; /*!< Maximum commands that can be stored in history. */
	int currentHistory; /*!< Current history index. */
	struct termios oldTerminalSettings; /*!< Default terminal settings. */
	struct termios newTerminalSettings; /*!< New terminal settings to capture key presses. */
	int incr(int x); /*!< Increment in modulo maxHistoryCount system. */
	int decr(int x); /*!< Decrement in modulo maxHistoryCount system. */
public:
	Ui();
	virtual ~Ui();

	void init(string appName, string delim, string promptColor); /*!< Start User Interface. */
	int getInputLine(string **input); /*!< Wait for user input. */
	void displayPrompt(); /*!< Display prompt. */
};

extern Ui ui;

#endif /* HEADER_UI_H_ */
