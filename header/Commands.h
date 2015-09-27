/*
 * Help.h
 *
 *  Created on: 19-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_COMMANDS_H_
#define HEADER_COMMANDS_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include "../header/global.h"
#include "../header/Server.h"
#include "../header/Client.h"
#include "../header/StateTableManager.h"

using namespace std;

void printHelp();
void port(string port);
pthread_t create();
void join(string ip, string port);
void put(string key, string value);
void get(string key);
void lset();
void routetable();
void nset();
void dump();
void quit();
void shutdown();

#endif /* HEADER_COMMANDS_H_ */
