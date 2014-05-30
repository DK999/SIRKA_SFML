
/*
 * crc.h
 *
 *  Created on: 24.03.2014
 *      Author: Labor
 */
#include <stdint.h>

#pragma once

class CRC16
{
public:
    CRC16();
    void crcInit(void);
    uint16_t CreateCRC(uint16_t crc, uint8_t data);

private:
    const uint16_t m_CRC16_Table[256];
    uint16_t m_crcTable[256];

};
