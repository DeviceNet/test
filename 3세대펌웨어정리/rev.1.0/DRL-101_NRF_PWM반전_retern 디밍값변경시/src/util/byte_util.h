/*
 * byte_util.h
 *
 *  Created on: 2012. 7. 18.
 *      Author: GoldSunda
 */

#ifndef BYTE_UTIL_H_
#define BYTE_UTIL_H_

#include "../mytypes.h"
#include "../types.h"

#define	GET_BYTE0(x)	(uint8)(x)
#define	GET_BYTE1(x)	(uint8)((x)>>8)
#define	GET_BYTE2(x)	(uint8)((x)>>16)
#define	GET_BYTE3(x)	(uint8)((x)>>24)

#define	MAKE_UINT(b3,b2,b1,b0)	((uint32)( (uint32)((uint32)((uint32)(b3))<<24) | (uint32)((uint32)((uint32)(b2))<<16) | ((uint32)((uint32)(b1))<<8) | ((uint32)(uint32)(b0)) ))
#define	MAKE_UINT32(b3,b2,b1,b0)	MAKE_UINT(b3,b2,b1,b0)

uint8 REMOVE_UINT08(uint8 **p);
uint16 REMOVE_UINT16(uint8 **p);
uint32 REMOVE_UINT32(uint8 **p);

void APPEND_UINT08(uint8 *buff, size_t *index, uint8 value);
void APPEND_UINT16(uint8 *buff, size_t *index, uint16 value);
void APPEND_UINT32(uint8 *buff, size_t *index, uint32 value);

#endif /* BYTE_UTIL_H_ */
