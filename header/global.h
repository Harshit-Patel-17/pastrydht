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

/*! An enum type.
 *  Various message types supported.
 */

enum message_type {
	GET,
	PUT,
	STATE_TABLE,
	STATE_TABLE_A,
	STATE_TABLE_Z,
	STATE_TABLE_X,
	STATE_TABLE_AZ,
	STATE_TABLE_LSET,
	JOIN, JOIN_A,
	VALUE,
	REDISTRIBUTE,
	FLOOD,
	REPAIR_LSET,
	LSET_REQ,
	ROUTETABLE_REQ,
	NSET_REQ,
	DUMP_REQ,
	LSET,
	ROUTETABLE,
	NSET,
	DUMP
};
enum flood_command {QUIT, SHUTDOWN};


/*! \brief Contains information pertaining to a node in pastry network.
 */
class Node {
public:
	string nodeIp; /*!< Node IP of local node. */
	string nodeId; /*!< Node ID of local node. */
	string port; /*!< Port number on which server thread is listening. */
	StateTable stateTable; /*!< Pastry state table. */
	map<string, string> HT; /*!< Local Hash Table. */
	int serverSockFd; /*!< File descriptor of server socket. */
	bool isNodeInitialized; /*!< Check if structure is initialized. */
	Node() {
		serverSockFd = -1;
		isNodeInitialized = false;
	}
	virtual ~Node() {}

	int setNode(string nodeIp, string port); /*!< Initializes Node structure. */
	virtual void application(string s); /*!< Virtual function. To be defined by application which uses services of pastry. */
};


/*! \brief Contains key-value pair along with IP and port addresses of sender.
 */
struct KeyValue {
	char key[9]; /*!< Hex of CRC32 hash. */
	char value[50]; /*!< Value corresponding to the key. */
	char ip[16]; /*!< IP address of sender. */
	char port[6]; /*!< Port address of sender. */
	bool valueFound; /*!< Flag to indicate whether value is valid or not */
};

/*! \brief Contains IP and port addresses of sender.
 */
struct NodeIdentifier {
	char ip[16]; /*!< IP address of sender. */
	char port[6]; /*!< Port address of sender. */
};

/*! \brief Contains command with one optional argument to be sent over flood packet.
 */
struct FloodCommand {
	flood_command command; /*!< Flood command. */
	char arg[10]; /*!< Optional argument. */
};

/*! \brief Header of packet to be sent over network.
 */
struct Header {
	string srcNodeId; /*!< Node ID of sender. */
	string key; /*!< Key to find destination node. */
	int hopCount; /*!< Number of hops packet has traveled. */
	message_type type; /*!< Type of message inside packet. */
	unsigned int messageLength; /*!< Length of message inside packet. */
};

/*! \brief Packet to be sent over network.
 */
struct Packet {
	Header header; /*!< Header of Packet. */
	string message; /*!< Message of Packet. */
	void print(); /*!< Print elements of Packet along with elements of Header. */
	string serialize(); /*!< Convert Packet into character stream */
	int deserialize(char *packet); /*!< Construct Packet from character stream. */
	void build(string srcNodeId, string key, int hopCount, message_type type, string message); /*!< Initialize memebers of Packet. */
};

extern Node localNode;
extern const int keyLengthInBits;
extern const int b, l, m;
extern const int keyLength;

int shl(string s1, string s2);
void signal_callback_handler(int signum);
#endif /* HEADER_GLOBAL_H_ */
