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
#include <string.h>

using namespace std;

typedef struct cell 
{
	char ip[16];
	char port[6];
	char nodeId[9];
}cell;

typedef struct leafSetStructure
{
	cell closestSmallerIds[2];
	cell closestLargerIds[2];
}leafSetStructure;

typedef struct neighbourhoodSetStructure
{
	cell closestNeighbours[4];
}neighbourhoodSetStructure;

tyepdef struct routingTableStructure
{
	cell entries[8][16];
}routingTableStructure;

class stateTable
{
	leafSetStructure leafSet;
	neighbourhoodSetStructure neighbourhoodSet;
	routingTableStructure routingTable;
	void initializeLeafSet();
	void initializeNeighbourhoodSet();
	void initializeRoutingTable();

public:
	pastryNode();
	pastryNode(leafSetStructure leafSet, neighbourhoodSetStructure neighbourhoodSet, routingTableStructure routingTable);

	leafSetStructure getLeafSet() { return leafSet;}
	neighbourhoodSetStructure getNeighbourhoodSet() { return neighbourhoodSet;}
	routingTableStructure getRoutingTable() { return routingTable;}

	void printState();
};

#endif /* HEADER_PASTRYNODESTRUCTURE_H_ */