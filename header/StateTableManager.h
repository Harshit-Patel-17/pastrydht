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

/*! \brief Local StateTable manager.
 */
class StateTableManager {
	bool isInLeafSet(cell nodeCell); /*!< Check whether node cell is in leaf set of local state table. */
	bool isInNeighborhoodSet(cell nodeCell); /*!< Check whether node cell is in neighborhood set of local state table. */
public:
	vector<int> hopCountVector; /*!< Used for synchronization of phase1 and phase2 of join. */
	bool zReceived; /*!< Used for synchronization of phase1 and phase2 of join. */
	queue< pair<pair<string, StateTable*>, message_type> > Q; /*!< Stores state tables received from peers for processing. */
	StateTableManager();
	virtual ~StateTableManager();

	void insertInQ(string nodeId, StateTable stateTable, message_type type); /*!< Insert state table in queue. */
	void joinPhase1(string destNodeIp, string destPort); /*!< Join phase1. */
	void joinPhase2(); /*!< Join phase2. */
	void redistributePhase(); /*!< Redistribution phase once a new node joins. */
	pthread_t startManager(); /*!< Start StateTableManager thread. */
	void updateLeafSet(pair<pair<string, StateTable*>, message_type> QElem); /*!< Update leaf set using received state table. */
	void updateLeafSet(cell nodeCell); /*!< Update leaf set using received state table. */
	void updateNeighbourhoodSet(pair<pair<string, StateTable*>, message_type> QElem); /*!< Update neighborhood set using received state table. */
	void updateNeighbourhoodSet(cell nodeCell); /*!< Update neighborhood set using received state table. */
	bool allStateTableReceived(); /*!< Used for synchronization of phase1 and phase2 of join. */
	void clearAll(); /*!< Used for synchronization of phase1 and phase2 of join. */
	void send(StateTable *stateTable, string ip, string port, string key, message_type type, int hopCount); /*!< Send local state table to peer with given IP and Port addresses. */
	void repairLeafSet(string purgedNodeId); /*!< Initiate repairing of leaf set once an entry from leaf set is removed. */
};

extern StateTableManager stateTableManager;
extern pthread_mutex_t lock;
extern pthread_mutex_t qaccess;
extern pthread_mutex_t htaccess;
extern pthread_mutex_t staccess;

#endif /* HEADER_STATETABLEMANAGER_H_ */
