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

#define L 4
#define M 4

using namespace std;

typedef struct cell
{
	char ip[16];
	char port[6];
	char nodeId[9];
}cell;

typedef struct LeafSetStructure
{
	cell closestIds[L+1];

	void print();
}LeafSetStructure;

typedef struct NeighbourhoodSetStructure
{
	cell closestNeighbours[M];

	void print();
}NeighbourhoodSetStructure;

typedef struct RoutingTableStructure
{
	cell entries[8][16];

	void print();
}RoutingTableStructure;

struct StateTable {
	int hopCount; //Useful in joining
	LeafSetStructure leafSet;
	NeighbourhoodSetStructure neighbourhoodSet;
	RoutingTableStructure routingTable;
	void initializeLeafSet();
	void initializeNeighbourhoodSet();
	void initializeRoutingTable();

	StateTable();
	StateTable(const StateTable &stateTable);
	StateTable(LeafSetStructure leafSet, NeighbourhoodSetStructure neighbourhoodSet, RoutingTableStructure routingTable);
	virtual ~StateTable();

	void init(cell localNodeCell);
	void print();
};

#endif /* HEADER_STATETABLE_H_ */
