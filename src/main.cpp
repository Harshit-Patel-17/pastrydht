/*
 * main.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/global.h"
#include "../header/Server.h"
#include "../header/Client.h"

void getFromPastry(string message) {

	cout << message;

}

int main(void) {

	//Set up local node
	string ip = "127.0.0.1";
	string port = "3000";
	localNode.setNode(ip, port, getFromPastry);

	//Set up app and Ui
	string appName = "pastry";
	string delim = "$";
	ui.init(appName, delim, GOLD);

	//Create client & server objects
	Server server;
	Client client;

	pthread_t serverId = server.start(&localNode);

	int totalArguments;
	string *arguments;
	string key = "Something";
	string message = "This should be printed on screen.";
	while(1) {
		totalArguments = ui.getInputLine(&arguments);
		string response = client.send(key, message);
		cout << response << "Hello" << endl;
	}

	pthread_join(serverId, NULL);
	return 0;

}
