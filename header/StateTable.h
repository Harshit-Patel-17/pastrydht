/*
 * StateTable.h
 *
 *  Created on: 18-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_STATETABLE_H_
#define HEADER_STATETABLE_H_

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

typedef struct LeafSetStructure
{
	cell closestSmallerIds[2];
	cell closestLargerIds[2];

	void print();
}LeafSetStructure;

typedef struct NeighbourhoodSetStructure
{
	cell closestNeighbours[4];

	void print();
}NeighbourhoodSetStructure;

typedef struct RoutingTableStructure
{
	cell entries[8][16];

	void print();
}RoutingTableStructure;

struct StateTable {
	LeafSetStructure leafSet;
	NeighbourhoodSetStructure neighbourhoodSet;
	RoutingTableStructure routingTable;
	void initializeLeafSet();
	void initializeNeighbourhoodSet();
	void initializeRoutingTable();

	StateTable();
	StateTable(LeafSetStructure leafSet, NeighbourhoodSetStructure neighbourhoodSet, RoutingTableStructure routingTable);
	virtual ~StateTable();

	void print();
};

#endif /* HEADER_STATETABLE_H_ */
