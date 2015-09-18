/*
 * PastryNodesManager.cpp
 *
 *  Created on: 18-Sep-2015
 *      Author: Swarnadeep
 */

#include "../header/PastryNodesManager.h"

pastryNodesManager::pastryNodesManager() {

}

pastryNodesManager::addPastryNode(string nodeId) {
	if(pastryNodesMap.find(nodeId) == pastryNodesMap.end())
	{
		pastryNode *newPastryNode = pastryNode();
		map[nodeId] = newPastryNode;
	}
}

pastryNodesManager::removePastryNode(int nodeId) {
	pastryNode *nodeToBeRemoved = pastryNodesMap[nodeId];
	pastryNodesMap.erase(nodeId);
	delete nodeToBeRemoved;
}

pastryNodesManager::forward(string nodeId, string key, string message) {
	pastryNode *currentNode = pastryNodesMap[nodeId];
	leafSetStructure leafSet = currentNode->getLeafSet();

	string smallestInLeafSet = leafSet.closestSmallerIds[0], largestInLeafSet = leafSet.closestLargerIds[0];

	// Find the node with the smallest id 
	for(int i=1;i<(l/2);i++) {
		if(leafSet.closestSmallerIds[i] < smallestInLeafSet) {
			smallestInLeafSet = leafSet.closestSmallerIds[i];
		}
	}

	// Find the node with the largest id
	for(int i=1;i<(l/2);i++) {
		if(leafSet.closestLargerIds[i] > largestInLeafSet) {
			largestInLeafSet = leafSet.closestLargerIds[i];
		}
	}

	// Check if key lies in the range, if so forward the message to the node with whom the distance of key is minimum
	if(key >= smallestInLeafSet && key <= largestInLeafSet) {
		// find Li which has the smallest distance from key and forward the message to Li
	}
	else {
		int matchingPrefixLength = 0;
		for(int i=0;i<nodeId.size();i++) {
			if(nodeId[i] == key[i]) matchingPrefixLength++;
		}

		routingTableStructure routingTable = currentNode->getRoutingTable();
		int firstUnmatchingDigit = key[matchingPrefixLength];
		string nearestNodeId = routingTable.entries[matchingPrefixLength][firstUnmatchingDigit];

	}

}