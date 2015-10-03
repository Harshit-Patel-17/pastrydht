/*
 * Client.h
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_CLIENT_H_
#define HEADER_CLIENT_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include "global.h"

using namespace std;

/*! \brief Client class to start communication with peer.
 */
class Client {
	unsigned int abs_difference(string hex1, string hex2);
	unsigned int abs_difference(unsigned int hexNum1, unsigned int hexNum2);
public:
	cell forward(string key); /*!< Find next hop in pastry network based on key. */
	int send(string ip, string port, string message, string *response); /*!< Send message to peer with given ip and port. */
	Client();
	virtual ~Client();

	string send(string key, string message, message_type type, int hopCount = 0); /*!< Route message with given key. */
	void flood(string prevKey, string message); /*!< Flood message to each node of pastry network. */
};

extern Client client;

#endif /* HEADER_CLIENT_H_ */
