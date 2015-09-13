/*
 * Ui.cpp
 *
 *  Created on: 12-Sep-2015
 *      Author: harshit
 */

#include "../header/Ui.h"

Ui::Ui() {

	history = NULL;
	maxHistoryCount = 50;
	historyStart = 0;
	historyEnd = 0;
	currentHistory = 0;
}

Ui::~Ui() {
	delete[] history;
}

int Ui::incr(int x) {

	return (x + 1) % maxHistoryCount;

}
int Ui::decr(int x) {

	return (maxHistoryCount + x - 1) % maxHistoryCount;

}

void Ui::init(string appName, string delim, string promptColor) {

	this->appName = appName;
	this->delim = delim;
	this->promptColor = promptColor;
	this->historyStart = 0;
	this->historyEnd = 0;
	history = new string[maxHistoryCount];
	currentHistory = historyStart;
	history[currentHistory] = "";

}

int Ui::getInputLine(string **input) {

	int totalArguments = 0, oldBufferSize;
	int inputChar;

	*input = NULL;

	cout << endl;
	displayPrompt();

	//Stop input buffering of terminal
	struct termios oldT, newT;
	tcgetattr(0, &oldT);
	newT = oldT;
	newT.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr(0, TCSANOW, &newT);

	//Read input character by character
	while(1) {
		oldBufferSize = buffer.length();
		inputChar = getchar();

		if(inputChar == 10)
			break;

		if(inputChar == '\033') {
			//Detect Arrow Keys
			getchar();
			inputChar = getchar();
			switch(inputChar) {
				case KEY_UP:
					if(currentHistory != historyEnd) {
						currentHistory = incr(currentHistory);
						buffer = history[currentHistory];
					}
					break;

				case KEY_DOWN:
					if(currentHistory != historyStart) {
						currentHistory = decr(currentHistory);
						buffer = history[currentHistory];
					}
					break;

				case KEY_RIGHT:
					break;

				case KEY_LEFT:
					break;
			}
		} else {
			//Detect Relevant Ascii Values
			switch(inputChar) {
				case KEY_BACKSPACE:
					if(buffer.length() != 0)
						buffer.erase(buffer.length() - 1);
					break;

				default:
					buffer = buffer + (char)inputChar;
					break;
			}
		}

		//displayPrompt();
		for(int i = 0; i < oldBufferSize; i++)
			cout << "\b \b";
		cout << buffer;
	}

	//Trim buffer
	int first = buffer.find_first_not_of(' ');
	int last = buffer.find_last_not_of(' ');

	//Split buffer using whitespace as delimiter if it is not empty
	if(first != -1 && last != -1) {
		buffer = buffer.substr(first, last-first+1);
		if(history[incr(historyStart)] != buffer) {
			history[historyStart] = buffer;
			historyStart = decr(historyStart);
			history[historyStart] = "";
			if(historyStart == historyEnd)
						historyEnd = decr(historyEnd);
		}
		currentHistory = historyStart;

		totalArguments = 1;
		for(unsigned int i = 0; i < buffer.length(); i++)
			if(buffer.at(i) == ' ' && buffer.at(i-1) != ' ')
				totalArguments++;
		*input = new string[totalArguments];
		int inputIndex = 0;
		for(unsigned int i = 0; i < buffer.length(); i++) {
			if(buffer.at(i) == ' ') {
				if(buffer.at(i-1) != ' ')
					inputIndex++;
			} else {
				(*input)[inputIndex].push_back(buffer.at(i));
			}
		}
	}

	//Empty buffer for next input
	buffer = history[currentHistory];

	//Restore terminal settings
	tcsetattr(0, TCSANOW, &oldT);
	cout << endl;
	return totalArguments;

}

void Ui::displayPrompt() {

	cout << promptColor << appName << " " << delim << " " << RESET_COLOR;

}

