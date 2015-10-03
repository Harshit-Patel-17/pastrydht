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
			cout << "-------- ";
		else
			cout << closestIds[i].nodeId << " ";
	}

	cout << endl;

}

void NeighbourhoodSetStructure::print() {

	for(int i=0;i<M;i++)
	{
		if(strlen(closestNeighbours[i].nodeId) == 0)
			cout << "-------- ";
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
				cout << "-------- ";
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

	strcpy(dontAccept, "\0");

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

bool StateTable::purge(string nodeId) {

	bool purgedFromLeafSet = false;

	//Purge from leaf set
	for(int i = 0; i < L+1; i++) {
		if(strcmp(nodeId.c_str(), leafSet.closestIds[i].nodeId) == 0) {
			strcpy(leafSet.closestIds[i].nodeId, "\0");
			strcpy(leafSet.closestIds[i].ip, "\0");
			strcpy(leafSet.closestIds[i].port, "\0");
			purgedFromLeafSet = true;
		}
	}

	//Purge from routing table
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 16; j++) {
			if(strcmp(nodeId.c_str(), routingTable.entries[i][j].nodeId) == 0) {
				strcpy(routingTable.entries[i][j].nodeId, "\0");
				strcpy(routingTable.entries[i][j].ip, "\0");
				strcpy(routingTable.entries[i][j].port, "\0");
			}
		}
	}

	//Purge from neighborhood set
	for(int i = 0; i < M; i++) {
		if(strcmp(nodeId.c_str(), neighbourhoodSet.closestNeighbours[i].nodeId) == 0) {
			strcpy(neighbourhoodSet.closestNeighbours[i].nodeId, "\0");
			strcpy(neighbourhoodSet.closestNeighbours[i].ip, "\0");
			strcpy(neighbourhoodSet.closestNeighbours[i].port, "\0");
		}
	}

	return purgedFromLeafSet;

}

void StateTable::print() {

	string ip = leafSet.closestIds[L/2].ip;
	string port = leafSet.closestIds[L/2].port;
	cout << "================================================================"
			<< ip << ":" << port
			<< "================================================================" << endl;
	cout << "Leaf Set" << endl;
	leafSet.print();
	cout << endl;
	cout << "Routing Table" << endl;
	routingTable.print();
	cout << endl;
	cout << "Neighborhood Set" << endl;
	neighbourhoodSet.print();
	cout << endl;

}
