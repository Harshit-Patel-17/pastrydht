/*
 * global.h
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_GLOBAL_H_
#define HEADER_GLOBAL_H_


#include <string>

using namespace std;

struct Node {
	string nodeIp;
	string nodeId;
	string port;
	void (*callback)(string);
	bool isLocalNodeInitialized;
	Node() {
		callback = NULL;
		isLocalNodeInitialized = false;
	}
	int setNode(string nodeIp, string port, void (*callback)(string));
};


#endif /* HEADER_GLOBAL_H_ */
