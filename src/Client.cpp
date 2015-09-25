/*
 * Client.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/Client.h"

Client client;

cell Client::forward(string key) {

	//Check whether key lies within leaf-set
	string minKey, maxKey;
	for(int i = 0; i < L+1; i++) {
		if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
			if(strcmp(minKey.c_str(), localNode.stateTable.leafSet.closestIds[i].nodeId) > 0)
				minKey = localNode.stateTable.leafSet.closestIds[i].nodeId;
			if(strcmp(maxKey.c_str(), localNode.stateTable.leafSet.closestIds[i].nodeId) < 0)
				maxKey = localNode.stateTable.leafSet.closestIds[i].nodeId;
		}
	}
	int closestNodeIdIndex, min = INT_MAX;
	if(strcmp(minKey.c_str(), key.c_str()) <= 0 && strcmp(maxKey.c_str(), key.c_str()) >= 0) {
		for(int i = 0; i < L+1; i++) {
			if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
				int difference = abs(strcmp(key.c_str(), localNode.stateTable.leafSet.closestIds[i].nodeId));
				if(difference < min) {
					closestNodeIdIndex = i;
					min = difference;
				}
			}
		}
		return localNode.stateTable.leafSet.closestIds[closestNodeIdIndex];
	}

	int l = shl(key, localNode.nodeId);

	//Find entry in routing table
	int row = l, column = (key[l] < 58) ? (key[l] - '0') : (key[l] - 'a' + 10);
	if(strlen(localNode.stateTable.routingTable.entries[row][column].nodeId) != 0) {
		return localNode.stateTable.routingTable.entries[row][column];
	}

	//Rare case
	unsigned int localNodeId, destNodeId, cellNodeId;
	sscanf(localNode.nodeId.c_str(), "%x", &localNodeId);
	sscanf(key.c_str(), "%x", &destNodeId);
	//Find in leaf set
	for(int i = 0; i < L+1; i++) {
		if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
			sscanf(localNode.stateTable.leafSet.closestIds[i].nodeId, "%x", &cellNodeId);
			if(shl(localNode.stateTable.leafSet.closestIds[i].nodeId, key) >= l && (cellNodeId ^ destNodeId) < (localNodeId ^ destNodeId))
				return localNode.stateTable.leafSet.closestIds[i];
		}
	}
	//Find in routing table
	for(int i = 0; i < keyLength; i++) {
		for(int j = 0; j < 16; j++) {
			if(strlen(localNode.stateTable.routingTable.entries[i][j].nodeId) != 0) {
				sscanf(localNode.stateTable.routingTable.entries[i][j].nodeId, "%x", &cellNodeId);
				if(shl(localNode.stateTable.routingTable.entries[i][j].nodeId, key) >= l && (cellNodeId ^ destNodeId) < (localNodeId ^ destNodeId))
					return localNode.stateTable.routingTable.entries[i][j];
			}
		}
	}
	//Find in neighborhood set
	for(int i = 0; i < M; i++) {
		if(strlen(localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId) != 0) {
			sscanf(localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId, "%x", &cellNodeId);
			if(shl(localNode.stateTable.neighbourhoodSet.closestNeighbours[i].nodeId, key) >= l && (cellNodeId ^ destNodeId) < (localNodeId ^ destNodeId))
				return localNode.stateTable.neighbourhoodSet.closestNeighbours[i];
		}
	}

	return localNode.stateTable.leafSet.closestIds[L/2];
}

Client::Client() {

}

Client::~Client() {

}

int Client::send(string ip, string port, string message, string *response) {

	int sockFd, portNo, count;
	struct sockaddr_in serverAddr;
	const unsigned int bufferSize = 8192;
	char buffer[bufferSize];

	portNo = atoi(port.c_str());

	//Create TCP socket
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd < 0)
		return SOCK_CREATE_ERROR;

	//Initialize socket structure
	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	serverAddr.sin_port = htons(portNo);

	//Connect to server
	if(connect(sockFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
		return SOCK_CONNECT_ERROR;

	//Send message to server
	count = write(sockFd, message.c_str(), message.length());
	if(count < 0)
		return SOCK_WRITE_ERROR;

	//Receive response from server
	bzero(buffer, bufferSize);
	count = read(sockFd, buffer, bufferSize-1);
	if(count < 0)
		return SOCK_READ_ERROR;

	*response = string(buffer);

	close(sockFd);
	return 0;

}

string Client::send(string key, string message, message_type type, int hopCount) {

	cell nextHop = forward(key);

	cout << "Next hop: " << nextHop.nodeId << endl;

	if(strcmp(nextHop.nodeId, localNode.nodeId.c_str()) == 0)
		return "Destination reached";

	string response;
	string ip = nextHop.ip;
	string port = nextHop.port;

	//Construct a packet to be sent
	Packet packet;
	packet.header.srcNodeId = localNode.nodeId;
	packet.header.key = key;
	packet.header.hopCount = hopCount;
	packet.header.type = type;
	packet.header.messageLength = message.length();
	packet.message = message;
	message = packet.serialize();
	cout << send(ip, port, message, &response) << endl;

	return response;

}
