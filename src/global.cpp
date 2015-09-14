/*
 * global.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/global.h"

void signal_callback_handler(int signum) {

	cout << endl << "Shutting server down..." << endl;
	if(localNode.serverSockFd != -1) {
		shutdown(localNode.serverSockFd, SHUT_RDWR);
		close(localNode.serverSockFd);
	}
	exit(signum);
}

int Node::setNode(string nodeIp, string port, void (*callback)(string)) {

	if(isNodeInitialized)
		return -1;
	this->nodeIp = nodeIp;
	this->port = port;
	this->callback = callback;
	this->isNodeInitialized = true;

	//Register signals and signal handler
	signal(SIGINT, signal_callback_handler);
	signal(SIGTSTP, signal_callback_handler);

	//TODO: Compute CRC-32bit hash for nodeIp:port here

	return 0;
}
