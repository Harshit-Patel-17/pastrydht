/*
 * global.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/global.h"

Node localNode;
Ui ui;
const int keyLengthInBits = 32;
const int b = 4;
const int l = pow(2, b);
const int m = l;
const int keyLength = keyLengthInBits / b;

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
	crc_32_type crc;
	char crcString[9];
	crc.process_bytes((nodeIp + port).data(), (nodeIp + port).size());
	sprintf(crcString, "%x\n", crc.checksum());
	this->nodeId = crcString;

	//Register signals and signal handler
	signal(SIGINT, signal_callback_handler);
	signal(SIGTSTP, signal_callback_handler);

	return 0;
}

void Packet::print() {

	cout << "Source Node Id: " << header.srcNodeId << endl;
	cout << "           Key: " << header.key << endl;
	cout << "          Type: " << header.type << endl;
	cout << "Message Length: " << header.messageLength << endl;
	cout << "       Message: " << message << endl;

}

string Packet::serialize() {

	string packet;
	char type[3], messageLength[20]; //Assuming that messageLength field won't be more than 19 digits long
	sprintf(type, "%d", header.type);
	sprintf(messageLength, "%d", header.messageLength);

	packet = "srcNodeId:" + header.srcNodeId + "\n";
	packet += "key:" + header.key + "\n";
	packet += "type:" + string(type) + "\n";
	packet += "messageLength:" + string(messageLength) + "\n";
	packet += "#";

	for(unsigned int i = 0; i < message.length(); i++)
		packet.push_back(message[i]);

	return packet;

}

int Packet::deserialize(char *packet) {

	int headerItemsRead;
	char srcNodeId[keyLength+1], key[keyLength+1];
	int type, messageLength;

	//Read header fields
	headerItemsRead = sscanf(packet, "srcNodeId:%s key:%s type:%d messageLength:%d", srcNodeId, key, &type, &messageLength);
	if(headerItemsRead != 4)
		return -1;
	header.srcNodeId = srcNodeId;
	header.key = key;
	header.type = (message_type)type;
	header.messageLength = messageLength;

	//Extract message
	int messageStart;
	for(messageStart = 0; packet[messageStart] != '#'; messageStart++);
	messageStart++;
	message = "";
	for(int i = 0; i < messageLength; i++)
		message.push_back(packet[messageStart + i]);

	return 0;
}
