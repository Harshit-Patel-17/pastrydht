/*
 * StateTableManager.h
 *
 *  Created on: 19-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_STATETABLEMANAGER_H_
#define HEADER_STATETABLEMANAGER_H_

#include <string>
#include <queue>
#include "global.h"
#include "Client.h"

using namespace std;

class StateTableManager {
public:
	queue< pair<pair<string, StateTable*>, message_type> > Q;
	StateTableManager();
	virtual ~StateTableManager();

	void insertInQ(string nodeId, StateTable stateTable, message_type type);
	void join(string destNodeIp, string destPort);
	pthread_t startManager();
};

extern StateTableManager stateTableManager;

#endif /* HEADER_STATETABLEMANAGER_H_ */
