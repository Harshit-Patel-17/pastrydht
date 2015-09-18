/*
 * StateTable.cpp
 *
 *  Created on: 18-Sep-2015
 *      Author: harshit
 */

#include "../header/StateTable.h"

void LeafSetStructure::print() {

}

void NeighbourhoodSetStructure::print() {

}

void RoutingTableStructure::print() {

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<16;j++)
		{
			if(strlen(entries[i][j].nodeId) == 0)
				cout << "xxxxxxxx ";
			else
				cout << entries[i][j].nodeId << " ";
		}
		cout << endl;
	}

}

StateTable::StateTable() {
	// TODO Auto-generated constructor stub

}

StateTable::~StateTable() {
	// TODO Auto-generated destructor stub
}

StateTable::StateTable(LeafSetStructure leafSet, NeighbourhoodSetStructure neighbourhoodSet, RoutingTableStructure routingTable) {

	this->leafSet = leafSet;
	this->neighbourhoodSet = neighbourhoodSet;
	this->routingTable = routingTable;

}

void StateTable::initializeLeafSet() {
	for(int i=0;i<2;i++)
	{
		strcpy(leafSet.closestSmallerIds[i].ip,"\0");
		strcpy(leafSet.closestSmallerIds[i].port,"\0");
		strcpy(leafSet.closestSmallerIds[i].nodeId,"\0");

		strcpy(leafSet.closestLargerIds[i].ip,"\0");
		strcpy(leafSet.closestLargerIds[i].port,"\0");
		strcpy(leafSet.closestLargerIds[i].nodeId,"\0");
	}
}

void StateTable::initializeRoutingTable() {
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<16;j++)
		{
			strcpy(routingTable.entries[i][j].ip,"\0");
			strcpy(routingTable.entries[i][j].port,"\0");
			strcpy(routingTable.entries[i][j].nodeId,"\0");
		}
	}
}

void StateTable::initializeNeighbourhoodSet() {
	for(int i=0;i<4;i++)
	{
		strcpy(neighbourhoodSet.closestNeighbours[i].ip,"\0");
		strcpy(neighbourhoodSet.closestNeighbours[i].port,"\0");
		strcpy(neighbourhoodSet.closestNeighbours[i].nodeId,"\0");
	}
}

void StateTable::print() {

	leafSet.print();
	routingTable.print();
	neighbourhoodSet.print();

}
