#ifndef ELASTICNODEMIDDLEWARE_LUFAUSARTIMPL_TEST_H
#define ELASTICNODEMIDDLEWARE_LUFAUSARTIMPL_TEST_H

#include <stdint.h>

void setUpUsbSerial(void);

void initLufa(void);

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

#endif //ELASTICNODEMIDDLEWARE_LUFAUSARTIMPL_TEST_H
