/*
 * PastryNodeStructure.cpp
 *
 *  Created on: 18-Sep-2015
 *      Author: Swarnadeep
 */

#include "../header/PastryNodeStructure.h"

stateTable::stateTable(){
	initializeLeafSet();
	initializeNeighbourhoodSet();
	initializeRoutingTable();
}

stateTable::stateTable(leafSetStructure leafSet, neighbourhoodSetStructure neighbourhoodSet, 
	routingTableStructure routingTable) {

	this->leafSet = leafSet;
	this->neighbourhoodSet = neighbourhoodSet;
	this->routingTable = routingTable;

}

stateTable::initializeLeafSet() {
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

stateTable::initializeRoutingTable() {
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

stateTable::initializeNeighbourhoodSet() {
	for(int i=0;i<4;i++)
	{
		strcpy(neighbourhoodSet.closestNeighbours[i].ip,"\0");
		strcpy(neighbourhoodSet.closestNeighbours[i].port,"\0");
		strcpy(neighbourhoodSet.closestNeighbours[i].nodeId,"\0");
	}
}

stateTable::printState() {
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<16;j++)
		{
			printf("%s ",routingTable.entries[i][j].nodeId);
		}
		printf("\n");
	}
}