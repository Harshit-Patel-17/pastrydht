/*
 * stateTableManager.cpp
 *
 *  Created on: 19-Sep-2015
 *      Author: harshit
 */

#include "../header/StateTableManager.h"

StateTableManager stateTableManager;

StateTableManager::StateTableManager() {
	// TODO Auto-generated constructor stub

}

StateTableManager::~StateTableManager() {
	// TODO Auto-generated destructor stub
}


void StateTableManager::join(string destNodeIp, string destPort) {
	Packet packet;
	packet.header.srcNodeId = localNode.nodeId;
	packet.header.key = localNode.nodeId;
	packet.header.type = JOIN;
	packet.header.messageLength = sizeof(NodeIdentifier);

	struct NodeIdentifier *nodeIdentifier = new NodeIdentifier;
	strcpy(nodeIdentifier->ip,localNode.nodeIp.c_str());
	strcpy(nodeIdentifier->port,localNode.port.c_str());

	char *buffer = (char *) nodeIdentifier;
	for(unsigned int i = 0; i < sizeof(NodeIdentifier); i++)
		packet.message.push_back(buffer[i]);

	delete nodeIdentifier;

	string message = packet.serialize();
	string response;
	client.send(destNodeIp, destPort, message, &response);

	cout<<response<<endl;
}
