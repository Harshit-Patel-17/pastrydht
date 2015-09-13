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
#include "global.h"

#define SOCK_CREATE_ERROR -1
#define SOCK_BIND_ERROR -2
#define SOCK_CONNECT_ERROR -3
#define SOCK_READ_ERROR -4
#define SOCK_WRITE_ERROR -5

using namespace std;

class Client {
	int send(string ip, string port, string message, string *response);
public:
	Client();
	virtual ~Client();

	string send(string key, string message);
};

#endif /* HEADER_CLIENT_H_ */
