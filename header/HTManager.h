/*
 * HTManager.h
 *
 *  Created on: 26-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_HTMANAGER_H_
#define HEADER_HTMANAGER_H_

#include "Client.h"

class HTManager {
public:
	HTManager();
	virtual ~HTManager();
	void redistribute();
};

extern HTManager htManager;

#endif /* HEADER_HTMANAGER_H_ */
