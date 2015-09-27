/*
 * main.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/Commands.h"
#include <algorithm>

void Node::application(string s) {

	cout << s << endl;

}

int main(void) {

	//Set up app and Ui
	string appName = "pastry";
	string delim = "$";
	ui.init(appName, delim, GOLD);

	int totalArguments;
	string *arguments;

	pthread_t serverId;
	while(1) {
		totalArguments = ui.getInputLine(&arguments);
		if(totalArguments > 0)
			transform(arguments[0].begin(), arguments[0].end(), arguments[0].begin(), ::tolower);
		if(totalArguments > 0) {

			if(arguments[0] == "help") {
				printHelp();
			}
			else if(arguments[0] == "port") {
				if(totalArguments == 2) {
					port(arguments[1]);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "create") {
				if(totalArguments == 1) {
					serverId = create();
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "join") {
				if(totalArguments == 3) {
					join(arguments[1], arguments[2]);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "put") {
				if(totalArguments == 3) {
					put(arguments[1], arguments[2]);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "get") {
				if(totalArguments == 2) {
					get(arguments[1]);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "lset") {
				if(totalArguments == 1) {
					lset();
				} else if(totalArguments == 3) {
					lset(arguments[1], arguments[2]);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "routetable") {
				if(totalArguments == 1) {
					routetable();
				} else if(totalArguments == 3) {
					routetable(arguments[1], arguments[2]);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "nset") {
				if(totalArguments == 1) {
					nset();
				} else if(totalArguments == 3) {
					nset(arguments[1], arguments[2]);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "dump") {
				if(totalArguments == 1) {
					dump();
				} else if(totalArguments == 3) {
					dump(arguments[1], arguments[2]);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "quit") {
				if(totalArguments == 1) {
					quit();
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "shutdown") {
				if(totalArguments == 1) {
					shutdown();
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else {
				cout << totalArguments << endl;
				if(totalArguments > 0)
					cout << "Command '" << arguments[0] << "' not recognized" << endl;
			}
		}
		//if(port != "3000")
		//stateTableManager.join("127.0.0.1", "3000");
		//string response = client.send(key, message);
		//cout << endl << response << endl;
	}

	pthread_join(serverId, NULL);
	return 0;

}
