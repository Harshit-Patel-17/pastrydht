/*
 * Enums.h
 *
 *  Created on: 27-Sep-2015
 *      Author: harshit
 */

#ifndef HEADER_ENUMS_H_
#define HEADER_ENUMS_H_


enum message_type {GET, PUT, STATE_TABLE, STATE_TABLE_A, STATE_TABLE_Z, STATE_TABLE_X, STATE_TABLE_AZ, JOIN, JOIN_A, VALUE, REDISTRIBUTE, FLOOD};
enum flood_command {QUIT, SHUTDOWN};


#endif /* HEADER_ENUMS_H_ */
