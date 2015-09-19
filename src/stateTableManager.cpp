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
	stateTableManager.startManager();
	client.send(destNodeIp, destPort, message, &response);

	cout<< "Remote: " << response << endl;
}

pthread_mutex_t lock;

void StateTableManager::insertInQ(string nodeId, StateTable stateTable) {

	StateTable *newStateTable = new StateTable(stateTable);

	Q.push(make_pair(nodeId, newStateTable));

	if(Q.size() == 1)
		pthread_mutex_unlock(&lock);

}

void *stateTableManagerRunner(void *arg) {

	cout << "In runner" << endl;
	queue< pair<string, StateTable*> > *Q = (queue< pair<string, StateTable*> > *)arg;
	while(1) {
		if(Q->size() == 0)
			pthread_mutex_lock(&lock);
		else {
			pair<string, StateTable*> QElem = Q->front();
			Q->pop();

			cout << QElem.first << endl;
			QElem.second->print();
		}
	}

	pthread_exit(NULL);

}

pthread_t StateTableManager::startManager() {

	pthread_t managerId;

	pthread_create(&managerId, NULL, stateTableManagerRunner, (void *) &Q);

	return managerId;

}
