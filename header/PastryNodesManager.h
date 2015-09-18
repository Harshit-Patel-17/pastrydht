/*
 * PastryNodesManager.h
 *
 *  Created on: 18-Sep-2015
 *      Author: Swarnadeep
 */

#ifndef HEADER_PASTRYNODESMANAGER_H_
#define HEADER_PASTRYNODESMANAGER_H_

#include "PastryNodeStructure.h"

class pastryNodesManager
{
	map<string,stateTable *> pastryNodesMap;
public:
	pastryNodesManager();
	void addPastryNode(string nodeId);
	void removePastryNode(string nodeId);
	/* sends the message to the next node in the route, nodeId is the current nodeId, key is the nodeId we want to reach */
	string forward(string nodeId, string key, string message); 
};

#endif /* HEADER_PASTRYNODESMANAGER_H_ */
