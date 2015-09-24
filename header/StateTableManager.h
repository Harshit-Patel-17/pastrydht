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
#include <map>
#include "global.h"
#include "Client.h"

using namespace std;

class StateTableManager {
public:
	queue< pair<pair<string, StateTable*>, message_type> > Q;
	StateTableManager();
	virtual ~StateTableManager();

	void insertInQ(string nodeId, StateTable stateTable, message_type type);
	void joinPhase1(string destNodeIp, string destPort);
	void joinPhase2();
	pthread_t startManager();
	void updateLeafSet(pair<pair<string, StateTable*>, message_type> QElem);
	void updateLeafSet(cell nodeCell);
	void updateNeighbourhoodSet(pair<pair<string, StateTable*>, message_type> QElem);
};

extern StateTableManager stateTableManager;

#endif /* HEADER_STATETABLEMANAGER_H_ */
