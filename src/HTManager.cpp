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

	cell leafNodeCell;
	strcpy(leafNodeCell.nodeId, "\0");

	for(int i = 0; i < L+1; i++) {
		if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
			leafNodeCell = localNode.stateTable.leafSet.closestIds[i];
			break;
		}
	}

	if(strlen(leafNodeCell.nodeId) == 0)
		return;

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
		client.send(leafNodeCell.ip, leafNodeCell.port, packet.serialize(), &response);
	}

}

