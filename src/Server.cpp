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
	const unsigned int bufferSize = 1024;
	char buffer[bufferSize];
	struct sockaddr_in serverAddr, clientAddr;
	Packet packet;

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
		packet.deserialize(buffer);
		packet.print();

		//string message = packet.message;
		//node->callback(message);

		bzero(buffer, bufferSize);
		strcpy(buffer, "I got your message");
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
