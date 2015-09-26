/*
 * HTManager.cpp
 *
 *  Created on: 26-Sep-2015
 *      Author: harshit
 */

#include "../header/HTManager.h"

HTManager htManager;

HTManager::HTManager() {
	// TODO Auto-generated constructor stub

}

HTManager::~HTManager() {
	// TODO Auto-generated destructor stub
}

void HTManager::redistribute() {

	map<string, string> temp;
	map<string, string>::iterator it;

	for(it = localNode.HT.begin(); it != localNode.HT.end(); it++)
		temp[it->first] = it->second;

	localNode.HT.clear();

	KeyValue keyValue;
	char *keyValueString;
	string message, response;
	Packet packet;
	for(it = temp.begin(); it != temp.end(); it++) {
		strcpy(keyValue.ip, localNode.nodeIp.c_str());
		strcpy(keyValue.port, localNode.port.c_str());
		strcpy(keyValue.key, (it->first).c_str());
		strcpy(keyValue.value, temp[it->first].c_str());

		keyValueString = (char *) &(keyValue);
		message = "";
		for(unsigned int i = 0; i < sizeof(KeyValue); i++)
			message.push_back(keyValueString[i]);

		packet.build(localNode.nodeId, it->first, 0, PUT, message);
		cout << "PUTTING KEY: " << it->first << ":" << it->second << endl;
		cout << client.send(localNode.nodeIp, localNode.port, packet.serialize(), &response) << endl;
		cout << "SENT" << endl;
	}

}

