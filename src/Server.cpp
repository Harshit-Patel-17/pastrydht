/*
 * Server.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/Server.h"

Server::Server() {

}

Server::~Server() {

}

void *serverRunner(void *arg) {

	int *retVal = new int;

	Node *node = (Node *) arg;
	int sockFd, newSockFd, port, count;
	unsigned int clientLen;
	const unsigned int bufferSize = 4096;
	char buffer[bufferSize];
	struct sockaddr_in serverAddr, clientAddr;
	Packet packetRecieved, packetToBeSent;
	string response = "";

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
		newSockFd = accept(sockFd, (struct sockaddr *) &clientAddr, &clientLen);
		if(newSockFd < 0) {
			*retVal = SOCK_ACCEPT_ERROR;
			close(sockFd);
			pthread_exit((void *) retVal);
		}

		//Start communicating
		bzero(buffer, bufferSize);
		count = read(newSockFd, buffer, bufferSize);
		if(count < 0) {
			*retVal = SOCK_READ_ERROR;
			close(sockFd);
			close(newSockFd);
			pthread_exit((void *) retVal);
		}
		packetRecieved.deserialize(buffer);
		//packet.print();
		RoutingTableStructure *routingTable = new RoutingTableStructure;
		routingTable = (RoutingTableStructure *) packetRecieved.message.c_str();
		routingTable->print();

		//string message = packet.message;
		//node->callback(message);

		packetToBeSentBack.header.srcNodeId = localNode.nodeId;
		packetToBeSentBack.header.key = "";

		bzero(buffer, bufferSize);
		switch(packet.header.type)
		{
			JOIN: // forward the message to the next hop
				  cout<<client.send(packetRecieved.header.key, packetRecieved.message, packetRecieved.header.type)<<endl;

				  // send back the packet(with state table) back to the newly joining node
				  NodeIdentifier *nodeIdentifier = new NodeIdentifier;
				  nodeIdentifier = (NodeIdentifier *)packetRecieved.message.c_str();
				  packetToBeSentBack.message = (char *)&(localNode.stateTable.routingTable);
				  packetToBeSentBack.header.type = STATE_TABLE;
				  client.send(nodeIdentifier.ip,nodeIdentifier.port,packetToBeSentBack.serialize(),&response);
				  cout<<response<<endl;
				  strcpy(buffer, "join packet recieved");
				  break;

			STATE_TABLE: // to be used by state manager
						 StateTable *stateTable = new StateTable;
						 stateTable = (StateTable *) packetRecieved.message.c_str();
						 stateTable.print();
						 strcpy(buffer, "State table recieved");
						 break;

		}
		//strcpy(buffer, packetToBeSent.serialize());
		count = write(newSockFd, buffer, strlen(buffer));
		if(count < 0) {
			*retVal = SOCK_WRITE_ERROR;
			close(sockFd);
			close(newSockFd);
			pthread_exit((void *) retVal);
		}

		close(newSockFd);
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
