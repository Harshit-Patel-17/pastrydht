/*
 * main.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/Ui.h"

int main(void) {

	string appName = "pastry";
	string delim = "$";
	Ui ui;

	ui.init(appName, delim, GOLD);

	int totalArguments;
	string *arguments;
	while(1) {
		totalArguments = ui.getInputLine(&arguments);
		for(int i = 0; i < totalArguments; i++)
			cout << arguments[i] << " ";
		delete[] arguments;
	}

	return 0;
}
