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
#include <vector>
#include "global.h"
#include "Client.h"
#include "HTManager.h"

using namespace std;

class StateTableManager {
public:
	vector<int> hopCountVector;
	bool zReceived;
	queue< pair<pair<string, StateTable*>, message_type> > Q;
	StateTableManager();
	virtual ~StateTableManager();

	void insertInQ(string nodeId, StateTable stateTable, message_type type);
	void joinPhase1(string destNodeIp, string destPort);
	void joinPhase2();
	void redistributePhase();
	pthread_t startManager();
	void updateLeafSet(pair<pair<string, StateTable*>, message_type> QElem);
	void updateLeafSet(cell nodeCell);
	void updateNeighbourhoodSet(pair<pair<string, StateTable*>, message_type> QElem);
	void updateNeighbourhoodSet(cell nodeCell);
	bool allStateTableReceived();
	void clearAll();
};

extern StateTableManager stateTableManager;

#endif /* HEADER_STATETABLEMANAGER_H_ */
