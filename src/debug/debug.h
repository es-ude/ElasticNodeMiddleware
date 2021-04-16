#ifndef ELASTICNODEMIDDLEWARE_DEBUG_H
#define ELASTICNODEMIDDLEWARE_DEBUG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//only for lufa communication over usb
#ifdef LUFA
void debugTask(void);
#endif

uint16_t debugNumInputAvailable(void);

void debugInit(void (*receiveHandler)(uint8_t));

#ifdef UART
void setDebugReceiveHandler(void (*receiveHandler)(uint8_t));
#endif

void debugNewLine(void);

void debugWriteBool(uint8_t input);

void debugWriteLine(char *s);

void debugWriteString(char *s);

void debugWriteStringBlock(char *s);    //only for uart communication over ftdi adapter

void debugWriteStringLength(char *s, uint16_t length);

void debugWriteChar(uint8_t c);

void debugWriteCharBlock(uint8_t c);

uint8_t debugReadCharAvailable(void);

#ifdef UART
void debugReadCharProcessed(void);
#endif

uint8_t debugReadCharBlock(void);

uint8_t debugGetChar(void);

void debugWriteHex8(uint8_t num);

void debugWriteHex16(uint16_t num);

void debugWriteHex32(uint32_t num);

void debugWriteDec8(uint8_t num);

void debugWriteDec16(uint16_t num);

void debugWriteDec32(uint32_t num);

void debugWriteDec32S(int32_t num);

void debugWriteBin4(uint8_t num);

void debugWriteBin8(uint8_t num);

void debugWriteBin32(uint32_t num);

void debugWriteBin(uint32_t num, uint8_t length);

void debugWriteFloat(float num);

void debugWriteFloatFull(float num);

void debugDone(void);

void debugReady(void);

void debugWaitUntilDone(void);

#ifdef UART
uint8_t debugSending(void);
#endif

void debugAck(uint8_t c);

#endif //ELASTICNODEMIDDLEWARE_DEBUG_H
