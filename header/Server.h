/*
 * Server.h
 *
 *  Created on: 13-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_SERVER_H_
#define HEADER_SERVER_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include "global.h"

#define SOCK_CREATE_ERROR -1
#define SOCK_BIND_ERROR -2
#define SOCK_ACCEPT_ERROR -3
#define SOCK_READ_ERROR -4
#define SOCK_WRITE_ERROR -5

using namespace std;

class Server {
public:
	Server();
	virtual ~Server();

	pthread_t start(Node *node);
};

#endif /* HEADER_SERVER_H_ */
