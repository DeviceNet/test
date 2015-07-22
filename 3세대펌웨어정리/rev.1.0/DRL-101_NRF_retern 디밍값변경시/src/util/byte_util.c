/*
 * byte_util.c
 *
 *  Created on: 2012. 7. 18.
 *      Author: GoldSunda
 */

#include "byte_util.h"



uint8 REMOVE_UINT08(uint8 **p) {
	uint8 value = *(*p);
	(*p)++;
	return value;

}
uint16 REMOVE_UINT16(uint8 **p) {
	uint16 value = (uint16) ((((uint8) (*p)[0]) << 8) | ((uint8) (*p)[1]));
	(*p) += 2;
	return value;
}

uint32 REMOVE_UINT32(uint8 **p) {
	uint32 value = MAKE_UINT32((*p)[0], (*p)[1], (*p)[2], (*p)[3]);
	(*p) += 4;
	return value;
}

void APPEND_UINT08(uint8 *buff, size_t *index, uint8 value) {
	buff[*index] = value;
	(*index)++;
}

void APPEND_UINT16(uint8 *buff, size_t *index, uint16 value) {
	buff[(*index)++] = GET_BYTE1(value);
	buff[(*index)++] = GET_BYTE0(value);
}

void APPEND_UINT32(uint8 *buff, size_t *index, uint32 value) {
	buff[(*index)++] = GET_BYTE3(value);
	buff[(*index)++] = GET_BYTE2(value);
	buff[(*index)++] = GET_BYTE1(value);
	buff[(*index)++] = GET_BYTE0(value);
}
