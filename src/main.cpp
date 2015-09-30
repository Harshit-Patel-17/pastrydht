/*
 * main.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/Commands.h"
#include <algorithm>
#include <map>

#define TOTAL_PARTS 4

void Node::application(string key, string value, bool valueFound) {

	static int partsReceived = 0;
	static int minKey = INT_MAX;
	//static string satCodeParts[PARTS];
	static map<int, string> parts;

	int part = atoi(key.c_str());
	if(minKey > part)
		minKey = part;

	parts[part] = value;
	partsReceived++;
	if(partsReceived == TOTAL_PARTS) {
		if(valueFound == false) {
			partsReceived = 0;
			cout << "Launch code not found for day " << part/TOTAL_PARTS + 1;
			return;
		}

		string receivedMd5SatCodeHex;
		for(int i = 0; i < TOTAL_PARTS; i++)
			receivedMd5SatCodeHex = receivedMd5SatCodeHex + parts[minKey + i];

		if(verifyCode == true) {
			string md5SatCodeHexString = md5Hex(satCode);
			if(md5SatCodeHexString.compare(receivedMd5SatCodeHex) == 0) {
				cout << "Valid launch code." << endl;
			} else {
				cout << "Invalid launch code." << endl;
			}
		} else {
			cout << receivedMd5SatCodeHex << endl;
		}
		partsReceived = 0;
		minKey = INT_MAX;
		parts.clear();
	}

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
			else if(arguments[0] == "store") {
				if(totalArguments == 3) {
					store(arguments[1], arguments[2], TOTAL_PARTS);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "verify") {
				if(totalArguments == 3) {
					verifyCode = true;
					satCode =arguments[2];
					retrieve(arguments[1], TOTAL_PARTS);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "retv") {
				if(totalArguments == 2) {
					verifyCode = false;
					retrieve(arguments[1], TOTAL_PARTS);
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
