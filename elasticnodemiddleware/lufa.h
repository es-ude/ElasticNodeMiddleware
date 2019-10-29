//
// Created by annika on 29.10.19.
//
//
// Created by Alwyn Burger on 2019-08-28.
//

#ifndef LUFA_H
#define LUFA_H

#include <stdint.h>

void initLufa(void);
void lufaDisable(void);
uint8_t lufaOutputAvailable(void);
void lufaWriteByte(uint8_t c);
void lufaWriteStringLength(char *s, uint16_t length);
void lufaWriteString(char *s);
void lufaWaitUntilDone(void);
uint16_t lufaNumInputAvailable(void);
uint8_t lufaReadAvailable(void);
uint8_t lufaGetChar();
int16_t lufaReadByte(void);
uint16_t lufaReadByteBlocking(void);
void lufaTask(void);

#endif
