/*
 * stateTableManager.cpp
 *
 *  Created on: 19-Sep-2015
 *      Author: harshit
 */

#include "../header/StateTableManager.h"

StateTableManager stateTableManager;
pthread_mutex_t lock;
pthread_mutex_t qaccess;
pthread_mutex_t htaccess;
pthread_mutex_t staccess;

bool StateTableManager::isInLeafSet(cell nodeCell) {

	for(int i = 0; i < L+1; i++) {
		if(strcmp(nodeCell.nodeId, localNode.stateTable.leafSet.closestIds[i].nodeId) == 0)
			return true;
	}

	return false;

}

bool StateTableManager::isInNeighborhoodSet(cell nodeCell) {

	for(int i = 0; i < M; i++) {
		if(strcmp(nodeCell.nodeId, localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId) == 0)
			return true;
	}

	return false;

}

void StateTableManager::updateLeafSet(pair<pair<string, StateTable*>, message_type> QElem) {

	for(int i = 0; i < L+1; i++) {
		if(strlen(QElem.first.second->leafSet.closestIds[i].nodeId) != 0)
			updateLeafSet(QElem.first.second->leafSet.closestIds[i]);
	}

}

void StateTableManager::updateLeafSet(cell nodeCell) {

	if(isInLeafSet(nodeCell))
		return;

	if(strcmp(localNode.stateTable.dontAccept, nodeCell.nodeId) == 0)
		return;

	if(strcmp(nodeCell.nodeId, localNode.nodeId.c_str()) < 0) {
		int minIndex = 0;
		string minString;
		for(int i = 0; i < L/2; i++) {
			if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
				if(minString.length() == 0) {
					minIndex = i;
					minString = localNode.stateTable.leafSet.closestIds[i].nodeId;
				} else {
					if(strcmp(minString.c_str(), localNode.stateTable.leafSet.closestIds[i].nodeId) > 0) {
						minIndex = i;
						minString = localNode.stateTable.leafSet.closestIds[i].nodeId;
					}
				}
			} else {
				localNode.stateTable.leafSet.closestIds[i] = nodeCell;
				return;
			}
		}
		if(strcmp(minString.c_str(), nodeCell.nodeId) < 0)
			localNode.stateTable.leafSet.closestIds[minIndex] = nodeCell;
	} else {
		int maxIndex = 0;
		string maxString;
		for(int i = L/2+1; i < L+1; i++) {
			if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
				if(maxString.length() == 0) {
					maxIndex = i;
					maxString = localNode.stateTable.leafSet.closestIds[i].nodeId;
				} else {
					if(strcmp(maxString.c_str(), localNode.stateTable.leafSet.closestIds[i].nodeId) < 0) {
						maxIndex = i;
						maxString = localNode.stateTable.leafSet.closestIds[i].nodeId;
					}
				}
			} else {
				localNode.stateTable.leafSet.closestIds[i] = nodeCell;
				return;
			}
		}
		if(strcmp(maxString.c_str(), nodeCell.nodeId) > 0)
			localNode.stateTable.leafSet.closestIds[maxIndex] = nodeCell;
	}

}

void StateTableManager::updateNeighbourhoodSet(pair<pair<string, StateTable*>, message_type> QElem) {

	for(int i = 0; i < M; i++) {
		if(strlen(QElem.first.second->neighbourhoodSet.closestNeighbours[i].nodeId) != 0)
			updateNeighbourhoodSet(QElem.first.second->neighbourhoodSet.closestNeighbours[i]);
	}

}

void StateTableManager::updateNeighbourhoodSet(cell nodeCell) {

	if(isInNeighborhoodSet(nodeCell))
		return;

	if(strcmp(localNode.stateTable.dontAccept, nodeCell.nodeId) == 0)
		return;

	unsigned int localNodeId, nodeId, nodeCellNodeId;
	sscanf(localNode.nodeId.c_str(), "%x", &localNodeId);

	unsigned int maxIndex, maxDifference = 0;
	for(int i = 0; i < M; i++) {
		if(strlen(localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId) != 0) {
			sscanf(localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId, "%x", &nodeId);
			if((nodeId ^ localNodeId) > maxDifference) {
				maxDifference = nodeId ^ localNodeId;
				maxIndex = i;
			}
		} else {
			localNode.stateTable.neighbourhoodSet.closestNeighbours[i] = nodeCell;
			return;
		}
	}

	sscanf(localNode.stateTable.neighbourhoodSet.closestNeighbours[maxIndex].nodeId, "%x", &nodeId);
	sscanf(nodeCell.nodeId, "%x", &nodeCellNodeId);
	if((nodeId ^ localNodeId) > (nodeCellNodeId ^ localNodeId))
		localNode.stateTable.neighbourhoodSet.closestNeighbours[maxIndex] = nodeCell;

}

