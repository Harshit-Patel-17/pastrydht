/*
 * global.cpp
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#include "../header/global.h"

int Node::setNode(string nodeIp, string port, void (*callback)(string)) {

	if(isLocalNodeInitialized)
		return -1;
	this->nodeIp = nodeIp;
	this->port = port;
	this->callback = callback;
	this->isLocalNodeInitialized = true;
	//TODO: Compute CRC-32bit hash for nodeIp:port here

	return 0;
}
