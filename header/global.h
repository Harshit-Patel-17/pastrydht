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
#include <math.h>
#include <boost/crc.hpp>
#include <sys/socket.h>
#include "StateTable.h"
#include "Ui.h"

using namespace std;
using namespace boost;

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
	StateTable stateTable;
	void (*callback)(string);
	int serverSockFd;
	bool isNodeInitialized;
	Node() {
		serverSockFd = -1;
		callback = NULL;
		isNodeInitialized = false;
	}
	int setNode(string nodeIp, string port, void (*callback)(string));
	void join(string destNodeIp, string destPort);
};

enum message_type {APP_DATA, STATE_TABLE, CONTROL_DATA, JOIN, RESPONSE};

struct NodeIdentifier {
	char ip[16];
	char port[6];
};

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
	int deserialize(char *packet);
};

extern Node localNode;
extern Ui ui;
extern const int keyLengthInBits;
extern const int b, l, m;
extern const int keyLength;
#endif /* HEADER_GLOBAL_H_ */
