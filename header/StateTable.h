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

/*! \brief cell of StateTable.
 */
typedef struct cell
{
	char ip[16]; /*!< IP address of destination. */
	char port[6]; /*!< Port address of destination. */
	char nodeId[9]; /*!< Hex of CRC32 hash of IP:Port. */
}cell;

/*! \brief Leaf set in StateTable.
 */
typedef struct LeafSetStructure
{
	cell closestIds[L+1]; /*!< Array of cells of leaf set. */

	void print(); /*!< Print leaf set. */
}LeafSetStructure;

/*! \brief Neighborhood set in StateTable.
 */
typedef struct NeighbourhoodSetStructure
{
	cell closestNeighbours[M]; /*!< Array of cell of neighborhood set. */

	void print(); /*!< Print neighborhood set. */
}NeighbourhoodSetStructure;

/*! \brief Routing table in StateTable.
 */
typedef struct RoutingTableStructure
{
	cell entries[8][16]; /*!< Array of cell of routing table. */

	void print(); /*!< Print routing table. */
}RoutingTableStructure;

/*! \brief Pastry StateTable.
 */
struct StateTable {
	int hopCount; /*!< Used for synchronization between phase1 and phase2 of join. */
	char dontAccept[9]; /*!< Used while repairing the leaf set to avoid node ID of departed node to reappear in StateTable. */
	LeafSetStructure leafSet; /*!< LeafSetStructure of StateTable. */
	NeighbourhoodSetStructure neighbourhoodSet; /*!< NeighbourhoodSetStructure of StateTable. */
	RoutingTableStructure routingTable; /*!< TouringTableStructure of StateTable. */
	void initializeLeafSet(); /*!< Initialize leaf set with node id of local node in middle. */
	void initializeNeighbourhoodSet(); /*!< Initialize neighborhood set with one cell having node id of local node. */
	void initializeRoutingTable(); /*!< Initialize unused entries of routing table with node id of local node. */

	StateTable();
	StateTable(const StateTable &stateTable); /*!< Copy constructor. */
	StateTable(LeafSetStructure leafSet, NeighbourhoodSetStructure neighbourhoodSet, RoutingTableStructure routingTable); /*!< Copy constructor. */
	virtual ~StateTable();

	void init(cell localNodeCell); /*!< Initialize leaf set with node id of local node in middle.
									Initialize neighborhood set with one cell having node id of local node.
									Initialize unused entries of routing table with node id of local node. */
	bool purge(string nodeId); /*!< Remove nodeId from the entire StateTable. */
	void print(); /*!< Print StateTable. */
};

#endif /* HEADER_STATETABLE_H_ */
