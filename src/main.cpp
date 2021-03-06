/*
 * main.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/global.h"
#include "../header/Server.h"
#include "../header/Client.h"
#include "../header/StateTableManager.h"

void getFromPastry(string message) {

	cout << message;

}

int main(void) {

	//Set up local node
	string ip = "127.0.0.1";
	string port = "3000";
	cin >> port;
	localNode.setNode(ip, port, getFromPastry);

	//Set up app and Ui
	string appName = "pastry";
	string delim = "$";
	ui.init(appName, delim, GOLD);

	//Start server
	pthread_t serverId = server.start(&localNode);

	int totalArguments;
	string *arguments;
	string key = "aaaaaaaa";
	string message = (char *) &(localNode.stateTable);
	while(1) {
		totalArguments = ui.getInputLine(&arguments);
		if(port != "3000")
		stateTableManager.join("127.0.0.1", "3000");
		//string response = client.send(key, message);
		//cout << endl << response << endl;
	}

	pthread_join(serverId, NULL);
	return 0;

}
