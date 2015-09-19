/*
 * Help.cpp
 *
 *  Created on: 19-Sep-2015
 *      Author: harshit
 */

#include "../header/Commands.h"

void printHelp() {

	//TODO: Write help text
	cout << "Program Help" << endl;

}

void port(string port) {

	int portNum = atoi(port.c_str());
	if(portNum > 1024 && portNum < 65536) {
		localNode.setNode("127.0.0.1", port);
		cout << "Port set to " << port << endl;
	} else {
		cout << "Invalid port number" << endl;
	}

}

pthread_t create() {

	pthread_t serverId = server.start(&localNode);
	if(serverId < 0)
		cout << "Cannot create server thread" << endl;

	return serverId;
}

void join(string ip, string port) {

	//TODO: Check correctness of ip and port format
	stateTableManager.join(ip, port);

}

void put(string key, string value) {

	cout << "Put command executed" << endl;

}

void get(string key) {

	cout << "Get command executed" << endl;

}

void lset() {

	localNode.stateTable.leafSet.print();

}

void routetable() {

	localNode.stateTable.routingTable.print();

}

void nset() {

	localNode.stateTable.neighbourhoodSet.print();

}

void dump() {

	localNode.stateTable.print();

}

