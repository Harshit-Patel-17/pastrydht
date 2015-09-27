/*
 * Server.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/Server.h"

Server server;

Server::Server() {

}

Server::~Server() {

}

pthread_mutex_t threadCreation;


void *communicate(void *arg) {

	int *retVal = new int;
	int newSockFd = *(int *)arg;
	const unsigned int bufferSize = 8*1024;
	char buffer[bufferSize];
	int count;

	pthread_mutex_unlock(&threadCreation);

	Packet packetReceived, packetToBeSentBack;
	string response, message, status;
	NodeIdentifier *nodeIdentifier;// = new NodeIdentifier;
	FloodCommand *floodCommand;// = new FloodCommand;
	KeyValue *keyValue;// = new KeyValue;
	StateTable *stateTable;// = new StateTable;
	char *stateTableString, *keyValueString;
	message_type type;

	//Start communicating
	bzero(buffer, bufferSize);
	count = read(newSockFd, buffer, bufferSize);
	if(count < 0) {
		*retVal = SOCK_READ_ERROR;
		close(newSockFd);
		pthread_exit((void *) retVal);
	}

	packetReceived.deserialize(buffer);
	bzero(buffer, bufferSize);
	map<string, string> ::iterator it;

	switch(packetReceived.header.type)
	{
	case JOIN:
	case JOIN_A:
		strcpy(buffer, "join packet received");
		count = write(newSockFd, buffer, strlen(buffer));
		// forward the message to the next hop
		status = client.send(packetReceived.header.key, packetReceived.message, JOIN, packetReceived.header.hopCount + 1);
		if(status.compare("Destination reached") == 0)
			type = (packetReceived.header.type == JOIN_A) ? STATE_TABLE_AZ : STATE_TABLE_Z;
		else
			type = (packetReceived.header.type == JOIN_A) ? STATE_TABLE_A : STATE_TABLE;
		nodeIdentifier = (NodeIdentifier *)packetReceived.message.c_str();
		stateTableManager.send(&(localNode.stateTable), nodeIdentifier->ip, nodeIdentifier->port, packetReceived.header.key, type, packetReceived.header.hopCount + 1);
		break;

	case STATE_TABLE:
	case STATE_TABLE_A:
	case STATE_TABLE_Z:
	case STATE_TABLE_X:
	case STATE_TABLE_AZ:
	case STATE_TABLE_LSET:
	case LSET:
	case ROUTETABLE:
	case NSET:
	case DUMP:
		strcpy(buffer, "State table received");
		count = write(newSockFd, buffer, strlen(buffer));
		stateTable = (StateTable *) packetReceived.message.c_str();
		pthread_mutex_lock(&qaccess);
		stateTableManager.insertInQ(packetReceived.header.srcNodeId, *stateTable, packetReceived.header.type);
		pthread_mutex_unlock(&qaccess);
		break;

	case PUT:
		strcpy(buffer, "put packet received");
		count = write(newSockFd, buffer, strlen(buffer));
		status = client.send(packetReceived.header.key, packetReceived.message, packetReceived.header.type, packetReceived.header.hopCount + 1);
		keyValue = (KeyValue *) packetReceived.message.c_str();
		if(status.compare("Destination reached") == 0) {
			pthread_mutex_lock(&htaccess);
			localNode.HT[keyValue->key] = keyValue->value;
			pthread_mutex_unlock(&htaccess);
		}
		//for(it = localNode.HT.begin(); it != localNode.HT.end(); it++)
			//cout << it->first << ":" << it->second << endl;
		break;

	case GET:
		strcpy(buffer, "get packet received");
		count = write(newSockFd, buffer, strlen(buffer));
		status = client.send(packetReceived.header.key, packetReceived.message, packetReceived.header.type, packetReceived.header.hopCount + 1);
		keyValue = (KeyValue *) packetReceived.message.c_str();
		if(status.compare("Destination reached") == 0) {
			if(localNode.HT.find(keyValue->key) == localNode.HT.end()) {
				keyValue->valueFound = false;
			} else {
				keyValue->valueFound = true;
				strcpy(keyValue->value, localNode.HT[keyValue->key].c_str());
			}
			keyValueString = (char *) keyValue;
			message = "";
			for(unsigned int i = 0; i < sizeof(KeyValue); i++)
				message.push_back(keyValueString[i]);
			packetToBeSentBack.build(localNode.nodeId, packetReceived.header.srcNodeId, 0, VALUE, message);
			client.send(keyValue->ip,keyValue->port,packetToBeSentBack.serialize(),&response);
		}
		break;

	case VALUE:
		strcpy(buffer, "value packet received");
		count = write(newSockFd, buffer, strlen(buffer));
		keyValue = (KeyValue *) packetReceived.message.c_str();
		if(keyValue->valueFound)
			cout << keyValue->value << endl;
		else
			cout << "Key not found!" << endl;
		break;

	case REDISTRIBUTE:
		strcpy(buffer, "redistribute packet received");
		count = write(newSockFd, buffer, strlen(buffer));
		htManager.redistribute();
		break;

	case REPAIR_LSET:
		strcpy(buffer, "repair_lset packet received");
		count = write(newSockFd, buffer, strlen(buffer));
		nodeIdentifier = (NodeIdentifier *)packetReceived.message.c_str();
		stateTableManager.send(&(localNode.stateTable), nodeIdentifier->ip, nodeIdentifier->port, packetReceived.header.key, STATE_TABLE_LSET, packetReceived.header.hopCount + 1);
		break;

	case LSET_REQ:
	case ROUTETABLE_REQ:
	case NSET_REQ:
	case DUMP_REQ:
		strcpy(buffer, "state table component request packet received");
		count = write(newSockFd, buffer, strlen(buffer));
		nodeIdentifier = (NodeIdentifier *)packetReceived.message.c_str();
		if(packetReceived.header.type == LSET_REQ)
			type = LSET;
		else if(packetReceived.header.type == ROUTETABLE_REQ)
			type = ROUTETABLE;
		else if(packetReceived.header.type == NSET_REQ)
			type = NSET;
		else
			type = DUMP;
		stateTableManager.send(&(localNode.stateTable), nodeIdentifier->ip, nodeIdentifier->port, packetReceived.header.key, type, packetReceived.header.hopCount + 1);
		break;

	case FLOOD:
		strcpy(buffer, "flood packet received");
		count = write(newSockFd, buffer, strlen(buffer));
		floodCommand = (FloodCommand *) packetReceived.message.c_str();
		cout << floodCommand->command << " " << floodCommand->arg << endl;
		client.flood(packetReceived.header.srcNodeId, packetReceived.message);
		switch(floodCommand->command) {
		case QUIT:
			if(localNode.stateTable.purge(floodCommand->arg)) {
				localNode.stateTable.print();
				strcpy(localNode.stateTable.dontAccept, floodCommand->arg);
				stateTableManager.repairLeafSet(floodCommand->arg);
			}
			break;

		case SHUTDOWN:
			signal_callback_handler(0);
			break;
		}
		break;
	}

	close(newSockFd);
	pthread_exit(0);
}

void *serverRunner(void *arg) {

	int *retVal = new int;

	Node *node = (Node *) arg;
	int sockFd, newSockFd, port;
	unsigned int clientLen;
	struct sockaddr_in serverAddr, clientAddr;

	//Create TCP socket
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	localNode.serverSockFd = sockFd; //Register server socket
	if(sockFd < 0) {
		*retVal = SOCK_CREATE_ERROR;
		pthread_exit((void *) retVal);
	}

	//Initialize socket structure
	bzero((char *) &serverAddr, sizeof(serverAddr));
	port = atoi((node->port).c_str());
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);

	//Bind the host address with socket
	if(bind(sockFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		*retVal = SOCK_BIND_ERROR;
		close(sockFd);
		pthread_exit((void *) retVal);
	}

	while(1) {
		//Start listening to incoming connections
		listen(sockFd, 5);
		clientLen = sizeof(clientAddr);

		//Accept connection from client
		pthread_mutex_lock(&threadCreation);
		newSockFd = accept(sockFd, (struct sockaddr *) &clientAddr, &clientLen);
		if(newSockFd < 0) {
			*retVal = SOCK_ACCEPT_ERROR;
			close(sockFd);
			pthread_exit((void *) retVal);
		}

		pthread_t communicationThreadId;
		pthread_create(&communicationThreadId, NULL, communicate, (void *) &newSockFd);
	}
	close(sockFd);
	localNode.serverSockFd = -1; //Server socket closed
	delete retVal;
	pthread_exit(0);

}

pthread_t Server::start(Node *node) {

	pthread_t tid;

	pthread_create(&tid, NULL, serverRunner, (void *) node);

	return tid;

}
