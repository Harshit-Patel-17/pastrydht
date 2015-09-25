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
#include <map>
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

class Node {
public:
	string nodeIp;
	string nodeId;
	string port;
	StateTable stateTable;
	map<string, string> HT;
	int serverSockFd;
	bool isNodeInitialized;
	Node() {
		serverSockFd = -1;
		isNodeInitialized = false;
	}
	virtual ~Node() {}

	int setNode(string nodeIp, string port);
	virtual void application(string s);
};

enum message_type {GET, PUT, STATE_TABLE, STATE_TABLE_A, STATE_TABLE_Z, STATE_TABLE_X, STATE_TABLE_AZ, JOIN, JOIN_A, VALUE};

struct KeyValue {
	char key[9];
	char value[50];
	char ip[16];
	char port[6];
	bool valueFound;
};

struct NodeIdentifier {
	char ip[16];
	char port[6];
};

struct Header {
	string srcNodeId;
	string key;
	int hopCount;
	message_type type;
	unsigned int messageLength;
};

struct Packet {
	Header header;
	string message;
	void print();
	string serialize();
	int deserialize(char *packet);
	void build(string srcNodeId, string key, int hopCount, message_type type, string message);
};

extern Node localNode;
extern Ui ui;
extern const int keyLengthInBits;
extern const int b, l, m;
extern const int keyLength;

int shl(string s1, string s2);
#endif /* HEADER_GLOBAL_H_ */
