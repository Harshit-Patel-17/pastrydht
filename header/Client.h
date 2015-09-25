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

class Client {
public:
	cell forward(string key);
	int send(string ip, string port, string message, string *response);
	Client();
	virtual ~Client();

	string send(string key, string message, message_type type = PUT, int hopCount = 0);
};

extern Client client;

#endif /* HEADER_CLIENT_H_ */
