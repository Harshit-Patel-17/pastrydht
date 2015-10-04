/*
 * Help.cpp
 *
 *  Created on: 19-Sep-2015
 *      Author: harshit
 */

#include "../header/Commands.h"

string getIp() {

	struct ifaddrs * ifAddrStruct=NULL;
	struct ifaddrs * ifa=NULL;
	void * tmpAddrPtr=NULL;
	map<string, string> ip_addrs;

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}
		if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			ip_addrs[ifa->ifa_name] = addressBuffer;
		}
	}

	if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);

	if(ip_addrs.find("eth0") != ip_addrs.end())
		return ip_addrs["eth0"];

	if(ip_addrs.find("wlan0") != ip_addrs.end())
			return ip_addrs["wlan0"];

	return ip_addrs["lo"];

}

string md5Hex(string s) {

	unsigned const char *unsignedConstS;
	unsigned char md5S[16];
	char md5SHex[33];
	string md5SHexString;

	unsignedConstS = reinterpret_cast<unsigned const char*>(s.c_str());
	MD5(unsignedConstS, s.length(), md5S);

	memset(md5SHex, 0, 33);
	for(int i = 0; i < 16; i++)
		sprintf(&md5SHex[2*i], "%02x", md5S[i]);

	md5SHexString = md5SHex;
	return md5SHexString;

}

void printHelp() {

    cout << "                   help: Provides a list of commands and their usage details." << endl;
    cout << "               port <x>: Listen on this port for other instances of the program over different nodes." << endl;
	cout << "                 create: creates the pastry henceforth will be known by this node’s address and decided port." << endl;
	cout << "           join <x> <p>: Join the pastry with x address and port p." << endl;
	cout << "      put <key> <value>: insert the given <key,value> pair in the pastry." << endl;
	cout << "              get <key>: returns the value corresponding to the key, if one was previously inserted in the node." << endl;
	cout << "                   lset: Prints the leafset of current node." << endl;
	cout << "             routetable: Prints the routing table of current node." << endl;
	cout << "                   nset: Prints the neighbourhood set of current node." << endl;
	cout << "                   dump: display all information pertaining to current node." << endl;
    cout << "                   quit: Shuts down this node, not the pastry, distributing the data." << endl;
    cout << "               shutdown: shuts down the entire pastry, no node should have any keys or pastry data, the programs at all the terminals should get closed on the notification." << endl;
	cout << "           lset <x> <p>: Prints the leafset of node at address x and port p" << endl;
	cout << "     routetable <x> <p>: Prints the routing table of node at address x and port p." << endl;
	cout << "           nset <x> <p>: Prints the neighbourhood set of node at address x and port p." << endl;
	cout << "           dump <x> <p>: display all information pertaining to node at address x and port p." << endl;
	cout << "                 finger: a list of addresses of all nodes on this pastry." << endl;
	cout << "                dumpall: All information of all the nodes." << endl;
	cout << " store <DAY> <SAT-CODE>: Stores the launch code as parts on the network. DAY ranges from 1-365 and SAT-CODE is any valid combination of ascii characters. If the launch code is already stored, displays warning first and if allowed to proceed, over-writes it." << endl;
	cout << "verify <DAY> <SAT-CODE>: Verify if this SAT-CODE is actually valid for the given day. If the launch code is not stored on the network, reports error." << endl;

}

void port(string port) {

	int portNum = atoi(port.c_str());
	if(portNum > 1024 && portNum < 65536) {
		localNode.setNode(getIp(), port);
		cout << "Port set to " << port << endl;
	} else {
		cout << "Invalid port number" << endl;
	}

}

pthread_t create() {

	if(localNode.isNodeInitialized == false) {
		cout << "Node ip and port are not initialized." << endl;
		return -1;
	}

	if(localNode.serverSockFd >= 0) {
		cout << "Server is already running." << endl;
		return -1;
	}

	pthread_t serverId = server.start(&localNode);
	if(serverId < 0)
		cout << "Cannot create server thread" << endl;

	//Initialize state table
	cell localNodeCell;
	strcpy(localNodeCell.nodeId, localNode.nodeId.c_str());
	strcpy(localNodeCell.ip, localNode.nodeIp.c_str());
	strcpy(localNodeCell.port, localNode.port.c_str());
	localNode.stateTable.init(localNodeCell);

	//Start state table manager
	stateTableManager.startManager();

	return serverId;

}

