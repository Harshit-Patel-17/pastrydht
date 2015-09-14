/*
 * global.h
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_GLOBAL_H_
#define HEADER_GLOBAL_H_


#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include "Ui.h"

using namespace std;

#define SOCK_CREATE_ERROR -1
#define SOCK_BIND_ERROR -2
#define SOCK_ACCEPT_ERROR -3
#define SOCK_READ_ERROR -4
#define SOCK_WRITE_ERROR -5
#define SOCK_CONNECT_ERROR -6

struct Node {
	string nodeIp;
	string nodeId;
	string port;
	void (*callback)(string);
	int serverSockFd;
	bool isNodeInitialized;
	Node() {
		serverSockFd = -1;
		callback = NULL;
		isNodeInitialized = false;
	}
	int setNode(string nodeIp, string port, void (*callback)(string));
};

enum message_type {APP_DATA, ROUTING_TABLE, CONTROL_DATA};

struct Header {
	string srcNodeId;
	string key;
	message_type type;
	unsigned int messageLength;
};

struct Packet {
	Header header;
	string message;
	void print();
	string serialize();
	int deserialize(string packet);
};

extern Node localNode;
extern Ui ui;
extern int keyLengthInBits;
extern int b;
extern int keyLength;
#endif /* HEADER_GLOBAL_H_ */
