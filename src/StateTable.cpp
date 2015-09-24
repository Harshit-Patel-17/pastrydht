/*
 * StateTable.cpp
 *
 *  Created on: 18-Sep-2015
 *      Author: harshit
 */

#include "../header/StateTable.h"

void LeafSetStructure::print() {

	for(int i=0;i<L+1;i++)
	{
		if(strlen(closestIds[i].nodeId) == 0)
			cout << "xxxxxxxx ";
		else
			cout << closestIds[i].nodeId << " ";
	}

	cout << endl;

}

void NeighbourhoodSetStructure::print() {

	for(int i=0;i<M;i++)
	{
		if(strlen(closestNeighbours[i].nodeId) == 0)
			cout << "xxxxxxxx ";
		else
			cout << closestNeighbours[i].nodeId << " ";
	}
	cout << endl;

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

	hopCount = 0;

}

StateTable::StateTable(const StateTable &stateTable) {

	this->hopCount = stateTable.hopCount;
	this->leafSet = stateTable.leafSet;
	this->routingTable = stateTable.routingTable;
	this->neighbourhoodSet = stateTable.neighbourhoodSet;

}

StateTable::~StateTable() {
	// TODO Auto-generated destructor stub
}

StateTable::StateTable(LeafSetStructure leafSet, NeighbourhoodSetStructure neighbourhoodSet, RoutingTableStructure routingTable) {

	this->hopCount = 0;
	this->leafSet = leafSet;
	this->neighbourhoodSet = neighbourhoodSet;
	this->routingTable = routingTable;

}

void StateTable::initializeLeafSet() {
	for(int i=0;i<L+1;i++)
	{
		strcpy(leafSet.closestIds[i].ip,"\0");
		strcpy(leafSet.closestIds[i].port,"\0");
		strcpy(leafSet.closestIds[i].nodeId,"\0");
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
	for(int i=0;i<M;i++)
	{
		strcpy(neighbourhoodSet.closestNeighbours[i].ip,"\0");
		strcpy(neighbourhoodSet.closestNeighbours[i].port,"\0");
		strcpy(neighbourhoodSet.closestNeighbours[i].nodeId,"\0");
	}
}

void StateTable::init(cell localNodeCell) {

	//Initialize leaf-set
	leafSet.closestIds[L/2] = localNodeCell;

	//Initialize routing-table
	string localNodeId = localNodeCell.nodeId;
	for(int i = 0; i < 8; i++) {
		int j = (localNodeId[i] < 58) ? (localNodeId[i] - '0') : (localNodeId[i] - 'a' + 10);
		routingTable.entries[i][j] = localNodeCell;
	}

	//Initialize neighborhood set
	neighbourhoodSet.closestNeighbours[0] = localNodeCell;

}

void StateTable::print() {

	cout << "Leaf Set" << endl;
	leafSet.print();
	cout << endl;
	cout << "Routing Table" << endl;
	routingTable.print();
	cout << endl;
	cout << "Neighbourhood Set" << endl;
	neighbourhoodSet.print();
	cout << endl;

}
