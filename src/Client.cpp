/*
 * Client.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/Client.h"

Client::Client() {

}

Client::~Client() {

}

int Client::send(string ip, string port, string message, string *response) {

	int sockFd, portNo, count;
	struct sockaddr_in serverAddr;
	const unsigned int bufferSize = 1024;
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

string Client::send(string key, string message) {

	//Todo: Write logic to obtain destination nodeIp from key using Routing Table

	string response;
	string ip = "127.0.0.1"; //Test Ip
	string port = "3000"; //Test Port
	send(ip, port, message, &response);

	return response;

}
