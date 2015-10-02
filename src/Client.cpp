/*
 * Client.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/Client.h"

Client client;

int Client::difference(string hex1, string hex2) {

	unsigned int hexNum1, hexNum2;
	sscanf(hex1.c_str(), "%x", &hexNum1);
	sscanf(hex2.c_str(), "%x", &hexNum2);

	return hexNum1 - hexNum2;

}

cell Client::forward(string key) {

	//Check whether key lies within leaf-set
	string minKey = "ffffffff", maxKey = "00000000";
	for(int i = 0; i < L+1; i++) {
		if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
			if(strcmp(minKey.c_str(), localNode.stateTable.leafSet.closestIds[i].nodeId) > 0)
				minKey = localNode.stateTable.leafSet.closestIds[i].nodeId;
			if(strcmp(maxKey.c_str(), localNode.stateTable.leafSet.closestIds[i].nodeId) < 0)
				maxKey = localNode.stateTable.leafSet.closestIds[i].nodeId;
		}
	}
	int closestNodeIdIndex;
	unsigned int min = UINT_MAX;
	if(strcmp(minKey.c_str(), key.c_str()) <= 0 && strcmp(maxKey.c_str(), key.c_str()) >= 0) {
		for(int i = 0; i < L+1; i++) {
			if(strlen(localNode.stateTable.leafSet.closestIds[i].nodeId) != 0) {
				unsigned int absDifference = abs(difference(key.c_str(), localNode.stateTable.leafSet.closestIds[i].nodeId));
				if(absDifference < min) {
					closestNodeIdIndex = i;
					min = absDifference;
				}
			}
		}
		return localNode.stateTable.leafSet.closestIds[closestNodeIdIndex];
	}

	int l = shl(key, localNode.nodeId);
	//Find entry in routing table
	int row = l, column = (key[l] <= '9') ? (key[l] - '0') : (key[l] - 'a' + 10);
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
	const unsigned int bufferSize = 8*1024;
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

	if(strcmp(nextHop.nodeId, localNode.nodeId.c_str()) == 0)
		return "Destination reached";

	string response;
	string ip = nextHop.ip;
	string port = nextHop.port;

	//Construct a packet to be sent
	Packet packet;
	packet.build(localNode.nodeId, key, hopCount, type, message);
	send(ip, port, packet.serialize(), &response);

	return response;

}

void Client::flood(string prevKey, string message) {

	Packet packet;
	string response;

	if(prevKey.compare(localNode.nodeId) <= 0) {
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
		if(strlen(minInRight.nodeId) != 0) {
			packet.build(localNode.nodeId, minInRight.nodeId, 0, FLOOD, message);
			client.send(minInRight.ip, minInRight.port, packet.serialize(), &response);
		}
	}

	if(prevKey.compare(localNode.nodeId) >= 0) {
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
		if(strlen(maxInLeft.nodeId) != 0) {
			packet.build(localNode.nodeId, maxInLeft.nodeId, 0, FLOOD, message);
			client.send(maxInLeft.ip, maxInLeft.port, packet.serialize(), &response);
		}
	}

}