void join(string ip, string port) {

	if(localNode.isNodeInitialized == false) {
		cout << "Node ip and port are not initialized." << endl;
		return;
	}

	if(localNode.serverSockFd < 0){
		cout << "Server is not running." << endl;
		return;
	}

	if(localNode.isNodeJoined == true) {
		cout << "Node is already joined to the pastry network." << endl;
		return;
	}

	stateTableManager.joinPhase1(ip, port);

}

void put(string key, string value) {

	char *keyValueString;
	KeyValue keyValue;
	crc_32_type crc;
	char crcString[9];
	string message, response;
	Packet packet;

	//Compute CRC32 hash of key
	crc.process_bytes(key.data(), key.size());
	sprintf(crcString, "%08x", crc.checksum());

	//Build KeyValue structure
	strcpy(keyValue.ip, localNode.nodeIp.c_str());
	strcpy(keyValue.port, localNode.port.c_str());
	strcpy(keyValue.key, key.c_str());
	strcpy(keyValue.value, value.c_str());

	keyValueString = (char *) &(keyValue);
	for(unsigned int i = 0; i < sizeof(KeyValue); i++)
		message.push_back(keyValueString[i]);

	packet.build(localNode.nodeId, crcString, 0, PUT, message);
	client.send(localNode.nodeIp, localNode.port, packet.serialize(), &response);

}

void get(string key) {

	char *keyValueString;
	KeyValue keyValue;
	crc_32_type crc;
	char crcString[9];
	string message, response;
	Packet packet;

	//Compute CRC32 hash of key
	crc.process_bytes(key.data(), key.size());
	sprintf(crcString, "%08x", crc.checksum());

	//Build KeyValue structure
	strcpy(keyValue.ip, localNode.nodeIp.c_str());
	strcpy(keyValue.port, localNode.port.c_str());
	strcpy(keyValue.key, key.c_str());

	keyValueString = (char *) &(keyValue);
	for(unsigned int i = 0; i < sizeof(KeyValue); i++)
		message.push_back(keyValueString[i]);

	packet.build(localNode.nodeId, crcString, 0, GET, message);
	client.send(localNode.nodeIp, localNode.port, packet.serialize(), &response);

}

void lset() {

	localNode.stateTable.leafSet.print();

}

void lset(string ip, string port) {

	Packet packet;
	string message, response;

	struct NodeIdentifier *nodeIdentifier = new NodeIdentifier;
	strcpy(nodeIdentifier->ip,localNode.nodeIp.c_str());
	strcpy(nodeIdentifier->port,localNode.port.c_str());

	char *buffer = (char *) nodeIdentifier;
	for(unsigned int i = 0; i < sizeof(NodeIdentifier); i++)
		message.push_back(buffer[i]);

	delete nodeIdentifier;

	packet.build(localNode.nodeId, "00000000", 0, LSET_REQ, message); //Key value is don't care
	client.send(ip, port, packet.serialize(), &response);

}

void routetable() {

	localNode.stateTable.routingTable.print();

}

void routetable(string ip, string port) {

	Packet packet;
	string message, response;

	struct NodeIdentifier *nodeIdentifier = new NodeIdentifier;
	strcpy(nodeIdentifier->ip,localNode.nodeIp.c_str());
	strcpy(nodeIdentifier->port,localNode.port.c_str());

	char *buffer = (char *) nodeIdentifier;
	for(unsigned int i = 0; i < sizeof(NodeIdentifier); i++)
		message.push_back(buffer[i]);

	delete nodeIdentifier;

	packet.build(localNode.nodeId, "00000000", 0, ROUTETABLE_REQ, message); //Key value is don't care
	client.send(ip, port, packet.serialize(), &response);

}

