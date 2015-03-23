#ifndef IMUSERIAL_H
#define IMUSERIAL_H

#include "IMUPacket.h"

BOOL LookForIMUPacketInByte(UInt8 pByte, IMUPacket_t *pPkt);

#endif // IMUSERIAL_H