#ifndef __XNET_PROTOCOL_H__
#define __XNET_PROTOCOL_H__


#include <string.h>
#include "Util.h"
#include "Xcps.h"
#include "ZRMProtocol.h"

// --------------- XNetMessage Handler --------------- //
void XNetHandler(uint16 srcNetAddr, uint8 buff[], int buff_length);

void XNetCommand(uint8 msg[], int buff_length);

#endif

