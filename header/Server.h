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
#include "Client.h"
#include "StateTableManager.h"
#include "HTManager.h"

using namespace std;

/*! \brief Server class to accept incoming client requests.
 */
class Server {
public:
	Server();
	virtual ~Server();

	pthread_t start(Node *node); /*!< Start server thread on port defined by Node structure. */
};

extern Server server;

#endif /* HEADER_SERVER_H_ */