bool StateTableManager::allStateTableReceived() {

	int maxHopCount = hopCountVector[0];

	for(unsigned int i = 0; i < hopCountVector.size(); i++)
		maxHopCount = max(maxHopCount, hopCountVector[i]);

	return maxHopCount == hopCountVector.size();
}

void StateTableManager::clearAll() {

	zReceived = false;
	hopCountVector.clear();

}

void StateTableManager::send(StateTable *stateTable, string ip, string port, string key, message_type type, int hopCount) {

	stateTable->hopCount = hopCount;

	char *stateTableString = (char *) stateTable;
	string message, response;
	for(unsigned int i = 0; i < sizeof(StateTable); i++)
		message.push_back(stateTableString[i]);

	Packet packet;
	packet.build(localNode.nodeId, key, 0, type, message);

	client.send(ip, port, packet.serialize(), &response);

}

void StateTableManager::repairLeafSet(string purgedNodeId) {

	Packet packet;
	string response, message;

	struct NodeIdentifier *nodeIdentifier = new NodeIdentifier;
	strcpy(nodeIdentifier->ip,localNode.nodeIp.c_str());
	strcpy(nodeIdentifier->port,localNode.port.c_str());

	char *buffer = (char *) nodeIdentifier;
	for(unsigned int i = 0; i < sizeof(NodeIdentifier); i++)
		message.push_back(buffer[i]);

	delete nodeIdentifier;

	if(purgedNodeId.compare(localNode.nodeId) < 0) {
		//Find min from left part of leaf set
		cell minInLeft;
		strcpy(minInLeft.nodeId, "\0");
		for(int i = 0; i < L/2; i++) {
			if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
				if(strlen(minInLeft.nodeId) == 0) {
					minInLeft = localNode.stateTable.leafSet.closestIds[i];
				} else {
					if(strcmp(localNode.stateTable.leafSet.closestIds[i].nodeId, minInLeft.nodeId) < 0)
						minInLeft = localNode.stateTable.leafSet.closestIds[i];
				}
			}
		}
		if(strlen(minInLeft.nodeId) != 0) {
			packet.build(localNode.nodeId, minInLeft.nodeId, 0, REPAIR_LSET, message);
			client.send(minInLeft.ip, minInLeft.port, packet.serialize(), &response);
		}
	}

	if(purgedNodeId.compare(localNode.nodeId) > 0) {
		//Find max from left part of leaf set
		cell maxInRight;
		strcpy(maxInRight.nodeId, "\0");
		for(int i = L/2+1; i < L+1; i++) {
			if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
				if(strlen(maxInRight.nodeId) == 0) {
					maxInRight = localNode.stateTable.leafSet.closestIds[i];
				} else {
					if(strcmp(localNode.stateTable.leafSet.closestIds[i].nodeId, maxInRight.nodeId) > 0)
						maxInRight = localNode.stateTable.leafSet.closestIds[i];
				}
			}
		}
		if(strlen(maxInRight.nodeId) != 0) {
			packet.build(localNode.nodeId, maxInRight.nodeId, 0, REPAIR_LSET, message);
			client.send(maxInRight.ip, maxInRight.port, packet.serialize(), &response);
		}
	}

}

StateTableManager::StateTableManager() {

	zReceived = false;
	hopCountVector.clear();

}

StateTableManager::~StateTableManager() {
	// TODO Auto-generated destructor stub
}

void StateTableManager::joinPhase1(string destNodeIp, string destPort) {

	Packet packet;
	packet.header.srcNodeId = localNode.nodeId;
	packet.header.key = localNode.nodeId;
	packet.header.hopCount = 0;
	packet.header.type = JOIN_A;
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

}

