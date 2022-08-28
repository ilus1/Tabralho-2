/*
 * crc16.h
 *
 *  Created on: 18/03/2014
 *      Author: Renato Coral Sampaio
 */

#ifndef CRC16_H_
#define CRC16_H_

#include <cstring>
#include <stdint.h>

uint16_t CRC16(uint16_t crc, char data);
uint16_t computeCrc(unsigned char *commands, int size);

#endif /* CRC16_H_ */