void nset() {

	localNode.stateTable.neighbourhoodSet.print();

}

void nset(string ip, string port) {

	Packet packet;
	string message, response;

	struct NodeIdentifier *nodeIdentifier = new NodeIdentifier;
	strcpy(nodeIdentifier->ip,localNode.nodeIp.c_str());
	strcpy(nodeIdentifier->port,localNode.port.c_str());

	char *buffer = (char *) nodeIdentifier;
	for(unsigned int i = 0; i < sizeof(NodeIdentifier); i++)
		message.push_back(buffer[i]);

	delete nodeIdentifier;

	packet.build(localNode.nodeId, "00000000", 0, NSET_REQ, message); //Key value is don't care
	client.send(ip, port, packet.serialize(), &response);

}

void dump() {

	localNode.stateTable.print();
	cout << endl << "Local Hash Table" << endl;
	map<string, string> ::iterator it;
	int totalEntries = 0;
	for(it = localNode.HT.begin(); it != localNode.HT.end(); it++) {
		cout << it->first << ": " << it->second << endl;
		totalEntries++;
	}
	cout << endl << "Total entries in Local Hash Table: " << totalEntries << endl;

}

void dump(string ip, string port) {

	Packet packet;
	string message, response;

	struct NodeIdentifier *nodeIdentifier = new NodeIdentifier;
	strcpy(nodeIdentifier->ip,localNode.nodeIp.c_str());
	strcpy(nodeIdentifier->port,localNode.port.c_str());

	char *buffer = (char *) nodeIdentifier;
	for(unsigned int i = 0; i < sizeof(NodeIdentifier); i++)
		message.push_back(buffer[i]);

	delete nodeIdentifier;

	packet.build(localNode.nodeId, "00000000", 0, DUMP_REQ, message); //Key value is don't care
	client.send(ip, port, packet.serialize(), &response);

}

void quit() {

	localNode.stateTable.purge(localNode.nodeId);

	FloodCommand floodCommand;
	char *floodCommandString;

	floodCommand.command = QUIT;
	strcpy(floodCommand.arg1, localNode.nodeId.c_str());

	floodCommandString = (char *) &floodCommand;

	string message;
	for(unsigned int i = 0; i < sizeof(FloodCommand); i++)
		message.push_back(floodCommandString[i]);

	client.flood(localNode.nodeId, message);
	cout << "Redistribution starts in 5 seconds..." << endl;
	sleep(5);
	cout << "Redistributing..." << endl;
	htManager.redistribute();
	signal_callback_handler(0);

}

void shutdown() {

	FloodCommand floodCommand;
	char *floodCommandString;

	floodCommand.command = SHUTDOWN;
	strcpy(floodCommand.arg1, "\0");

	floodCommandString = (char *) &floodCommand;

	string message;
	for(unsigned int i = 0; i < sizeof(FloodCommand); i++)
		message.push_back(floodCommandString[i]);

	client.flood(localNode.nodeId, message);

	signal_callback_handler(0);

}

void finger()
{
	FloodCommand floodCommand;
	char *floodCommandString;

	floodCommand.command = FLOOD_ID_REQ;
	strcpy(floodCommand.arg1, localNode.nodeIp.c_str());
	strcpy(floodCommand.arg2, localNode.port.c_str());

	floodCommandString = (char *) &floodCommand;

	string message;
	for(unsigned int i = 0; i < sizeof(FloodCommand); i++)
		message.push_back(floodCommandString[i]);

	client.flood(localNode.nodeId, message);

}

void dumpall()
{
	localNode.stateTable.print();

	FloodCommand floodCommand;
	char *floodCommandString;

	floodCommand.command = FLOOD_DUMP_REQ;
	strcpy(floodCommand.arg1, localNode.nodeIp.c_str());
	strcpy(floodCommand.arg2, localNode.port.c_str());

	floodCommandString = (char *) &floodCommand;

	string message;
	for(unsigned int i = 0; i < sizeof(FloodCommand); i++)
		message.push_back(floodCommandString[i]);

	client.flood(localNode.nodeId, message);

}