void StateTableManager::joinPhase2() {

	Packet packet;
	packet.header.messageLength = sizeof(StateTable);
	packet.header.srcNodeId = localNode.nodeId;
	packet.header.type = STATE_TABLE_X;
	char *stateTableString = (char *)&(localNode.stateTable);
	packet.message = "";
	for(unsigned int i = 0; i < sizeof(StateTable); i++)
		packet.message.push_back(stateTableString[i]);

	map<string, bool> hasBeenSentStateTable;

	string ip, port, response;
	//Send state table to nodes in leaf-set
	for(int i = 0; i < L+1; i++) {
		if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0 && i != L/2 &&
				hasBeenSentStateTable.find(localNode.stateTable.leafSet.closestIds[i].nodeId) == hasBeenSentStateTable.end()) {
			packet.header.key = localNode.stateTable.leafSet.closestIds[i].nodeId;
			ip = localNode.stateTable.leafSet.closestIds[i].ip;
			port = localNode.stateTable.leafSet.closestIds[i].port;
			client.send(ip, port, packet.serialize(), &response);
			hasBeenSentStateTable[localNode.stateTable.leafSet.closestIds[i].nodeId] = true;
		}
	}

	//Send state table to nodes in routing table
	for(int i = 0; i < keyLength; i++) {
		int l = (localNode.nodeId[i] < 58) ? (localNode.nodeId[i] - '0') : (localNode.nodeId[i] - 'a' + 10); //Find useless column in row
		for(int j = 0; j < 16; j++) {
			if(strlen(localNode.stateTable.routingTable.entries[i][j].nodeId) != 0 && j != l &&
					hasBeenSentStateTable.find(localNode.stateTable.routingTable.entries[i][j].nodeId) == hasBeenSentStateTable.end()) {
				packet.header.key = localNode.stateTable.routingTable.entries[i][j].nodeId;
				ip = localNode.stateTable.routingTable.entries[i][j].ip;
				port = localNode.stateTable.routingTable.entries[i][j].port;
				client.send(ip, port, packet.serialize(), &response);
				hasBeenSentStateTable[localNode.stateTable.routingTable.entries[i][j].nodeId] = true;
			}
		}
	}

	//Send state table to nodes in neighbourhood set
	for(int i = 0; i < M; i++) {
		if(strlen(localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId) != 0) {
			if(strcmp(localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId, localNode.nodeId.c_str()) != 0 &&
					hasBeenSentStateTable.find(localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId) == hasBeenSentStateTable.end()) {
				packet.header.key = localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId;
				ip = localNode.stateTable.neighbourhoodSet.closestNeighbours[i].ip;
				port = localNode.stateTable.neighbourhoodSet.closestNeighbours[i].port;
				client.send(ip, port, packet.serialize(), &response);
				hasBeenSentStateTable[localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId] = true;
			}
		}
	}

}

void StateTableManager::redistributePhase() {

	//Find max from left part of leaf set
	cell maxInLeft;
	strcpy(maxInLeft.nodeId, "\0");
	for(int i = 0; i < L/2; i++) {
		if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
			if(strlen(maxInLeft.nodeId) == 0) {
				maxInLeft = localNode.stateTable.leafSet.closestIds[i];
			} else {
				if(strcmp(localNode.stateTable.leafSet.closestIds[i].nodeId, maxInLeft.nodeId) > 0)
					maxInLeft = localNode.stateTable.leafSet.closestIds[i];
			}
		}
	}

	//Find min from right part of leaf set
	cell minInRight;
	strcpy(minInRight.nodeId, "\0");
	for(int i = L/2+1; i < L+1; i++) {
		if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
			if(strlen(minInRight.nodeId) == 0) {
				minInRight = localNode.stateTable.leafSet.closestIds[i];
			} else {
				if(strcmp(localNode.stateTable.leafSet.closestIds[i].nodeId, minInRight.nodeId) < 0)
					minInRight = localNode.stateTable.leafSet.closestIds[i];
			}
		}
	}

	//Send redistribute packet
	Packet packet;
	string response;
	if(strlen(maxInLeft.nodeId) != 0) {
		packet.build(localNode.nodeId, maxInLeft.nodeId, 0, REDISTRIBUTE, "");
		client.send(maxInLeft.ip, maxInLeft.port, packet.serialize(), &response);
	}
	if(strlen(minInRight.nodeId) != 0) {
		packet.build(localNode.nodeId, minInRight.nodeId, 0, REDISTRIBUTE, "");
		client.send(minInRight.ip, minInRight.port, packet.serialize(), &response);
	}
}

