/*
 * StateTableManager.h
 *
 *  Created on: 19-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_STATETABLEMANAGER_H_
#define HEADER_STATETABLEMANAGER_H_

#include <string>
#include "global.h"
#include "Client.h"

using namespace std;

class StateTableManager {
public:
	StateTableManager();
	virtual ~StateTableManager();

	void join(string destNodeIp, string destPort);
};

extern StateTableManager stateTableManager;

#endif /* HEADER_STATETABLEMANAGER_H_ */
