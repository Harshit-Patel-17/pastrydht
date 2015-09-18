/*
 * PastryNodeStructure.h
 *
 *  Created on: 18-Sep-2015
 *      Author: Swarnadeep
 */

#ifndef HEADER_PASTRYNODESTRUCTURE_H_
#define HEADER_PASTRYNODESTRUCTURE_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct leafSetStructure
{
	int L;
	vector<string> closestSmallerIds;
	vector<string> closestLargerIds;
}leafSetStructure;

typedef struct neighbourhoodSetStructure
{
	int M;
	vector<string> closestNeighbours;
}neighbourhoodSetStructure;

tyepdef struct routingTableStructure
{
	int rows;
	int cols;
	vector<vector<string> > entries;
}routingTableStructure;

class pastryNode
{
	leafSetStructure leafSet;
	neighbourhoodSetStructure neighbourhoodSet;
	routingTableStructure routingTable;

public:
	pastryNode();
	pastryNode(leafSetStructure leafSet, neighbourhoodSetStructure neighbourhoodSet, routingTableStructure routingTable);

	leafSetStructure getLeafSet() { return leafSet;}
	neighbourhoodSetStructure getNeighbourhoodSet() { return neighbourhoodSet;}
	routingTableStructure getRoutingTable() { return routingTable;}
};

#endif /* HEADER_PASTRYNODESTRUCTURE_H_ */