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

extern bool verifyCode;
extern string satCode;

string md5Hex(string s);

void printHelp();
void port(string port);
pthread_t create();
void join(string ip, string port);
void put(string key, string value);
void get(string key);
void lset();
void lset(string ip, string port);
void routetable();
void routetable(string ip, string port);
void nset();
void nset(string ip, string port);
void dump();
void dump(string ip, string port);
void quit();
void shutdown();
void store(string day, string sat_code, int n);
void retrieve(string day, int n);

#endif /* HEADER_COMMANDS_H_ */
