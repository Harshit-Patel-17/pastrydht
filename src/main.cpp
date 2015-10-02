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

enum get_mode {VERIFY, RETV, GETALL};
get_mode mode = RETV;
string satCode = "";
mutex retrieveLock;

void store(string day, string sat_code, int n) {
	int dayInt = atoi(day.c_str());
	if(dayInt < 1 || dayInt > 365) {
		cout << "Invalid day number." << endl;
		return;
	}

	string md5SatCodeHexString;
	md5SatCodeHexString = md5Hex(sat_code);

	int partLength = 32 / n;
	char key[10];
	string value;

	for(int i = 0; i < n; i++) {
		sprintf(key, "%d", (dayInt-1)*n + i);
		value = md5SatCodeHexString.substr(i*partLength, partLength);
		put(key, value);
	}
}

void retrieve(string day, int n) {

	retrieveLock.lock();
	int dayInt = atoi(day.c_str());
	if(dayInt < 1 || dayInt > 365) {
		cout << "Invalid day number." << endl;
		return;
	}

	char key[10];

	for(int i = 0; i < n; i++) {
		sprintf(key, "%d", (dayInt-1)*n + i);
		get(key);
	}

}

void getall(string d1, string d2, int n)
{
	int D1 = atoi(d1.c_str());
	int D2 = atoi(d2.c_str());

	if(D1 > D2)
	{
		int temp = D1;
		D1 = D2;
		D2 = temp;
	}

	for(int i = D1; i <= D2; i++)
	{
		char day[4];
		sprintf(day, "%d", i);
		retrieve(day, n);
	}
}

void Node::application(string key, string value, bool valueFound) {

	static int partsReceived = 0;
	static int minKey = INT_MAX;
	static map<int, string> parts;

	int part = atoi(key.c_str());
	if(minKey > part)
		minKey = part;

	parts[part] = value;
	partsReceived++;
	if(partsReceived == TOTAL_PARTS) {
		string receivedMd5SatCodeHex;
		for(int i = 0; i < TOTAL_PARTS; i++)
			receivedMd5SatCodeHex = receivedMd5SatCodeHex + parts[minKey + i];
		switch(mode)
		{
		case VERIFY:
			if(valueFound == false)
				cout << "Launch code not found for day " << part/TOTAL_PARTS + 1;
			else {
				string md5SatCodeHexString = md5Hex(satCode);
				if(md5SatCodeHexString.compare(receivedMd5SatCodeHex) == 0)
					cout << "Valid launch code." << endl;
				else
					cout << "Invalid launch code." << endl;
			}
			break;

		case RETV:
			if(valueFound == false)
				cout << "Launch code not found for day " << part/TOTAL_PARTS + 1;
			else
				cout << receivedMd5SatCodeHex << endl;
			break;

		case GETALL:
			if(valueFound == false)
				cout << part/TOTAL_PARTS + 1 << ": " << "MISSING" << endl;
			else
				cout << part/TOTAL_PARTS + 1 << ": " << receivedMd5SatCodeHex << endl;
			break;
		}

		partsReceived = 0;
		minKey = INT_MAX;
		parts.clear();
		retrieveLock.unlock();
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
			else if(arguments[0] == "finger") {
				if(totalArguments == 1) {
					finger();
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
					mode = VERIFY;
					satCode =arguments[2];
					retrieve(arguments[1], TOTAL_PARTS);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "retv") {
				if(totalArguments == 2) {
					mode = RETV;
					retrieve(arguments[1], TOTAL_PARTS);
				} else {
					cout << "Wrong number of arguments" << endl;
				}
			}
			else if(arguments[0] == "getall") {
				if(totalArguments == 3) {
					mode = GETALL;
					getall(arguments[1], arguments[2], TOTAL_PARTS);
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
	}

	pthread_join(serverId, NULL);
	return 0;

}
