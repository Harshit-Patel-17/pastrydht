/*
 * PastryNodeStructure.cpp
 *
 *  Created on: 18-Sep-2015
 *      Author: Swarnadeep
 */

#include "../header/PastryNodeStructure.h"

pastryNode::pastryNode(){

}

pastryNode::pastryNode(leafSetStructure leafSet, neighbourhoodSetStructure neighbourhoodSet, 
	routingTableStructure routingTable) {

	this->leafSet = leafSet;
	this->neighbourhoodSet = neighbourhoodSet;
	this->routingTable = routingTable;

}