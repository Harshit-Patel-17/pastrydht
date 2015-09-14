/*
 * global.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/global.h"

Node localNode;
Ui ui;
int keyLengthInBits = 32;
int b = 4;
int keyLength = keyLengthInBits / b;

void signal_callback_handler(int signum) {

	cout << endl << "Shutting server down..." << endl;
	if(localNode.serverSockFd != -1) {
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

	//TODO: Compute CRC-32bit hash for nodeIp:port here
	this->nodeId = "12345678"; //Dummy nodeId

	//Register signals and signal handler
	signal(SIGINT, signal_callback_handler);
	signal(SIGTSTP, signal_callback_handler);

	return 0;
}

void Packet::print() {

	cout << "Source Node Id: " << header.srcNodeId << endl;
	cout << "           Key: " << header.key << endl;
	cout << "          Type: " << header.type << endl;
	cout << "       Message: " << message << endl;

}

string Packet::serialize() {

	string packet;
	char type[2], messageLength[20]; //Assuming that messageLength won't be more than 19 digits long
	sprintf(type, "%d", header.type);
	sprintf(messageLength, "%d", header.messageLength);

	packet = "srcNodeId:" + header.srcNodeId + "\n";
	packet += "key:" + header.key + "\n";
	packet += "type:" + string(type) + "\n";
	packet += "messageLength:" + string(messageLength) + "\n";
	packet += "#";
	packet += message;

	return packet;

}

int Packet::deserialize(string packet) {

	int headerItemsRead;
	char srcNodeId[keyLength+1], key[keyLength+1];
	int type, messageLength;

	//Read header fields
	headerItemsRead = sscanf(packet.c_str(), "srcNodeId:%s key:%s type:%d messageLength:%d", srcNodeId, key, &type, &messageLength);
	if(headerItemsRead != 4)
		return -1;
	header.srcNodeId = srcNodeId;
	header.key = key;
	header.type = (message_type)type;
	header.messageLength = messageLength;

	//Extract message
	int messageStart = packet.find_first_of('#');
	message = packet.substr(messageStart + 1, packet.length() - messageStart);

	return 0;
}