void StateTableManager::insertInQ(string nodeId, StateTable stateTable, message_type type) {

	StateTable *newStateTable = new StateTable(stateTable);

	Q.push(make_pair(make_pair(nodeId, newStateTable), type));

	if(Q.size() == 1)
		pthread_mutex_unlock(&lock);

}

void *stateTableManagerRunner(void *arg) {

	queue< pair<pair<string, StateTable*>, message_type> > *Q = (queue< pair<pair<string, StateTable*>, message_type> > *)arg;
	while(1) {
		if(Q->size() == 0)
			pthread_mutex_lock(&lock);
		else {
			pair<pair<string, StateTable*>, message_type> QElem = Q->front();
			pthread_mutex_lock(&qaccess);
			Q->pop();
			pthread_mutex_unlock(&qaccess);

			int l = shl(localNode.nodeId, QElem.first.first);
			for(int i = 0; i < 16; i++) {
				if(strlen(localNode.stateTable.routingTable.entries[l][i].nodeId) == 0)
					if(strcmp(localNode.stateTable.dontAccept, QElem.first.second->routingTable.entries[l][i].nodeId) != 0)
						localNode.stateTable.routingTable.entries[l][i] = QElem.first.second->routingTable.entries[l][i];
			}

			switch(QElem.second) {
			case STATE_TABLE:
				stateTableManager.hopCountVector.push_back(QElem.first.second->hopCount);
				if(stateTableManager.zReceived) {
					if(stateTableManager.allStateTableReceived()) {
						stateTableManager.joinPhase2();
						stateTableManager.clearAll();
						stateTableManager.redistributePhase();
					}
				}
				break;

			case STATE_TABLE_A:
				pthread_mutex_lock(&staccess);
				stateTableManager.updateNeighbourhoodSet(QElem);
				stateTableManager.hopCountVector.push_back(QElem.first.second->hopCount);
				pthread_mutex_unlock(&staccess);
				if(stateTableManager.zReceived) {
					if(stateTableManager.allStateTableReceived()) {
						stateTableManager.joinPhase2();
						stateTableManager.clearAll();
						stateTableManager.redistributePhase();
					}
				}
				break;

			case STATE_TABLE_Z:
				pthread_mutex_lock(&staccess);
				stateTableManager.updateLeafSet(QElem);
				stateTableManager.zReceived = true;
				stateTableManager.hopCountVector.push_back(QElem.first.second->hopCount);
				pthread_mutex_unlock(&staccess);
				if(stateTableManager.allStateTableReceived()) {
					stateTableManager.joinPhase2();
					stateTableManager.clearAll();
					stateTableManager.redistributePhase();
				}
				break;

			case STATE_TABLE_X:
				pthread_mutex_lock(&staccess);
				stateTableManager.updateLeafSet(QElem.first.second->leafSet.closestIds[L/2]);
				stateTableManager.updateNeighbourhoodSet(QElem.first.second->leafSet.closestIds[L/2]);
				pthread_mutex_unlock(&staccess);
				break;

			case STATE_TABLE_AZ:
				pthread_mutex_lock(&staccess);
				stateTableManager.updateNeighbourhoodSet(QElem);
				stateTableManager.updateLeafSet(QElem);
				stateTableManager.zReceived = true;
				stateTableManager.hopCountVector.push_back(QElem.first.second->hopCount);
				pthread_mutex_unlock(&staccess);
				if(stateTableManager.allStateTableReceived()) {
					stateTableManager.joinPhase2();
					stateTableManager.clearAll();
					stateTableManager.redistributePhase();
				}
				break;

			case STATE_TABLE_LSET:
				pthread_mutex_lock(&staccess);
				stateTableManager.updateLeafSet(QElem);
				strcpy(localNode.stateTable.dontAccept, "\0");
				pthread_mutex_unlock(&staccess);
				localNode.stateTable.print();
				break;

			case LSET:
				QElem.first.second->leafSet.print();
				break;

			case ROUTETABLE:
				QElem.first.second->routingTable.print();
				break;

			case NSET:
				QElem.first.second->neighbourhoodSet.print();
				break;

			case DUMP:
				QElem.first.second->print();
				break;
			}
		}
	}

	pthread_exit(NULL);

}

pthread_t StateTableManager::startManager() {

	pthread_t managerId;

	pthread_create(&managerId, NULL, stateTableManagerRunner, (void *) &Q);

	return managerId;

}
