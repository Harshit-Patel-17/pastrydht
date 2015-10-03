/*
 * Help.cpp
 *
 *  Created on: 19-Sep-2015
 *      Author: harshit
 */

#include "../header/Commands.h"

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

	//TODO: Write help text
	cout << "Program Help" << endl;

}

void port(string port) {

	int portNum = atoi(port.c_str());
	if(portNum > 1024 && portNum < 65536) {
		localNode.setNode("127.0.0.1", port);
		cout << "Port set to " << port << endl;
	} else {
		cout << "Invalid port number" << endl;
	}

}

pthread_t create() {

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

	//TODO: Check correctness of ip and port format
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

	cout << crcString << endl;

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
	for(it = localNode.HT.begin(); it != localNode.HT.end(); it++)
		cout << it->first << ": " << it->second << endl;

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
	htManager.redistribute();

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
